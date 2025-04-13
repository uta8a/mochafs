#include "mocha.h"

static inline struct mochafs_inode *MOCHAFS_SB(struct super_block *sb)
{
    return sb->s_fs_info;
}

static inline struct mochafs_inode *MOCHAFS_INODE(struct inode *inode)
{
    return inode->i_private;
}
