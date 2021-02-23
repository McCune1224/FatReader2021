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
char* PartitionTypeName(uint8_t PartitionType)
{  //binary to hex?
   //printf("Read in the byte and decide PartitionType");
   
   
   switch(PartitionType)
   {
       case 0x01:
            return ("0x01 is FAT12");
               
       case 0x04:
            return ("0x04 is FAT16");
            
       case 0x06:
            return ("0x06 is FAT16B");
        
        case 0x07:
            return("0x07 is NTFS");
        
        case 0x0B:
            return("0x0B is FAT32 With CHS Addressing");
        
        case 0x0C:
            return("0x0C is FAT32 With LBA");
            
        default:
            return ("Partition Type is unknown");
            
   }
   
   
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