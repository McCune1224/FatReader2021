#include <stdio.h>
#include <stdlib.h>
#include "reader.h"

int ReadDiskImage(char* filename)
{
    return NULL;
}

MBR* ReadMsterBootRecord(FILE* fp, long int offset)
{
    return NULL;
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
    int size = count * sizeof(ROOT_ENTRY);

    char* buffer = (char*)malloc(size);
    //                 |
    //Error Check here v
    
    
    int seek_rc = fseek(fp, offset, SEEK_SET);
    if (seek_rc != 0)
    {
        printf("seek failed");
        free(buffer);
        return NULL;
    }


    int read_rc = fread(buffer, sizeof(ROOT_ENTRY), count, fp);
    if (read_rc != count)
    {
        printf("read failed");
        printf("size: %d", size);
        printf("///%d", read_rc);
        free(buffer);
        return NULL;
    }

    //printf("-------------------------------------\n");
    //printf("%x", (ROOT_ENTRY*)buffer);
    //printf("-------------------------------------");
    return (ROOT_DIR*)buffer;
}

