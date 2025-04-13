#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

#include "mocha.h"

static int write_superblock(int fd) {
    struct mochafs_super_block sb;
    int ret = 0;

    sb.version = MOCHAFS_VERSION;
    sb.magic = MOCHAFS_MAGIC;

    // Write the superblock to the file
    if (write(fd, &sb, sizeof(sb)) != MOCHAFS_BLOCKSIZE) {
        perror("Error write");
        ret = -1;
    }
    printf("write superblock: version=%ld, magic=%ld\n", sb.version, sb.magic);

    return ret;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <device>\n", argv[0]);
        return 1;
    }
    const char *device = argv[1];
    int fd = open(device, O_RDWR);
    if (fd < 0) {
        perror("Error open fd");
        return 1;
    }

    if (write_superblock(fd) < 0) {
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}
