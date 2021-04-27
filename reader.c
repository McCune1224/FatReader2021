#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "reader.h"
#include "helper.h"
#include "linked_list.h"



#define FILE_ATTRIBUTE_READONLY 0x01
#define FILE_ATTRIBUTE_HIDDEN 0x02
#define FILE_ATTRIBUTE_SYSTEM 0x04
#define FILE_ATTRIBUTE_VOLUME 0x08
#define FILE_ATTRIBUTE_LFN \
    (FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN | \
     FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_VOLUME)
#define FILE_ATTRIBUTE_DIRECTORY 0x10
#define FILE_ATTRIBUTE_ARCHIVE 0x20


static FILE* g_filePointer;
static FAT_BOOT* g_fatBoot;
static FAT_TABLE* g_fatTable;
static ROOT_DIR* g_rootDir;
static uint32_t g_offsetToDataClusters;


ROOT_ENTRY* GetDirEntry(char* filename);

typedef struct _FAT16_ENTRY
{
    uint16_t data;
} __attribute__((packed)) FAT16_ENTRY;

/******************************************************************************
**                              ReadDiskImage                                **
******************************************************************************/

static FILE* g_filePointer;
static FAT_BOOT* g_fatBoot;
static FAT_TABLE* g_fatTable;
static ROOT_DIR* g_rootDir;
static uint32_t g_offsetToDataClusters;

#define FILE_ATTRIBUTE_READONLY 0x01
#define FILE_ATTRIBUTE_HIDDEN 0x02
#define FILE_ATTRIBUTE_SYSTEM 0x04
#define FILE_ATTRIBUTE_VOLUME 0x08
#define FILE_ATTRIBUTE_LFN \
    (FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN | \
     FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_VOLUME)
#define FILE_ATTRIBUTE_DIRECTORY 0x10
#define FILE_ATTRIBUTE_ARCHIVE 0x20

uint32_t GetFileSizeFromEntry(ROOT_ENTRY* entry);
int GetDirectorySizeFromEntry(ROOT_ENTRY* entry);
const char* EightDotThreeString(const uint8_t name[8], const uint8_t ext[3]);
static void RemoveTrailingSpaces(char* fat_filename_buffer);
ROOT_ENTRY* FindMatchingEntryName(char* filename, ROOT_DIR* root_dir, int entries);

ROOT_ENTRY* GetDirEntry(char* filename);
ROOT_ENTRY* Function0(ROOT_DIR* subroot, int num_entries, char* filename);
const char* Function1(const uint8_t name[8], const uint8_t ext[3]);
static void Function2(char* fat_filename_buffer);
const char* Function3(const char* path);
int Function4(ROOT_ENTRY* entry, void** data, uint32_t* size);
int Function5(ROOT_ENTRY* entry, uint32_t* entry_size);
static int Function6(int start_cluster);

int ReadDiskImage(char* filename)
{
    // Open disk image ------------------------------------------------------------------------
    FILE* fp = fopen(filename, "rb");
    g_filePointer = fp;
    if (fp == NULL)
    {
        printf("Error: Could not open binary disk image: '%s'\n", filename);
        return UNREADABLE;
    }

    // Master Boot Record ---------------------------------------------------------------------
    MBR* mbr = ReadMasterBootRecord(fp, 0);
    if(mbr == NULL)
    {
        printf("Error: ReadMasterBootRecord Failed\n");
        return UNREADABLE;
    }
    // HexDump(mbr, 512);

    // Boot Sector ----------------------------------------------------------------------------
    int offsetToBootSector = 0;

    // Iterate through all 4 Partitions of the Master Boot Record
    for(int i = 0; i < 4; i++)
    {
        // Check if partition is FAT16 or FAT16B
        if(mbr->list[i].type == 0x04 || mbr->list[i].type == 0x06)
        {
            offsetToBootSector = mbr->list[i].lba_offset * 512;
        }
    }

    // Check if a FAT16 or FAT16B bootsector was found, if not return failure
    if(offsetToBootSector == 0)
    {
        printf("Error: Could not find BootSector in MBR\n");
        return UNREADABLE;
    }

    FAT_BOOT* boot = ReadFatBootSector(fp, offsetToBootSector);
    g_fatBoot = boot;
    // HexDump(boot, 512);

    // Fat Table ------------------------------------------------------------------------------

    // Get required parameters for ReadFatTable using the FAT_BOOT structure
    int count = boot->number_of_file_allocation_table;
    int fat_sectors = boot->logical_sectors_per_alloc_table;
    int sector_size = boot->bytes_per_sector;
    int offsetToFatTable = offsetToBootSector + (boot->reserved_logical_sectors * sector_size);

    // Print debug data (TEMP, TO BE REMOVED IN FUTURE)
    // printf("%d\n", count);
    // printf("%d\n", fat_sectors);
    // printf("%d\n", sector_size);
    // printf("%d\n", offsetToFatTable);

    FAT_TABLE* fat = ReadFatTable(fp, offsetToFatTable, count, fat_sectors, sector_size);
    g_fatTable = fat;
    if(fat == NULL)
    {
        printf("Error: ReadFatTable Failed\n");
        return UNREADABLE;
    }
    printf("data: %08x\n", *(unsigned int*)fat);


    // Root Directory -------------------------------------------------------------------------
    int offsetToRootDir = offsetToFatTable + (count * fat_sectors * sector_size);

    ROOT_DIR* root = ReadFatRootDirectory(fp, offsetToRootDir, boot->fat_root_directory_entries);
    g_rootDir = root;
    if(root == NULL)
    {
        printf("Error: ReadFatRootDirectory Failed\n");
        return UNREADABLE;
    }

    g_offsetToDataClusters = offsetToRootDir + (boot->fat_root_directory_entries * sizeof(ROOT_ENTRY));

    HexDump(root, 100);

    //fclose(fp);
    return 0;
}

