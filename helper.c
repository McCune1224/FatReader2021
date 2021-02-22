#include <helper.h>

// Luke
// Turns input parameter of "1 byte" into a more human friendly readable string
char* FileAttributes(uint8_t)
{

}

// Yunhu
// Turns input parameter "int bytes" into a more human friendly readable string.
char* AsHumanSize(int bytes)
{

}

// Yunhu
// Read input parameter of 1 byte and returns the media type. Examples: HDD, Floppy, Disc, etc.
char* MediaType(uint8_t)
{

}

// Kevin
// Reads input parameter of 1 byte and responds with telling us the File System. For example, reading the byte will tell us if the file system is FAT16, FAT32, NTFS, etc.
char* PartitionTypeName(uint8_t)
{

}

// Ali
// Reads data at memory address (void*) till the end of given size, then dumps the hex values. Here is an example output:
void HexDump(void*, int size)
{

}

// Alex
// Fixes up the name of data in our Fat Table. Note that this is only designed for short file. 
// Short files can only contain a file name with a max size of 8, and a extension name with a max size of 3.
void FixShortFile(void*)
{

}