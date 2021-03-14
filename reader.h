#ifndef READER_HEADER
#define READER_HEADER
#include <stdint.h>

// Partition Type
typedef struct _PARTITION
{
	uint8_t bootable;
	uint8_t first_chs[3];
	uint8_t type;
	uint8_t last_chs[3];
	uint32_t lba_offset;
	uint32_t sector_count;
}__attribute__((packed)) PARTITION;

// Master Boot Record
typedef struct _MBR
{

}__attribute__((packed)) MBR;

// Fat Boot Sector
typedef struct FAT_BOOT
{

}__attribute__((packed)) FAT_BOOT;

// Fat Table
typedef struct FAT_TABLE
{
	uint16_t Table;
}__attribute__((packed)) FAT_TABLE;

// Root Directory
typedef struct ROOT_DIR
{
	
}__attribute__((packed)) ROOT_DIR;

int ReadDiskImage(char* filename);
MBR* ReadMsterBootRecord(FILE* fp, long int offset);
FAT_BOOT* ReadFatBootSector(FILE* fp, long int offset);
FAT_TABLE* ReadFatTable(FILE* fp, long int offset, int count, int fat_sectors, int sector_size);
ROOT_DIR* ReadFatRootDirectory(FILE* fp, long int offset, int count);

#endif