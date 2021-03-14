#include <stdio.h>
#include "reader.h"


int ReadDiskImage(char* filename)
{
    return NULL;
}

MBR* ReadMsterBootRecord(FILE* fp, long int offset)
{
    
    int rc = fseek(fp, offset, SEEK_SET);
    if (rc != 0) //if fseek equals 0, it is then successful. If it returns a nonzero, it has failed
    {
        printf("fseek failed, did not reach correct location\n");
        return 0;
    }
    
    char* buffer = (char*)malloc(sizeof(MBR));
    if (buffer == NULL) //when buffer equals, null it will return a null byte which indicates lack of memory
    {
        printf("Error, not enough memory\n");
        return NULL;
    }
    int count = fread(buffer, 1, sizeof(MBR), fp); 
    if (count < sizeof(MBR)) //if the count is less than the size of MBR, it wont be able to read the contents of the file
    {
        printf("The contents of the file are not able to be read\n");
        return NULL; // or would it be return count; or return 0; ??
    }

    return (MBR*)buffer;
}

FAT_BOOT* ReadFatBootSector(FILE* fp, long int offset)
{
    return NULL;
}

FAT_TABLE* ReadFatTable(FILE* fp, long int offset, int count, int fat_sectors, int sector_size)
{
    return NULL;
}

ROOT_DIR* ReadFatRootDirectory(FILE* fp, long int offset, int count)
{
    return NULL;
}