/******************************************************************************
**                           ReadMasterBootRecord                            **
******************************************************************************/

// Kevin- MBR
// seeks to offset of fp(our pointer)
// offset of MBR is 0
// Calculates size of buffer
// If "fread" fails, MBR won't read contents of file or it would read some contents, but not all that are required
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
    //HexDump(buffer, total_count);
    //printf("----------------------------\n");
    //int total_count2 = fread(buffer, 1, sizeof(MBR), fp);
    //HexDump(buffer, total_count2);
    //printf("%d\n", total_count2);

    if (total_count < sizeof(MBR)) //if the total count is less than the size of MBR, it wont be able to read the contents of the file
    {
        printf("The contents of the file are not able to be read\n");
        return NULL;
    }

    return (MBR*)buffer;
}

/******************************************************************************
**                             ReadFatBootSector                             **
******************************************************************************/
//Read in the file's pointer and offset value based off lba_offset
FAT_BOOT* ReadFatBootSector(FILE* fp, long int offset)
{
    //try to read fp
    int seek_rc = fseek(fp, offset, SEEK_SET);
    if (seek_rc != 0)
    {
        printf("Fat Boot Sector Failed to read");
        return NULL;
    }
    //allocate buffer for the size of FAT_BOOT struct as char*
    char* buffer = (char*)malloc(sizeof(FAT_BOOT));
    if (buffer == NULL)
    {
        printf("Unable to allocate memory for struct FAT_BOOT");
        return NULL;
    }
    int read_rc = fread(buffer, 1, sizeof(FAT_BOOT), fp);
    if (read_rc == 0)
    {
        printf("Unable to fread buffer");
        return NULL;
    }
    //return buffer wrapped ontop of FAT_BOOT struct
    return (FAT_BOOT*)buffer;
}

/******************************************************************************
**                               ReadFatTable                                **
******************************************************************************/

//Ali - ReadFatTable
//
//Seeks to specified offset of given fat file.
//Then calculates the size of needed buffer to read from the file from the previously stated offset and returns a pointer to the read buffer.
//Error checks a long the way.
FAT_TABLE* ReadFatTable(FILE* fp, long int offset, int count, int fat_sectors, int sector_size)
{
    int seek_rc = fseek(fp, offset, SEEK_SET);

    //Fat Table Seek Error
    if(seek_rc != 0)
    {
        printf("Could not find the data at given offset, %i\n", offset);
        return NULL;
    }

    //Calculating size to read
    int size = count*(fat_sectors * sector_size);
    //        count * 1 Fat Table Entry

    //Creating Buffer
    char* buffer = (char*)malloc(size);

    //Memory Allocation Error Check
    if (buffer == NULL)
    {
        printf("There is not enough memory available.\n");
        return NULL;
    }

    //Read from buffer a calculated amount from offset
    int read_rc = fread(buffer, 1, size, fp);

    //Fat Table Read Error Check
    if (read_rc < size)
    {
        printf("Incorrect fat_sectors or sector_size paramaters.\n");
        free(buffer);
        return NULL;
    }

    return (FAT_TABLE*)buffer;
}

/******************************************************************************
**                           ReadFatRootDirectory                            **
******************************************************************************/

