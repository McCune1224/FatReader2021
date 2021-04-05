#include <stdio.h>
#include <stdlib.h>
#include "reader.h"
#include "helper.h"
#include "linked_list.h"

/******************************************************************************
**                              ReadDiskImage                                **
******************************************************************************/

int ReadDiskImage(char* filename)
{
    // Open disk image ------------------------------------------------------------------------
    FILE* fp = fopen(filename, "rb");
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
    if(fat == NULL) 
    {
        printf("Error: ReadFatTable Failed\n");
        return 1;
    }
    printf("data: %08x\n", *(unsigned int*)fat);


    // Root Directory -------------------------------------------------------------------------
    int offsetToRootDir = offsetToFatTable + (count * fat_sectors * sector_size);

    ROOT_DIR* root = ReadFatRootDirectory(fp, offsetToRootDir, count);
    if(root == NULL)
    {
        printf("Error: ReadFatRootDirectory Failed\n");
        return 1;
    }

    HexDump(root, 100);

    fclose(fp);
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

/*Kevin*/
//1.get the correct root_entry.
//2.return file size from that entry.

//need to define a DIRECTORY_MASK
#define DIRECTORY_MASK 0X10

//uses 32 bits (4 bytes)
uint32_t GetFileSize(char* filename)
{   
    //call ROOT_ENTRY* entry GetRootEntry function
    ROOT_ENTRY* entry = GetRootEntry(filename);

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
    //1.get the correct root_entry.
    //2.read FAT_TABLE to follow up untill reaching EOF
    //3.return directory size (number of clusters) * (size of 1 cluster which is 512) 
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
    //2.get file size or directory size using GetFileSize()/GetDirectorySize()
    //3.malloc the buffer with file size/directory size
    //4.read the data into the buffer using ReadFileContents()
    //5.return the buffer
}

/*Ali*/
char* ReadFileContents(ROOT_ENTRY* entry, char* buffer,int size)
{
    //"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\gcc" helper.c reader.c linked_list.c -o test.exe

    //1.read ROOT_ENTRY to find first cluster
    int clusterPointers[0];
    clusterPointers[0] = entry->first_cluster;
    printf("%i", clusterPointers[0]);
    //2.follow up until reaching EOF
    for (int i = 1; FAT_TABLE[clusterPointers[i]] == 00; i++){
        clusterPointers[i] = FAT_TABLE[clusterPointers[i-1]];
    }
    //3.read correspond cluster in data region
    char* clusters = (char*)malloc(size);

    for(int i = 0; i < sizeof(clusterPointers); i++;){
        clusters += data[clusterPointers[i]];
    }
    //4.load the data from clusters into the buffer
    buffer += clusters;
    //5.return the buffer
    return buffer;
}

