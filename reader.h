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
	uint8_t bootloader[446]; //bootloader of MBR starts with 446 bytes
	PARTITION list[4]; //There are 4 partitions(each one is 16 bytes each)
	uint16_t bootsignature; //using uint16_t as the boot signature is 2 bytes

}__attribute__((packed)) MBR;

// Fat Boot Sector
typedef struct FAT_BOOT
{

}__attribute__((packed)) FAT_BOOT;

// Fat Table
typedef struct FAT_TABLE
{
	
}__attribute__((packed)) FAT_TABLE;

// Root Directory
typedef struct ROOT_DIR
{
	
}__attribute__((packed)) ROOT_DIR;

int ReadDiskImage(char* filename);
MBR* ReadMasterBootRecord(FILE* fp, long int offset);
FAT_BOOT* ReadFatBootSector(FILE* fp, long int offset);
FAT_TABLE* ReadFatTable(FILE* fp, long int offset, int count, int fat_sectors, int sector_size);
ROOT_DIR* ReadFatRootDirectory(FILE* fp, long int offset, int count);

#endif