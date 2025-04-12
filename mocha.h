/* The block size of mochafs */
#define MOCHAFS_BLOCKSIZE 1024

/* The address of the superblock */
const int MOCHAFS_SUPERBLOCK_BLOCK_NUMBER = 0;

/* The magic number of mochafs head */
#define MOCHAFS_MAGIC 0xCAFEC0DE

/* The version of mochafs */
#define MOCHAFS_VERSION 1

struct mochafs_super_block {
	uint64_t version;
	uint64_t magic;
    char padding[1008]; // struct 1024 bytes - version(8 bytes) - magic(8 bytes)
};
