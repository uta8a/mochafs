// SPDX-License-Identifier: MIT

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

static struct dentry *mochafs_mount(struct file_system_type *fs_type, int flags, const char *dev_name, void *data)
{
    struct dentry *root_dentry;

    printk(KERN_INFO "mochafs: Mounting filesystem...\n");

    root_dentry = mount_bdev(fs_type, flags, dev_name, data, NULL); // TODO: implement fill_super(now is NULL)

    if (unlikely(IS_ERR(root_dentry))) {
        printk(KERN_ERR "mochafs: Mount failed\n");
    } else {
        printk(KERN_INFO "mochafs: Mounted successfully on [%s]\n", dev_name);
    }

    return root_dentry;
}

static void mochafs_kill_superblock(struct super_block *sb)
{
    printk(KERN_INFO "mochafs: Superblock is destroyed. Unmount successful.\n");
    kill_block_super(sb);
}

struct file_system_type mochafs_fs_type = {
    .name = "mochafs",
    .fs_flags = FS_REQUIRES_DEV,
    .mount = mochafs_mount,
    .kill_sb = mochafs_kill_superblock,
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
