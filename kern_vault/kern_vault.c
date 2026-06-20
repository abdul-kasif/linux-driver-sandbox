#include <linux/atomic.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/types.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abdul Kasif <abdulkasif.ra@gmail.com>");
MODULE_DESCRIPTION("Write and read secret message in kernel");

static char secret_message[128];

static atomic_t message_counter = ATOMIC_INIT(0);

static ssize_t kern_vault_proc_read(struct file *file, char __user *user_buf,
                                    size_t count, loff_t *ppos) {
  pr_info("kern_vault: inside kern_vault_proc_read.\n");
  return 0;
}

static ssize_t kern_vault_proc_write(struct file *file,
                                     const char __user *user_buf, size_t count,
                                     loff_t *ppos) {
  pr_info("kern_vault: inside kern_vault_proc_write.\n");
  return -1;
}

static struct proc_dir_entry *kern_vault_dir_entry;

static const struct proc_ops kern_vault_proc_ops = {
    .proc_read = kern_vault_proc_read,
    .proc_write = kern_vault_proc_write,
};

static int __init kern_vault_init(void) {
  pr_info("kern_vault init: entry\n");

  kern_vault_dir_entry =
      proc_create("kern_vault", 0444, NULL, &kern_vault_proc_ops);

  if (kern_vault_dir_entry == NULL) {
    pr_err("Failed to create /proc/kern_vault entry\n");
    return -ENOMEM;
  }

  pr_info("kern_vault init: exit\n");

  return 0;
}

static void __exit kern_vault_exit(void) {

  pr_info("kern_vault exit: entry\n");

  if (kern_vault_dir_entry) {
    proc_remove(kern_vault_dir_entry);
  }

  pr_info("kern_vault exit: exit\n");
}

module_init(kern_vault_init);
module_exit(kern_vault_exit);
