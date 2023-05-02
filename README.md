extfat utils is a utility to manipulate extfat images.

# Command Options:
In the command line, type ./extfat -(option).
```bash
The following options are:
   i: takes the input file name as an argument.
   o: takes the output file name as an argument. If no filename is provided, it will output file name will be the same as the input file name.
   c: the input file is copied to the output file without change.
   m: access the file with mmap().
   f: access the file with fread().
   v: takes an input file name as an argument. The main and backup boot sectors will be read and checked to see if they are the same. if they are not the same, an error message will be written to stdout.
   x: extracts from the extfat image a file.
   D: Deletes a file from the extfat image. This option requires a file name.
```
# Example Invocations (verify):

Step 1: Create execute file:
> $ make extfat

Step 2: To check the boot sectors, type:
> $ ./extfat -i test.image -v

# Example Invocations (copy):

Step 1: Create execute file:
> $ make extfat

Step 2: To copy file, type:
> $ ./extfat -i README.md -o readme.md -c

Step 3: Compare two files:
> $ md5sum README.md readme.md

If two hash code are the same, then the process is successful

# Internals

## Build Process
```bash
% make clean
% make

To run the tests
% make tests

```

## Examples
There is a directory of examples.  They are compiled by default using the makefile
Before you can run the example programs, you need to have an file image.

```bash
% bash /examples/create_image.bash
% ./examples/fread
% ./examples/mmap
```

## Misc notes, eventually delete.
```
dd if=/dev/zero of=zzz count=1 bs=1G
yum install -y exfat-utils fuse fuse-exfat
losetup /dev/loop0 diskimage 
mount /dev/loop0 /mnt
umount /mnt
```
## References


https://pawitp.medium.com/notes-on-exfat-and-reliability-d2f194d394c2


https://learn.microsoft.com/en-gb/windows/win32/fileio/exfat-specification


https://uta.service-now.com/selfservice?id=ss_kb_article&sys_id=KB0011414


https://nemequ.github.io/munit/#download


https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html


https://www.freecodecamp.org/news/how-to-use-git-and-github-in-a-team-like-a-pro/


https://en.wikipedia.org/wiki/ExFAT


https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax




















## Misc notes, eventually delete.
```
dd if=/dev/zero of=zzz count=1 bs=1G
yum install -y exfat-utils fuse fuse-exfat
losetup /dev/loop0 diskimage 
mount /dev/loop0 /mnt
umount /mnt
```
## References


https://pawitp.medium.com/notes-on-exfat-and-reliability-d2f194d394c2


https://learn.microsoft.com/en-gb/windows/win32/fileio/exfat-specification


https://uta.service-now.com/selfservice?id=ss_kb_article&sys_id=KB0011414


https://nemequ.github.io/munit/#download


https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html


https://www.freecodecamp.org/news/how-to-use-git-and-github-in-a-team-like-a-pro/


https://en.wikipedia.org/wiki/ExFAT


https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax