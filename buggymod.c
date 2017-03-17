#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/fs.h>

MODULE_LICENSE("BSD");
MODULE_AUTHOR("hidepin");

int fd = 0;
char *path = "/usr/bin/man";
struct file * file;

static int buggymod_init(void) {
  printk(KERN_ALERT "hello buggymod!\n");

  file = filp_open(path, O_WRONLY, 0);

  msleep(10000);

  printk(KERN_ALERT "wakeup!\n");

  return 0;
}

static void buggymod_exit(void) {
  printk(KERN_ALERT "by buggymod!\n");
}

module_init(buggymod_init);
module_exit(buggymod_exit);
