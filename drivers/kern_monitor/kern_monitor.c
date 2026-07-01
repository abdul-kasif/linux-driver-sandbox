#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abdul Kasif <abdulkasif.ra@gmail.com>");
MODULE_DESCRIPTION("Display every running tasks currently managed by the CPU");

static int __init kern_monitor_init(void) {
  pr_info("kern_monitor initialized\n");
  return 0;
}

static void __exit kern_monitor_exit(void) {
  pr_info("kern_monitor removed\n");
}

module_init(kern_monitor_init);
module_exit(kern_monitor_exit);
