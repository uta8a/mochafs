obj-m := mochafs.o
mochafs-objs := mocha.o
ccflags-y += -DMOCHAFS_DEBUG

all: ko mkfs-mochafs

ko:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

mkfs-mochafs_SOURCES:
	mkfs-mochafs.c mocha.h

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm mkfs-mochafs

install:
	sudo insmod mochafs.ko
uninstall:
	sudo rmmod mochafs.ko