ROOT_DIR* ReadFatRootDirectory(FILE* fp, long int offset, int count)
{
   int size = count * sizeof(ROOT_ENTRY);

   char* buffer = (char*)malloc(size);
   //malloc error check
   if (buffer == NULL)
   {
       printf("Read Root Directory ERROR! Not enough memory!");
       return NULL;
   }

   int seek_rc = fseek(fp, offset, SEEK_SET);

   //fseek error check
   if (seek_rc != 0)
   {
       printf("seek failed");
       free(buffer);
       return NULL;
   }

   //read the data from file into the buffer
   int read_rc = fread(buffer, sizeof(ROOT_ENTRY), count, fp);

   //fread error check
   if (read_rc != count)
   {
       printf("read failed");
       printf("size: %d", size);
       printf("///%d", read_rc);
       free(buffer);
       return NULL;
   }

   return (ROOT_DIR*)buffer;
}

//need to define a DIRECTORY_MASK
#define DIRECTORY_MASK 0X10

uint32_t GetFileSize(char* filename)
{
    //call ROOT_ENTRY* entry GetRootEntry function
    ROOT_ENTRY* entry = GetRootEntry(filename);
    return GetFileSizeFromEntry(entry);
}

uint32_t GetFileSizeFromEntry(ROOT_ENTRY* entry)
{
    //error checking for file
    //if file does not exist, there will be no entry available
    if (entry == NULL)
    {
        printf("File does not exist, root_entry not available\n");

        // temporary error code for entry
        return 0;
    }

    //DIRECTORY MASK 0X10
    //Determines whether its a file or a directory
    if(entry-> file_attribute & DIRECTORY_MASK == DIRECTORY_MASK)
    {
        return GetDirectorySizeFromEntry(entry);
    }
    else
    {
        //returns entry file size
        return entry->file_size;
    }
}

/*Yunhu*/
int GetDirectorySize(char* directory)
{
    ROOT_ENTRY* entry = GetRootEntry(directory);
    return GetDirectorySizeFromEntry(entry);
}

int GetDirectorySizeFromEntry(ROOT_ENTRY* entry)
{
    int count = 0;
    int cluster = entry->first_cluster;

    FAT_TABLE_ENTRY* base = (FAT_TABLE_ENTRY*)g_fatTable;
    FAT_TABLE_ENTRY* next = &base[cluster];

    int count = 0;
    int cluster = entry->first_cluster;

    FAT_TABLE_ENTRY* base = (FAT_TABLE_ENTRY*)g_fatTable;//store fat table
    FAT_TABLE_ENTRY* next = &base[cluster];//store next cluster

    //loop entill EOF
    while (cluster < 0xFFF8)
    {
        count++;
        cluster = *next;
        next = &base[cluster];
    }

    return count * 512;
}

/*Luke & prof.Tallman*/
ROOT_ENTRY* GetRootEntry(char* fullDirectory)
{
    //1.parse the full directory. e.g. /user/Yunhu/filename.txt  ==>  'user' 'Yunhu' 'filename.txt'
    //2.find match to the first directory entry in ROOT_DIR. e.g. 'user'
    //3.read ROOT_ENTRY to find first cluster of first directory entry
    //4.follow up until reaching EOF
    //5.read correspond cluster in data region
    //6.find match to the second directory entry in ROOT_DIR. e.g. 'Yunhu'
    //7.repeat step 3-5. Until we get to the last one
    //8.return that ROOT_ENTRY

    int entries = g_fatBoot->fat_root_directory_entries;

    ROOT_DIR* dir = g_rootDir;

    ROOT_ENTRY* entry = NULL;

    char str[strlen(fullDirectory)];
    strcpy(str, fullDirectory);
    const char delimiter[] = "/";
    char* ptr = strtok(str, delimiter);

    //uint32_t size;

    void* data;
    uint32_t size;

    while(ptr != NULL)
	{
		printf("DIR: %s\n", ptr);
        printf("%d\n", dir);
        printf("%d\n", entries);

        entry = FindMatchingEntryName(ptr, dir, entries);
        //entry = Function0(dir, entries, ptr);
        if (entry == NULL)
        {
            printf("Entry: NULL\n");
            return NULL;
        }

        // Print Entry
        printf("ENTRY: %p\n", entry);
        HexDump(entry, sizeof(ROOT_ENTRY));

        if((entry->file_attribute & 0x10) > 0) // If is Directory
        {
            //HexDump(entry, sizeof(ROOT_ENTRY));

            //int size = GetDirectorySizeFromEntry(entry); // <--- Problem
            //int rc = Function5(entry, &size);
            //printf("%d\n", size);

            //char* buffer = (char*)malloc(size);
            //memset(buffer, 0, size);
            //buffer = ReadFileContents(entry, buffer, size);

            int rc = Function4(entry, &data, &size);

            printf("%d\n", size);
            HexDump(data, size);
            
            if (rc != EXIT_SUCCESS)
            {
                printf("Error: Cannot read directory '%s'\n", ptr);
                return NULL;
            }

            dir = (ROOT_DIR*)data;
            entries = size / sizeof(ROOT_ENTRY);

            ///------------------
        }
        else
        {
            return entry; // Return File
        }

        ptr = strtok(NULL, delimiter);
	}
    return entry; // Return Directory
}

