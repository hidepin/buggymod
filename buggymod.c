#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/fs.h>

MODULE_AUTHOR("hidepin");
MODULE_DESCRIPTION("buggymod");
MODULE_LICENSE("BSD");

int fd = 0;
char *path = "/usr/bin/man";
struct file * file;
struct task_struct *k;

static int buggymod_cb(void *arg) {
  printk(KERN_ALERT "cb buggymod!\n");

  file = filp_open(path, O_WRONLY, 0);

  while (!kthread_should_stop()) {
    printk(KERN_ALERT "wakeup buggymod!\n");
    msleep(10000);
  }

  return 0;
}

static int buggymod_init(void) {
  printk(KERN_ALERT "hello buggymod!\n");

  k = kthread_create(buggymod_cb, NULL, "buggymod");

  wake_up_process(k);

  return 0;
}

static void buggymod_exit(void) {
  printk(KERN_ALERT "by buggymod!\n");
  kthread_stop(k);
}

module_init(buggymod_init);
module_exit(buggymod_exit);
