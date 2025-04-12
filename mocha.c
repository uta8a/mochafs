// SPDX-License-Identifier: MIT

#include <linux/init.h>
#include <linux/module.h>

static int __init mochafs_init(void)
{
    printk(KERN_INFO "mochafs: Initializing Mochafs module\n");
    return 0;
}
static void __exit mochafs_exit(void)
{
    printk(KERN_INFO "mochafs: Exiting Mochafs module\n");
}

module_init(mochafs_init);
module_exit(mochafs_exit);

MODULE_LICENSE("MIT");
MODULE_AUTHOR("Masanori Tani (@uta8a)");
MODULE_DESCRIPTION("A simple filesystem module.");
