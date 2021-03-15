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
	
}__attribute__((packed)) FAT_TABLE;

// Root Entry
typedef struct ROOT_ENTRY
{
	uint8_t filename[8];
	uint8_t file_exetension[3];
	uint8_t file_attribute;
	uint8_t reserved[10];
	uint16_t timeOfLastChange;
	uint16_t dateOfLastChange;
	uint8_t first_cluster[2];
	uint32_t file_size;

}__attribute__((packed)) ROOT_ENTRY;

//
//typedef struct ROOT_DIR
//{
//	DIR_ENTRY data[0];
//
//} __attribute__((packed)) ROOT_DIR;


int ReadDiskImage(char* filename);
MBR* ReadMsterBootRecord(FILE* fp, long int offset);
FAT_BOOT* ReadFatBootSector(FILE* fp, long int offset);
FAT_TABLE* ReadFatTable(FILE* fp, long int offset, int count, int fat_sectors, int sector_size);
ROOT_ENTRY* ReadFatRootDirectory(FILE* fp, long int offset, int count);

#endif