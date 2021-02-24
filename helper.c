#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "helper.h"


//// Luke
//// Turns input parameter of "1 byte" into a more human friendly readable string
//char* FileAttributes(uint8_t)
//{
//
//}

// Yunhu
// Turns input parameter "int bytes" into a more human friendly readable string.
char* AsHumanSize(uint64_t bytes)
{
	static char str[20] = "";
	char* readable_string;	
	float readable_num = 0;
	readable_num = bytes / 1024;
	if (readable_num <= 1024)
	{
		sprintf(str, "%.2f", readable_num);
		strcat(str, "B");
	}

	else if (readable_num > 1024 && readable_num < 1048576)
	{
		readable_num = readable_num / 1024;
		sprintf(str, "%.2f", readable_num);
		strcat(str, "MB");
	}

	else if (readable_num >= 1048576)
	{
		readable_num = readable_num / 1024;
		readable_num = readable_num / 1024;
		sprintf(str, "%.2f", readable_num);
		strcat(str, "GB");
	}
	readable_string = str;
	return readable_string;
}

//// Yunhu
//// Read input parameter of 1 byte and returns the media type. Examples: HDD, Floppy, Disc, etc.
char* MediaType(uint8_t byte)
{
	switch (byte)
	{
		case 0xE5: return "8-inch single sided floppy";
			break;
		case 0xED: return "5.25-inch double sided floppy.";
			break;
		case 0xF0: return "3.5-inch double sided floppy.";
			break;
		case 0xF5: return "4-sided fixed disk.";
			break;
		case 0xF8: return "Fixed disk. 3.5-inch single sided floppy. 5.25-inch double sided floppy.";
			break;
		case 0xF9: return "3.5-inch double sided floppy.";
			break;
		case 0xFA: return "3.5-inch and 5.25-inch single sided floppy.";
			break;
		case 0xFB: return "3.5-inch and 5.25-inch double sided floppy.";
			break;
		case 0xFC: return "5.25-inch single sided floppy.";
			break;
		case 0xFD: return "5.25-inch double sided floppy. 8-inch double sided floppy. ";
			break;
		case 0xFE: return "5.25-inch single sided floppy. 8-inch single sided floppy. 8-inch double sided floppy.";
			break;
		case 0xFF: return "5.25-inch double sided floppy.";
			break;
	}

}

//// Kevin
//// Reads input parameter of 1 byte and responds with telling us the File System. For example, reading the byte will tell us if the file system is FAT16, FAT32, NTFS, etc.
//char* PartitionTypeName(uint8_t)
//{
//
//}
//
//// Ali
//// Reads data at memory address (void*) till the end of given size, then dumps the hex values. Here is an example output:
//void HexDump(void*, int size)
//{
//
//}
//
//// Alex
//// Fixes up the name of data in our Fat Table. Note that this is only designed for short file. 
//// Short files can only contain a file name with a max size of 8, and a extension name with a max size of 3.
//void FixShortFile(void*)
//{
//
//}