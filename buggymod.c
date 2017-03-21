#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <asm/uaccess.h>
#include <asm/cacheflush.h>
#include <linux/syscalls.h>
#include <linux/mm.h>
#include <linux/fdtable.h>
#include <linux/delay.h>

MODULE_AUTHOR("hidepin");
MODULE_DESCRIPTION("buggymod");
MODULE_LICENSE("GPL");

static void **syscall_table = (void *) __SYSCALL_TABLE_ADDRESS__;

struct file *file;
char chkword[] = "hogehoge";

asmlinkage long (*orig_sys_close)(unsigned int fd);

asmlinkage long syscall_close_hook(unsigned int fd) {
  if ( get_current_user()->uid.val >= 1000 ) {
    file = fcheck_files(current->files, fd);
    if (strstr(file->f_dentry->d_name.name, chkword) != NULL) {
      printk("buggymod: uid = %u, pid= %d, fd= %d, file=%s\n", get_current_user()->uid.val, current->pid, fd, file->f_dentry->d_name.name);
      msleep(3600000);
    }
  }
  return orig_sys_close(fd);
}

static void save_original_syscall_address(void) {
  orig_sys_close = syscall_table[__NR_close];
}

static void change_page_attr_to_rw(pte_t *pte) {
  set_pte_atomic(pte, pte_mkwrite(*pte));
}

static void change_page_attr_to_ro(pte_t *pte) {
  set_pte_atomic(pte, pte_clear_flags(*pte, _PAGE_RW));
}

static void replace_system_call(void *new) {
  unsigned int level = 0;
  pte_t *pte;

  pte = lookup_address((unsigned long) syscall_table, &level);
  change_page_attr_to_rw(pte);

  syscall_table[__NR_close] = new;
  change_page_attr_to_ro(pte);
}

static int buggymod_init(void)
{
  printk("buggymod: sys_call_table address is 0x%p\n", syscall_table);

  save_original_syscall_address();
  printk("buggymod: original sys_close's address is %p\n", orig_sys_close);

  replace_system_call(syscall_close_hook);

  printk("buggymod: system call replaced\n");
  return 0;
}

static void buggymod_exit(void)
{
  printk("buggymod: cleanup\n");
  if (orig_sys_close)
    replace_system_call(orig_sys_close);
}

module_init(buggymod_init);
module_exit(buggymod_exit);