ROOT_ENTRY* FindMatchingEntryName(char* filename, ROOT_DIR* dir, int entries)
{
    ROOT_ENTRY* entry = dir->data;

    for(int i = 0; entry->filename[0] != '\0' && i < entries; i++)
    {
        if ((entry->file_attribute & 0x08) == 0)
        {
            const char* fullFileName = EightDotThreeString(entry->filename, entry->file_exetension);

            for(int i = 0; i < strlen(filename); i++)
                filename[i] = toupper(filename[i]);

            if(strcmp(fullFileName, filename) == 0)
                return entry;
        }
        entry++;
    }
    return NULL;
}

static void RemoveTrailingSpaces(char* fat_filename_buffer)
{
    assert(fat_filename_buffer != NULL);
    int end = strlen(fat_filename_buffer);
    assert(end <= 12);

    // Substutute NULLs for an trailing spaces
    int i = end - 1;
    while(i >= 0 && fat_filename_buffer[i] == ' ')
    {
        fat_filename_buffer[i] = '\0';
        i--;
    }
}

const char* EightDotThreeString(const uint8_t name[8], const uint8_t ext[3])
{
    static char full_filename[13] = {0};
    memset(full_filename, 0, sizeof(full_filename));
    strncat(full_filename, (char*)name, 8);
    RemoveTrailingSpaces(full_filename);

    if (ext[0] != ' ')
    {
        strcat(full_filename, ".");
        strncat(full_filename, (char*)ext, 3);
        RemoveTrailingSpaces(full_filename);
    }
    return full_filename;
}



/*Alex*/
char* GetFileData(char* targetFile)
{
    //1.get the correct root_entry. ROOT_ENTRY* entry = GetRootEntry(targetFile);
    ROOT_ENTRY* entry = GetRootEntry(targetFile);
    if (entry == NULL)
    {
        return NULL;
    }
    //2.get file size or directory size using GetFileSize()/GetDirectorySize()
    int fileSize = GetFileSize(targetFile);
    //3.malloc the buffer with file size/directory size
    char* buffer = (char*)malloc(fileSize); 
    //4.read the data into the buffer using ReadFileContents()
    buffer = ReadFileContents(entry, buffer, fileSize); 
    
    HexDump(buffer, fileSize);
    //5.return the buffer
    return buffer;
}

//Ali - ReadFileContents
//
//Searches through FAT to find pointers to the file's data and seeks to it to then reads all of the data into a buffer which gets returned.
char* ReadFileContents(ROOT_ENTRY* entry, char* buffer, int size)
{
    // How many bytes are left
    int remaining = size;
    // Buffer Curr
    void* buffer_pointer = buffer;
    // Amount of bytes read from buffer
    int read_c;
    // Amount of bytes seeked into buffer
    int seek_rc;

    //1. Read ROOT_ENTRY to find first cluster.

    // Cluster Curr
    FAT_TABLE_ENTRY clusterPointer = entry->first_cluster;
    // Offset from data to cluster
    long int cluster_offset = g_offsetToDataClusters + (512 * (clusterPointer - 2));

    //2. Follow up until reaching EOF
    while (clusterPointer < 0xFFF0 && remaining > 0)
    {

        //3. Seek and read correspond cluster in data region
        seek_rc = fseek(g_filePointer, cluster_offset, SEEK_SET);

        // Error Checking

        // If fseek returns 0, it is then successful. If it returns a nonzero, it has failed.
        if (seek_rc != 0){
            printf("fseek failed, did not reach correct location.\n");
            return NULL;
        }

        if (remaining>512){
            // Reading 512 bytes into Buffer
            read_c = fread(buffer_pointer, 1, 512, g_filePointer);
        }
        else
        {
            // Reading Last bit of Buffer
            read_c = fread(buffer_pointer, 1, remaining, g_filePointer);
        }
        // If fread returns 0 then nothing has been read.
        if (read_c == 0)
        {
            printf("Unable to fread into buffer");
            return NULL;
        }

        //Updating pointers/counters
        buffer_pointer+=512;
        remaining-=512;

        //4. Get next cluster from FAT
        clusterPointer = g_fatTable->Table[clusterPointer];
    }

    //5. Return the buffer
    return buffer;
}