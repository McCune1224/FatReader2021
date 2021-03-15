#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

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
                    strcat(result, "RO|");//Read Only
                    break;
                case 1:
                    strcat(result, "H|");//Hidden
                    break;
                case 2:
                    strcat(result, "S|");//System
                    break;
                case 3:
                    strcat(result, "V|");//Volume Label
                    break;
                case 4:
                    strcat(result, "Dir|");//Directory
                    break;
                case 5:
                    strcat(result, "A|");//Archive
                    break;
                case 6:
                    strcat(result, "D|");//Device
                    break;
                case 7:
                    strcat(result, "R|");//Reserved
                    break;
            }
        }
        else
        {
            strcat(result, "*|");
        }
    }

    return result;
    free(bitchar);
}

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
		case 0xE5: return "Floppy";
			break;
		case 0xED: return "Floppy";
			break;
		case 0xF0: return "Floppy";
			break;
		case 0xF5: return   "Disk";
			break;
		case 0xF8: return   "Disk";
			break;
		case 0xF9: return "Floppy";
			break;
		case 0xFA: return "Floppy";
			break;
		case 0xFB: return "Floppy";
			break;
		case 0xFC: return "Floppy";
			break;
		case 0xFD: return "Floppy";
			break;
		case 0xFE: return "Floppy";
			break;
		case 0xFF: return "Floppy";
			break;
	}

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
void hexDump(void *addr, int size)
{
    int i;
    char temp[17];
    unsigned char *x = (unsigned char*)addr;

    

    // Process every byte in the data.
    for (i = 0; i < size; i++) 
    {
       
        // Multiple of 8 means new line (with line offset).
        if ((i % 16) == 0) 
        {
            // Padding for proper formatting
            if (i != 0){
                printf("  %s\n", temp);}
               
            // Address offset on the left side
            printf("  %08x ", i);
        }

        // Now the hex code for the specific character.
        printf(" %02x", x[i]);

        // If current character is not ASCII then store it as '?'
        //  from  (space) --------->  (~) in ASCII 
        if ((x[i] < 0x20) || (x[i] > 0x7e)) 
        {
            temp[i % 8] = '.';
        } 
        else 
        {
            temp[i % 8] = x[i];
        }
        // Add a Null terminator to the end of the line so it can be printed
        temp[(i % 8) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf("  %s\n", temp);
}


//FixShortFile tests
char* filename1 = "filenametxtAAAAAA"; // "filename.txt"
char* filename2 = "file    docBBBBBB"; // "file.doc"
char* filename3 = "my_dir     CCCCCC"; // "my_dir"

// Alex
// Fixes up the name of data in our Fat Table. Note that this is only designed for short file. 
// Short files can only contain a file name with a max size of 8, and a extension name with a max size of 3.
void FixShortFile(void *rawFilename)
{
    char *pRawFileName = rawFilename;
    char fullFileName[12] = {'\0'};

    memcpy(fullFileName, pRawFileName, 12);
    printf("Modifying: |%s|\n", fullFileName);


    fullFileName[11] = '\0';
    printf("Result: |%s|\n",fullFileName);
    if (fullFileName[8] == ' ')
    {
        printf("Is a Directory.");
    }
    else
    {
        printf("Is a .%c%c%c File", fullFileName[8],fullFileName[9],fullFileName[10]);
    }
    int count = 0;
}
