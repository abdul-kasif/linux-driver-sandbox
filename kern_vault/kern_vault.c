#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abdul Kasif <abdulkasif.ra@gmail.com>");
MODULE_DESCRIPTION("Write and read secret message in kernel");

static int __init kern_vault_init(void) {
  pr_info("kern_vault loaded!\n");
  return 0;
}

static void __exit kern_vault_exit(void) { pr_info("kern_vault removed!\n"); }

module_init(kern_vault_init);
module_exit(kern_vault_exit);
