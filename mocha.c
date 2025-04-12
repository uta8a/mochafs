// SPDX-License-Identifier: MIT

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

struct file_system_type mochafs_fs_type = {
    .name = "mochafs",
    .fs_flags = FS_REQUIRES_DEV,
    .owner = THIS_MODULE,
};

static int __init mochafs_init(void)
{
    int ret = 0;

    ret = register_filesystem(&mochafs_fs_type);

    if (likely(ret == 0)) {
        printk(KERN_INFO "mochafs: Filesystem registered successfully\n");
    } else {
        printk(KERN_ERR "mochafs: Failed to register filesystem\n");
    }

    return ret;
}
static void __exit mochafs_exit(void)
{
    int ret = 0;

    ret = unregister_filesystem(&mochafs_fs_type);

    if (likely(ret == 0)) {
        printk(KERN_INFO "mochafs: Filesystem unregistered successfully\n");
    } else {
        printk(KERN_ERR "mochafs: Failed to unregister filesystem\n");
    }
}

module_init(mochafs_init);
module_exit(mochafs_exit);

MODULE_LICENSE("MIT");
MODULE_AUTHOR("Masanori Tani (@uta8a)");
MODULE_DESCRIPTION("A simple filesystem module.");
