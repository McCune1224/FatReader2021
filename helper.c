#include "helper.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte) \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

// Luke
// Turns input parameter of "1 byte" into a more human friendly readable string
char* FileAttributes(uint8_t byte)
{
    char* bitchar;
    asprintf(&bitchar, BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(byte));
    printf(bitchar);

    char* result = malloc(80);
    strcat(result, "|");

    for (int i = 0; i < strlen(bitchar)-1; i++)
    {
        if(bitchar[i] == '1')
        {
            switch(i)
            {
                case 0:
                    strcat(result, "ReadOnly|");
                    break;
                case 1:
                    strcat(result, "Hidden|");
                    break;
                case 2:
                    strcat(result, "System|");
                    break;
                case 3:
                    strcat(result, "VolumeLabel|");
                    break;
                case 4:
                    strcat(result, "Directory|");
                    break;
                case 5:
                    strcat(result, "Archive|");
                    break;
                case 6:
                    strcat(result, "Device|");
                    break;
                case 7:
                    strcat(result, "Reserved|");
                    break;
            }
        }
        else
        {
            strcat(result, "X|");
        }
    }

    return result;
    free(bitchar);
}

// Yunhu
// Turns input parameter "int bytes" into a more human friendly readable string.
char* AsHumanSize(int bytes)
{

}

// Yunhu
// Read input parameter of 1 byte and returns the media type. Examples: HDD, Floppy, Disc, etc.
char* MediaType(uint8_t byte)
{

}

// Kevin
// Reads input parameter of 1 byte and responds with telling us the File System. For example, reading the byte will tell us if the file system is FAT16, FAT32, NTFS, etc.
char* PartitionTypeName(uint8_t byte)
{

}

// Ali
// Reads data at memory address (void*) till the end of given size, then dumps the hex values. Here is an example output:
void HexDump(void* memory, int size)
{

}

// Alex
// Fixes up the name of data in our Fat Table. Note that this is only designed for short file. 
// Short files can only contain a file name with a max size of 8, and a extension name with a max size of 3.
void FixShortFile(void* memory)
{

}