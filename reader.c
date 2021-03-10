#include <stdio.h>
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
    return NULL;
}