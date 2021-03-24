#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "helper.h"
#include "reader.h"

uint8_t attributeNum = 235; // 11101011 eb

uint8_t partitionNum = 1;

int main(int argc, char* argv[])
{
    //char* attributes = FileAttributes(attributeNum);
    //printf("%s\n", attributes);
    //free(attributes);

	//char* partitionType = PartitionTypeName(partitionNum);
	//printf("%s\n", partitionType);

  	//uint64_t bytesBeforeConverted = 47281957121;
  	//printf("%s\n", AsHumanSize(bytesBeforeConverted));
  	//uint8_t byte = 0xE5;
  	//printf("%s\n", MediaType(byte));

	//char* dumpTest = "\x00\x00Test...?..sdjfasjdfj\0asd;lasm;d\x00\x001,,0maks,";
	//hexDump(dumpTest, 32);
  
    char* filename = "dfr-16-fat.dd";
 
    //struct stat fs = {0};
    //if (stat(filename, &fs) != 0)
    //{
    //    printf("Error: Could not read file statistics for '%s'\n", filename);
    //    return 0;
    //} 
 
    //FILE* fin = fopen(filename, "rb");
    //if (fin == NULL)
    //{
    //   printf("Error: Could not open binary file '%s'\n", filename);
    //    return 1;
    //}

	printf("\n");
	int result = ReadDiskImage(filename);
	printf("%d\n", result);

    //int offsetToFatTable = 0x810800; // taken from hex dump (unique to this image)
    //int count = 2;                   // taken from boot sector as seen in hex dump (pretty standard)
    //int fat_sectors = 254;           // taken from boot sector as seen in hex dump (unique to this image)
    //int sector_size = 512;           // taken from boot sector as seen in hex dump (pretty standard)

    //FAT_TABLE* fat = ReadFatTable(fin, offsetToFatTable, count, fat_sectors, sector_size);
	//printf("data: %08x\n", *(unsigned int*)fat);

    char *filename1 = "filenametxtAAAAAA"; // "filename.txt"
	char *filename2 = "file    docBBBBBB"; // "file.doc"
	char *filename3 = "my_dir     CCCCCC"; // "my_dir"

	printf("\nFile Test:\n");
	printf("%s\n", FixShortFile(filename1));
	printf("File 2 Test:\n");
	printf("%s\n", FixShortFile(filename2));
	printf("Dir Test:\n");
	printf("%s\n", FixShortFile(filename3));


    return 0;
}
