# Mochafs

Current status:

- [x] loadable as a kernel module
- [x] mountable
- [ ] read/write

```plaintext
[84774.438513] mochafs: Filesystem registered successfully
[84777.500739] loop0: detected capacity change from 0 to 800
[84777.500914] mochafs: Mounting filesystem...
[84777.501031] mochafs: The magic number is [3405693150]
[84777.501042] mochafs: Mounted successfully on [/dev/loop0]
```

```console
ubuntu@ubuntu-cui:~/mochafs$ mount | grep mocha
/home/ubuntu/mochafs/image on /mnt type mochafs (rw,relatime)
```

working with Linux Kernel 6.8

```console
ubuntu@ubuntu-cui:~/mochafs$ uname -r
6.8.0-57-generic
```

reference: https://github.com/psankar/simplefs
