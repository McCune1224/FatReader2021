#include <stdio.h>
#include <stdlib.h>
#include "reader.h"


int ReadDiskImage(char* filename)
{
    FILE* fp;
    fp = fopen(filename, "r");
    MBR* mbr;
    mbr = ReadMasterBootRecord(fp, 0);
}

MBR* ReadMasterBootRecord(FILE* fp, long int offset)
{
    int seek_rc = fseek(fp, offset, SEEK_SET);
    if (seek_rc != 0) //if fseek equals 0, it is then successful. If it returns a nonzero, it has failed
    {
        printf("fseek failed, did not reach correct location\n");
        return NULL;
    }
    
    char* buffer = (char*)malloc(sizeof(MBR));
    if (buffer == NULL) //when buffer equals null, it will return a null byte which indicates lack of memory
    {
        printf("Error, not enough memory\n");
        return NULL;
    }
    
    int total_count = fread(buffer, 1, sizeof(MBR), fp); 
    if (total_count < sizeof(MBR)) //if the total count is less than the size of MBR, it wont be able to read the contents of the file
    {
        printf("The contents of the file are not able to be read\n");
        return NULL; 
    }

    return (MBR*)buffer;
}

FAT_BOOT* ReadFatBootSector(FILE* fp, long int offset)
{
    return NULL;
}

FAT_TABLE* ReadFatTable(FILE* fp, long int offset, int count, int fat_sectors, int sector_size)
{
    int seek_rc = fseek(fp, offset, SEEK_SET);

    //Fat Table Seek Error
    if(seek_rc != 0)
    {
        printf("Could not find the data at given offset, %i", offset);
        return NULL;
    }

    int size = count*(fat_sectors * sector_size);
    //        count * 1 Fat Table Entry

    char* buffer = (char*)malloc(size);
        
    //Memory Allocation Error
    if (buffer == NULL)
    {
        printf("There is not enough memory available.");
        return NULL;
    }

    int read_rc = fread(buffer, 1, size, fp);

    //Fat Table Read Error
    if (read_rc < size)
    {
        printf("Incorrect fat_sectors or sector_size paramaters.");
        free(buffer);
        return NULL;
    }
    
    //printf("%x",(FAT_TABLE*)buffer);
    return (FAT_TABLE*)buffer;
}
/*
int main(int argc, char* argv[])
{
    char* filename = "dfr-16-fat.dd";
 
    struct stat fs = {0};
    if (stat(filename, &fs) != 0)
    {
        printf("Error: Could not read file statistics for '%s'\n", filename);
        return 0;
    } 
 
    FILE* fin = fopen(filename, "rb");
    if (fin == NULL)
    {
        printf("Error: Could not open binary file '%s'\n", filename);
        return 0;
    }
    int offsetToFatTable = 0x810800; // taken from hex dump (unique to this image)
    int count = 2;                   // taken from boot sector as seen in hex dump (pretty standard)
    int fat_sectors = 254;           // taken from boot sector as seen in hex dump (unique to this image)
    int sector_size = 512;           // taken from boot sector as seen in hex dump (pretty standard)
 
    FAT_TABLE* fat = ReadFatTable(fin, offsetToFatTable, count, fat_sectors, sector_size);
    printf("data: %08x\n", *(unsigned int*)fat);
    return 1;
}
*/

ROOT_DIR* ReadFatRootDirectory(FILE* fp, long int offset, int count)
{
    return NULL;
}
