// SPDX-License-Identifier: MIT

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/buffer_head.h>

#include "super.h"

static struct kmem_cache *mfs_inode_cachep;

/* mochafs_inode_ops */
static int mochafs_create(struct mnt_idmap *map, struct inode *dir, struct dentry *dentry, umode_t mode, bool excl);

struct dentry *mochafs_lookup(struct inode *parent_inode, struct dentry *child_dentry, unsigned int flags);

static int mochafs_mkdir(struct mnt_idmap *map, struct inode *dir, struct dentry *dentry, umode_t mode);

static struct inode_operations mochafs_inode_ops = {
    .create = mochafs_create,
    .lookup = mochafs_lookup,
    .mkdir = mochafs_mkdir,
};

static int mochafs_create_fs_object(struct inode *dir, struct dentry *dentry, umode_t mode)
{
    return -1;
}

static int mochafs_create(struct mnt_idmap *map, struct inode *dir, struct dentry *dentry, umode_t mode, bool excl)
{
    int ret = -ENOMEM;

    return ret;
}

struct dentry *mochafs_lookup(struct inode *parent_inode, struct dentry *child_dentry, unsigned int flags)
{
    return NULL;
}

static int mochafs_mkdir(struct mnt_idmap *map, struct inode *dir, struct dentry *dentry, umode_t mode)
{
    int ret = -ENOMEM;

    return ret;
}

/* mochafs_sops */
void mochafs_destroy_inode(struct inode *inode)
{
    struct mochafs_inode *mfs_inode = MOCHAFS_INODE(inode);

    printk(KERN_INFO "mochafs: Destroying inode %p [%lu]\n", mfs_inode, inode->i_ino);
    kmem_cache_free(mfs_inode_cachep, mfs_inode);
}

static const struct super_operations mochafs_sops = {
    .destroy_inode = mochafs_destroy_inode,
};

/* fill_super: initalize super block */
static int mochafs_fill_super(struct super_block *sb, void *data, int silent)
{
    struct inode *root_inode;
    struct buffer_head *bh;
    struct mochafs_super_block *sb_disk;
    int ret = -EPERM;

    // sb_bread のブロックサイズを1024にセット
    if (unlikely(sb_set_blocksize(sb, MOCHAFS_BLOCKSIZE) == 0)) {
        printk(KERN_ERR "mochafs: Failed to set block size\n");
        return -ENOMEM;
    }

    // 0番目のブロックをbuffer headへ読み出す
    bh = sb_bread(sb, MOCHAFS_SUPERBLOCK_BLOCK_NUMBER);
    BUG_ON(!bh);

    sb_disk = (struct mochafs_super_block *)bh->b_data;

    printk(KERN_INFO "mochafs: The magic number is [%llu]\n", sb_disk->magic);

    if (unlikely(sb_disk->magic != MOCHAFS_MAGIC)) {
        printk(KERN_ERR "mochafs: Invalid magic number. This is not of type mochafs.\n");
        goto release;
    }

    if (unlikely(sb_disk->version != MOCHAFS_VERSION)) {
        printk(KERN_ERR "mochafs: Invalid version number. This is not of using mochafs version.\n");
        goto release;
    }

    sb->s_magic = MOCHAFS_MAGIC;
    sb->s_fs_info = sb_disk;
    sb->s_maxbytes = MOCHAFS_BLOCKSIZE;
    sb->s_op = &mochafs_sops;

    // inodeの初期化
    root_inode = new_inode(sb);
    if (!root_inode) {
        printk(KERN_ERR "mochafs: Failed to create new inode\n");
        ret = -ENOMEM;
        goto release;
    }

    root_inode->i_ino = MOCHAFS_ROOTDIR_INODE_NUMBER;
    root_inode->i_uid = make_kuid(sb->s_user_ns, 0);
    root_inode->i_gid = make_kgid(sb->s_user_ns, 0);
    root_inode->i_mode = S_IFDIR | 0755;
    root_inode->i_sb = sb;
    root_inode->i_op = &mochafs_inode_ops;

    sb->s_root = d_make_root(root_inode);

    if (!sb->s_root) {
        printk(KERN_ERR "mochafs: Failed to create root dentry\n");
        ret = -ENOMEM;
        goto release;
    }

    ret = 0; // Success

release:
    brelse(bh);

    return ret;
}

struct dentry *mochafs_mount(struct file_system_type *fs_type, int flags, const char *dev_name, void *data)
{
    struct dentry *root_dentry;

    printk(KERN_INFO "mochafs: Mounting filesystem...\n");

    root_dentry = mount_bdev(fs_type, flags, dev_name, data, mochafs_fill_super);

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
    int ret;

    mfs_inode_cachep = kmem_cache_create("mfs_cache_node", sizeof(struct mochafs_inode), 0, (SLAB_RECLAIM_ACCOUNT | SLAB_MEM_SPREAD), NULL);
    if (!mfs_inode_cachep) {
        printk(KERN_ERR "mochafs: Failed to create inode cache\n");
        return -ENOMEM;
    }

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
    int ret;

    ret = unregister_filesystem(&mochafs_fs_type);
    kmem_cache_destroy(mfs_inode_cachep);

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
