/*
 * kern_vault.c - Simple procfs-based message vault
 *
 * This module creates a /proc/kern_vault entry that allows
 * userspace applications to store and retrieve a single message.
 *
 * Features:
 *    - Read/Write access through procfs
 *    - Message buffer protected by mutex
 *    - Read access statistics using atomic counter
 *
 * Limitations:
 *    - Stores only one message
 *    - Message is not persistent
 *    - Maximum message size is BUFFER_SIZE - 1 bytes
 *
 * Author: Abdul Kasif <abdulkasif.ra@gmail.com>
 * Licence: GPL
 */

#include <linux/atomic.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE 128

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abdul Kasif <abdulkasif.ra@gmail.com>");
MODULE_DESCRIPTION("Write and read secret message in kernel");

/*
 * Protects access to the shared message buffer.
 */
DEFINE_MUTEX(vault_lock);

/*
 * Stores the current vault message.
 *
 * Protected by vault_lock
 */
static char secret_message[BUFFER_SIZE];

/*
 * Counts the number of read sessions.
 *
 * The counter is incremented only when a read starts
 * from offset zero.
 */
static atomic_t message_counter = ATOMIC_INIT(0);

/**
 * kern_vault_proc_read - Read message from the vault
 * @file: procfs file structure
 * @user_buf: userspace destination buffer
 * @count: requested bytes count
 * @ppos: file position pointer
 *
 * Creates a temporary snapshot of the stored message while
 * holding vault_lock. The snapshot copied to userspace after
 * the mutex is released to minimize lock hold time.
 *
 * The read counter is incremented only when a new read
 * session begins ( if *ppos == 0 ).
 *
 * Context: Process context. Can sleep.
 *
 * Return:
 * Number of bytes copied on success.
 * 0 when end-of-file is reached.
 * -EFAULT if copying to userspace fails.
 */
static ssize_t kern_vault_proc_read(struct file *file, char __user *user_buf,
                                    size_t count, loff_t *ppos) {
  char temp_buf[BUFFER_SIZE];
  size_t message_len;
  unsigned long uncopied_bytes;

  if (*ppos == 0) {
    atomic_inc(&message_counter);
    pr_info("kern_vault: Vault accessed. Total reads: %d\n",
            atomic_read(&message_counter));
  }

  mutex_lock(&vault_lock);

  strscpy(temp_buf, secret_message, BUFFER_SIZE);

  mutex_unlock(&vault_lock);

  message_len = strlen(temp_buf);
  if (*ppos >= message_len) {
    return 0;
  }

  if (count > message_len - *ppos) {
    count = message_len - *ppos;
  }

  uncopied_bytes = copy_to_user(user_buf, temp_buf + *ppos, count);

  if (uncopied_bytes != 0) {
    pr_err("Failed to copy %lu bytes to user space\n", uncopied_bytes);
    return -EFAULT;
  }

  *ppos += count;

  return count;
}

/**
 * kern_vault_proc_write - Store a new message in the vault
 * @file: procfs file structure
 * @user_buf: userspace source buffer
 * @count: supplied bytes count
 * @ppos: file position pointer
 *
 * Copies userspace data into a temporary buffer and then
 * updates the shared message buffer while holding vault_lock.
 *
 * Existing vault message is replaced.
 *
 * Context: Process context. Can sleep.
 *
 * Return:
 * Number of bytes consumed on success.
 * -EFAULT if copying from userspace fails.
 */
static ssize_t kern_vault_proc_write(struct file *file,
                                     const char __user *user_buf, size_t count,
                                     loff_t *ppos) {
  char temp_buf[BUFFER_SIZE];
  size_t bytes_to_copy = (count < BUFFER_SIZE - 1) ? count : BUFFER_SIZE - 1;
  unsigned long uncopied;

  memset(temp_buf, 0, BUFFER_SIZE);

  uncopied = copy_from_user(temp_buf, user_buf, bytes_to_copy);

  if (uncopied != 0) {
    pr_err("kern_vault: Failed to copy %lu bytes from userspace\n", uncopied);
    return -EFAULT;
  }

  temp_buf[bytes_to_copy] = '\0';

  mutex_lock(&vault_lock);

  strscpy(secret_message, temp_buf, BUFFER_SIZE);

  mutex_unlock(&vault_lock);

  *ppos += bytes_to_copy;

  return bytes_to_copy;
}

static struct proc_dir_entry *kern_vault_dir_entry;

static const struct proc_ops kern_vault_proc_ops = {
    .proc_read = kern_vault_proc_read,
    .proc_write = kern_vault_proc_write,
};

/**
 * kern_vault_init - Initialize module resources
 *
 * Creates the /proc/kern_vault entry and prepares the module
 * for use.
 *
 * Return:
 * 0 on success.
 * -ENOMEM when procfs entry creation fails.
 */
static int __init kern_vault_init(void) {

  kern_vault_dir_entry =
      proc_create("kern_vault", 0644, NULL, &kern_vault_proc_ops);

  if (kern_vault_dir_entry == NULL) {
    pr_err("Failed to create /proc/kern_vault entry\n");
    return -ENOMEM;
  }

  pr_info("kern_vault initialized\n");

  return 0;
}

/**
 * kern_vault_exit - Cleanup module resources
 *
 * Removes the procfs entry and release resources associated
 * with the module.
 */
static void __exit kern_vault_exit(void) {

  if (kern_vault_dir_entry) {
    proc_remove(kern_vault_dir_entry);
  }

  pr_info("kern_vault removed\n");
}

module_init(kern_vault_init);
module_exit(kern_vault_exit);
