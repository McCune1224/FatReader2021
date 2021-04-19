#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "reader.h"
#include "helper.h"


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
ROOT_ENTRY* Function0(ROOT_DIR* subroot, int num_entries, char* filename);
const char* Function1(const uint8_t name[8], const uint8_t ext[3]);
static void Function2(char* fat_filename_buffer);
const char* Function3(const char* path);
int Function4(ROOT_ENTRY* entry, void** data, uint32_t* size);
int Function5(ROOT_ENTRY* entry, uint32_t* entry_size);
static int Function6(int start_cluster);

/******************************************************************************
**                              ReadDiskImage                                **
******************************************************************************/

int ReadDiskImage(char* filename)
{
    // Open disk image ------------------------------------------------------------------------
    FILE* fp = fopen(filename, "rb");
    g_filePointer = fp;
    if (fp == NULL)
    {
        printf("Error: Could not open binary disk image: '%s'\n", filename);
        return 1;
    }

    // Master Boot Record ---------------------------------------------------------------------
    MBR* mbr = ReadMasterBootRecord(fp, 0);
    if(mbr == NULL)
    {
        printf("Error: ReadMasterBootRecord Failed\n");
        return 1;
    }
    HexDump(mbr, 512);

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
        return 1;
    }

    FAT_BOOT* boot = ReadFatBootSector(fp, offsetToBootSector);
    g_fatBoot = boot;
    HexDump(boot, 512);

    // Fat Table ------------------------------------------------------------------------------

    // Get required parameters for ReadFatTable using the FAT_BOOT structure
    int count = boot->number_of_file_allocation_table;
    int fat_sectors = boot->logical_sectors_per_alloc_table;
    int sector_size = boot->bytes_per_sector;
    int offsetToFatTable = offsetToBootSector + (boot->reserved_logical_sectors * sector_size);

    // Print debug data (TEMP, TO BE REMOVED IN FUTURE)
    printf("%d\n", count);
    printf("%d\n", fat_sectors);
    printf("%d\n", sector_size);
    printf("%d\n", offsetToFatTable);

    FAT_TABLE* fat = ReadFatTable(fp, offsetToFatTable, count, fat_sectors, sector_size);
    g_fatTable = fat;
    if(fat == NULL)
    {
        printf("Error: ReadFatTable Failed\n");
        return 1;
    }
    printf("data: %08x\n", *(unsigned int*)fat);


    // Root Directory -------------------------------------------------------------------------
    int offsetToRootDir = offsetToFatTable + (count * fat_sectors * sector_size);

    ROOT_DIR* root = ReadFatRootDirectory(fp, offsetToRootDir, boot->fat_root_directory_entries);
    g_rootDir = root;
    if(root == NULL)
    {
        printf("Error: ReadFatRootDirectory Failed\n");
        return 1;
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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

/*Kevin*/
//1.get the correct root_entry.
//2.return file size from that entry.

//need to define a DIRECTORY_MASK
#define DIRECTORY_MASK 0X10

//uses 32 bits (4 bytes)
uint32_t GetFileSize(char* filename)
{
    //call ROOT_ENTRY* entry GetRootEntry function
    ROOT_ENTRY* entry = GetDirEntry(filename);

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
        return GetDirectorySize(filename);
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
    ROOT_ENTRY* entry = GetDirEntry(directory);

    int count = 0;
    int cluster = entry->first_cluster;

    FAT_TABLE_ENTRY* base = (FAT_TABLE_ENTRY*)g_fatTable;
    FAT_TABLE_ENTRY* next = &base[cluster];

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
}

/*Alex*/
char* GetFileData(char* targetFile)
{
    //1.get the correct root_entry. ROOT_ENTRY* entry = GetRootEntry(targetFile);
    ROOT_ENTRY* entry = GetRootEntry(targetFile);
    //2.get file size or directory size using GetFileSize()/GetDirectorySize()
    int fileSize = GetFileSize(targetFile);
    //3.malloc the buffer with file size/directory size
    char* buffer = (char*)malloc(fileSize); 
    //4.read the data into the buffer using ReadFileContents()
    buffer = ReadFileContents(entry, buffer, fileSize); 
    //5.return the buffer
    return buffer;
}

//Ali - ReadFileContents
//
//Searches through FAT to find pointers to the file's data and seeks to it to then reads all of the data into a buffer which gets returned.
char* ReadFileContents(ROOT_ENTRY* entry, char* buffer,int size)
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
    while (clusterPointer < 0xFFF0 && remaining > 0){
        
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



ROOT_ENTRY* GetDirEntry(char* filename)
{
    assert(filename != NULL);

    ROOT_DIR* root = g_rootDir;
    int num_entries = g_fatBoot->fat_root_directory_entries;

    ROOT_ENTRY* entry = NULL;
    void* data;
    uint32_t size;

    // filename has a series of n tokens separated by slashes
    // the very last of these tokens might be a filename or a directory
    // all of the other tokens from 1 through n-1 must be Directories

    const char* sep = Function3(filename);
    char* token = strtok(filename, sep);
    while(token != NULL)
    {
        printf("Token=%s\n", token);
        entry = Function0(root, num_entries, token);
        if (entry == NULL)
        {
            return NULL;
        }

        printf("Entry=%p\n", entry);
        if ((entry->file_attribute & FILE_ATTRIBUTE_DIRECTORY) > 0)
        {
            int rc = Function4(entry, &data, &size);
            if (rc != EXIT_SUCCESS)
            {
                printf("Error: Cannot read directory '%s'\n", token);
                return NULL;
            }
            root = (ROOT_DIR*)data;
            num_entries = size / sizeof(ROOT_ENTRY);
        }
        else
        {
            // if we get here, we've reached the final ROOT_ENTRY for this
            // file and this is what we need to return
            return entry;
        }

        token = strtok(NULL, sep);
    }

    // if we get here, we've exhausted all the directories that the user gave
    // us and are ready to return the final ROOT_ENTRY*
    return entry;
}


ROOT_ENTRY* Function0(ROOT_DIR* subroot, int num_entries, char* filename)
{
    assert(subroot != NULL);
    assert(num_entries != 0);
    assert(filename != NULL);

    // Search through every entry in this directory for a matching filename
    ROOT_ENTRY* dir = subroot->data;
    for(int i = 0; i < num_entries && dir->filename[0] != '\0'; i++)
    {
        HexDump(dir, sizeof(ROOT_ENTRY));
        // Ignore Long File Names and Volumes... LFNs include the Volume Bit
        if ((dir->file_attribute & FILE_ATTRIBUTE_VOLUME) == 0)
        {
            const char* test = Function1(dir->filename, dir->file_exetension);

            // FAT filenames are always uppercase on disk
            for(int i = 0; i < strlen(filename); i++)
            {
                filename[i] = toupper(filename[i]);
            }

            printf("%s ? %s\n", test, filename);
            if (strcmp(test, filename) == 0)
            {
                return dir;
            }
        }
        dir++;
    }

    return NULL;
}


const char* Function1(const uint8_t name[8], const uint8_t ext[3])
{
    static char full_filename[13] = {0};
    memset(full_filename, 0, sizeof(full_filename));
    strncat(full_filename, (char*)name, 8);
    Function2(full_filename);

    if (ext[0] != ' ')
    {
        strcat(full_filename, ".");
        strncat(full_filename, (char*)ext, 3);
        Function2(full_filename);
    }
    return full_filename;
}


static void Function2(char* fat_filename_buffer)
{
    assert(fat_filename_buffer != NULL);
    int end = strlen(fat_filename_buffer);
    assert(end <= 12);

    // Substitute NULLs for any trailing spaces
    int i = end - 1;
    while(i >= 0 && fat_filename_buffer[i] == ' ')
    {
        fat_filename_buffer[i] = '\0';
        i--;
    }
}


const char* Function3(const char* path)
{
    if (strchr(path, '\\') != NULL)
    {
        return "\\";
    }
    else if (strchr(path, '/') != NULL)
    {
       return "/";
    }
    else
    {
        return "\\";
    }
}


int Function4(ROOT_ENTRY* entry, void** data, uint32_t* size)
{
    assert(entry != NULL);
    assert(data != NULL);
    assert(size != NULL);

    uint32_t allocation_size = 0;
    if (Function5(entry, &allocation_size) != EXIT_SUCCESS)
    {
        printf("Error: Cannot determine size directory entry\n");
        return EXIT_FAILURE;
    }

    void* buffer = malloc(allocation_size);
    assert(buffer != NULL);
    memset(buffer, 0, allocation_size);

    long int data_base = g_offsetToDataClusters;
    int cluster_size   = g_fatBoot->sectors_per_cluster *
                         g_fatBoot->bytes_per_sector;

    uint32_t read_remaining = allocation_size;
    void*    read_ptr = buffer;
    int      read_cnt;
    if (read_remaining > cluster_size)
    {
        read_cnt = cluster_size;
    }
    else
    {
        read_cnt = read_remaining;
    }

    uint16_t cluster = entry->first_cluster;
    FAT_TABLE_ENTRY* fat_base = (FAT_TABLE_ENTRY*)g_fatTable;
    FAT_TABLE_ENTRY* fat_next = &fat_base[cluster];
    while(cluster < 0xFFF0 && read_remaining > 0)
    {
        long int seek_offset = data_base + (cluster - 2) * cluster_size;
        int rc = fseek(g_filePointer, seek_offset, SEEK_SET);
        if (rc != 0)
        {
            char* msg = "Error: Cannot seek to cluster %d at offset %ld\n";
            printf(msg, cluster, seek_offset);
            free(buffer);
            return EXIT_FAILURE;
        }

        if (read_remaining > cluster_size)
        {
            read_cnt = cluster_size;
        }
        else
        {
            read_cnt = read_remaining;
        }

        int count = fread(read_ptr, sizeof(char), read_cnt, g_filePointer);
        if (count != read_cnt)
        {
            char* msg = "Error: Expected %d for Cluster but got %d\n";
            printf(msg, read_cnt, count);
            free(buffer);
            return EXIT_FAILURE;
        }

        read_ptr += count;
        read_remaining -= count;
        cluster = *fat_next;
        fat_next = &fat_base[cluster];
    }

    *data = buffer;
    *size = allocation_size;
    return EXIT_SUCCESS;
}


int Function5(ROOT_ENTRY* entry, uint32_t* entry_size)
{
    assert(entry != NULL);
    assert(entry_size != NULL);

    if ((entry->file_attribute & FILE_ATTRIBUTE_DIRECTORY) == 0)
    {
        *entry_size = entry->file_size;
        return EXIT_SUCCESS;
    }
    else
    {
        int cluster_count = Function6(entry->first_cluster);
        int cluster_size  = g_fatBoot->sectors_per_cluster;
        int sector_size   = g_fatBoot->bytes_per_sector;
        *entry_size       = cluster_count * cluster_size * sector_size;
        return EXIT_SUCCESS;
    }
}


static int Function6(int start_cluster)
{
    assert(g_fatTable != NULL);

    int count  = 0;
    int cluster = start_cluster;

    FAT_TABLE_ENTRY* fat_base = (FAT_TABLE_ENTRY*)g_fatTable;
    FAT_TABLE_ENTRY* fat_next = &fat_base[cluster];
    while(cluster < 0xFFF0)
    {
        count++;
        cluster  = *fat_next;
        fat_next = &fat_base[cluster];
    }

    return count;
}
