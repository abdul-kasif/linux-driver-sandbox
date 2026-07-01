#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/types.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abdul Kasif <abdulkasif.ra@gmail.com>");
MODULE_DESCRIPTION("Display every running tasks currently managed by the CPU");

#define KM_DIR_NAME "kern_monitor"
#define KM_SUMMARY_NAME "summary"
#define KM_PROCESS_LIST_NAME "process_list"

static struct proc_dir_entry *monitor_dir;
static struct proc_dir_entry *summary_entry;
static struct proc_dir_entry *process_list_entry;

static ssize_t km_read(struct file *file, char __user *user_buf, size_t count,
                       loff_t *ppos) {
  pr_info("kern_monitor: km_proc_read accessed\n");
  return 0;
}

static const struct proc_ops km_fops = {.proc_read = km_read};

static int __init km_init(void) {

  monitor_dir = proc_mkdir(KM_DIR_NAME, NULL);

  if (!monitor_dir) {
    pr_err("Failed to create /proc/%s\n", KM_DIR_NAME);
    return -ENOMEM;
  }

  summary_entry = proc_create(KM_SUMMARY_NAME, 0444, monitor_dir, &km_fops);

  if (!summary_entry) {
    pr_err("Failed to create /proc/%s/%s\n", KM_DIR_NAME, KM_SUMMARY_NAME);
    proc_remove(monitor_dir);
    return -ENOMEM;
  }

  process_list_entry =
      proc_create(KM_PROCESS_LIST_NAME, 0444, monitor_dir, &km_fops);

  if (!process_list_entry) {
    pr_err("Failed to create /proc/%s/%s\n", KM_DIR_NAME, KM_PROCESS_LIST_NAME);
    proc_remove(monitor_dir);
    return -ENOMEM;
  }

  pr_info("kern_monitor: Module loaded\n");
  return 0;
}

static void __exit km_exit(void) {
  if (monitor_dir)
    proc_remove(monitor_dir);

  pr_info("kern_monitor: Module removed\n");
}

module_init(km_init);
module_exit(km_exit);
