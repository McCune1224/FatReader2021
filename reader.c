#include <stdio.h>
#include <stdlib.h>
#include "reader.h"
#include "helper.h"

int ReadDiskImage(char* filename)
{
    // File Open
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("Error: Could not open binary file '%s'\n", filename);
        return 1;
    }

    // Master Boot Record
    MBR* mbr = ReadMasterBootRecord(fp, 0);
    if(mbr == NULL)
    {
        printf("ERROR: ReadMasterBootRecord Failed\n");
        //return 1;
    }
    HexDump(mbr, 512);

    // Fat Table
    int offsetToFatTable = 0x810800; // taken from hex dump (unique to this image)
    int count = 2;                   // taken from boot sector as seen in hex dump (pretty standard)
    int fat_sectors = 254;           // taken from boot sector as seen in hex dump (unique to this image)
    int sector_size = 512;           // taken from boot sector as seen in hex dump (pretty standard)

    FAT_TABLE* fat = ReadFatTable(fp, offsetToFatTable, count, fat_sectors, sector_size);
    if(fat == NULL) 
    {
        printf("ERROR: ReadFatTable Failed\n");
        //return 1;
    }
    printf("data: %08x\n", *(unsigned int*)fat);

    // Root Directory
    ROOT_DIR* root = ReadFatRootDirectory(fp, 0x850000, 100);
    if(root == NULL)
    {
        printf("ERROR: ReadFatRootDirectory Failed\n");
        //return 1;
    }
    HexDump(root, 100);

    fclose(fp);
    return 0;
}

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

FAT_BOOT* ReadFatBootSector(FILE* fp, long int offset)
{
    int seek_rc = fseek(fp, offset, SEEK_SET);
    if (seek_rc !=0)
    {
        printf("Fat Boot Sector Failed to read");
        return NULL;
    }
    return NULL;
    char* buffer = (char*)malloc(sizeof(FAT_BOOT));
    if (buffer == NULL)
    {
        printf("Unable to allocate memory for struct FAT_BOOT");
        return NULL;
    }
    int read_rc = fread(buffer, 1, sizeof(FAT_BOOT), fp);
    if (read_rc == NULL)
    {
        printf("Unable to fread buffer");
        return NULL;
    }
    return (FAT_BOOT*)buffer;
    
}

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
