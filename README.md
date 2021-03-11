# FatReader
The CSC 430 class' basic implementtaion of a FAT File Reader System

# Design Document
Team Lead: Luke Frisbee

Date: February 22, 2021

Team Members:

* Kevin Alonso
* Ali Amin
* Luke Frisbee
* Alex McCune
* Yunhu Xiang

# Helper Functions

```c
[ ] Luke char*FileAttributes(uint8_t): 

Turns input parameter of "1 byte" into a more human friendly readable string

Example: 

11101011 → "ReadOnly|Hidden|System|X|Directory|X|Device|Reserved|"

```

```c
[ ] Yunhu char* AsHumanSize(int bytes):

Turns input parameter "int bytes" into a more human friendly readable string. For example:

1440 → "1.44 KB"

1243652 →"1.32 MB" 

111921029101 → "32.19 GB"

```

```c
[ ] Yunhu char* MediaType(uint8_t):

Read input parameter of 1 byte and returns the media type. Examples: HDD, Floppy, Disc, etc.

11101111 → 0xEF → "Floppy"

```

```c
[ ] Kevin char* PartitionTypeName(uint8_t):

Reads input parameter of 1 byte and responds with telling us the File System. For example, reading the byte will tell us if the file system is FAT16, FAT32, NTFS, etc.

Example:

00000001 → 0x01 → "FAT12"

00000100 → 0x04 → "FAT16"

00000110 → 0x06 → "FAT16B"

```

```c
[ ] Ali void HexDump(void*, int size):

Reads data at memory address (void*) till the end of given size, then dumps the hex values
```

```c
[] Alex void FixShortFile(void*):

Fixes up the name of data in our Fat Table. Note that this is only designed for short files. Short files can only contain a file name with a max size of 8, and an extension name with a max size of 3.


For Example:

0123456789ABC
File    .txt<jA...Lma)!M>Am1...

Would Need To Become:

012345678 9 A B C
File.txt\0\0\0\0\0jA...Lma)!M>Am1...
```

