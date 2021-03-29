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
	uint8_t jump_instruction[3]; //Jump instruction has a Field Length of 3 bytes ex:(EB 3C 90)
	uint8_t OEM_name_in_text[8]; //8 bytes for the OEM Name in ASCII text ex:("MSDOS5.0")
	
	//BIOS parameter block
	uint16_t bytes_per_sector; //can be 13,19,21, or 25 bytes used for parameter block 
	uint8_t sectors_per_cluster;
	uint16_t reserved_logical_sectors;
	uint8_t number_of_file_allocation_table;
	uint16_t fat_root_directory_entries;
	uint16_t logical_sectors;
	uint8_t media_descriptor;
	uint16_t logical_sectors_per_alloc_table;

	//DOS 3.1 BPB
	uint16_t phyiscal_sectors_per_track;
	uint16_t num_of_heads_for_disks;
	uint8_t count_of_hidden_sectors[4];
	uint8_t total_logical_sectors[4];

	//BIOS paramter block extended
	uint8_t physical_drive_num;
	uint8_t reserved;
	uint8_t extended_boot_signature;
	uint8_t volume_id[4];
	uint8_t partition_volume_label[11];
	uint8_t file_system_type[8];

	uint8_t bootstrap_code[448]; 
	uint16_t boot_sector_signature; //Indicates compatible boot code and tested by boot loader.
}__attribute__((packed)) FAT_BOOT;

// Fat Table
typedef uint16_t FAT_TABLE_ENTRY; // A single FAT Table entry 
typedef struct FAT_TABLE
{
	FAT_TABLE_ENTRY Table[0]; //An unkown amount of FAT Table Entries
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

typedef struct ROOT_DIR
{
	ROOT_ENTRY data[0];
} __attribute__((packed)) ROOT_DIR;


int ReadDiskImage(char* filename);
MBR* ReadMasterBootRecord(FILE* fp, long int offset);
FAT_BOOT* ReadFatBootSector(FILE* fp, long int offset);
FAT_TABLE* ReadFatTable(FILE* fp, long int offset, int count, int fat_sectors, int sector_size);
ROOT_DIR* ReadFatRootDirectory(FILE* fp, long int offset, int count);

#endif
