#ifndef READER_HEADER
#define READER_HEADER
#include <stdint.h>

#define UNREADABLE 1

#define FILE_ATTRIBUTE_READONLY 0x01
#define FILE_ATTRIBUTE_HIDDEN 0x02
#define FILE_ATTRIBUTE_SYSTEM 0x04
#define FILE_ATTRIBUTE_VOLUME 0x08
#define FILE_ATTRIBUTE_LFN \
    (FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN | \
     FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_VOLUME)
#define FILE_ATTRIBUTE_DIRECTORY 0x10
#define FILE_ATTRIBUTE_ARCHIVE 0x20




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
	uint16_t first_cluster;
	uint32_t file_size;

}__attribute__((packed)) ROOT_ENTRY;

typedef struct ROOT_DIR
{
	ROOT_ENTRY data[0];
} __attribute__((packed)) ROOT_DIR;

ROOT_DIR* g_rootDir;
FAT_BOOT* g_fatBoot;

/*
 * Reads the disk image containing a FAT16 partition
 * Forms data into four structs: MBR, FAT_BOOT, FAT_TABLE, ROOT_DIR
 *
 * -=| Params |=-
 *   filename: string filelocation of disk image
 *
 * -=| Returns |=-
 *   type:    int
 *   success: returns 0
 *   failure: returns 1
 */
int ReadDiskImage(char* filename);

MBR* ReadMasterBootRecord(FILE* fp, long int offset);

FAT_BOOT* ReadFatBootSector(FILE* fp, long int offset);

FAT_TABLE* ReadFatTable(FILE* fp, long int offset, int count, int fat_sectors, int sector_size);

ROOT_DIR* ReadFatRootDirectory(FILE* fp, long int offset, int count);

//stage 2

uint32_t GetFileSize(char* filename);

uint32_t GetFileSizeFromEntry(ROOT_ENTRY* entry);

int GetDirectorySize(char* directory);

int GetDirectorySizeFromEntry(ROOT_ENTRY* entry);

ROOT_ENTRY* GetRootEntry(char* filename);

char* GetFileData(char* targetFile);

char* ReadFileContents(ROOT_ENTRY* entry, char* buffer, int size);

ROOT_DIR* g_rootDir;




#endif
