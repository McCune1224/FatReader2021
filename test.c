#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "helper.h"
#include "reader.h"
#include "cmd.h"

uint8_t attributeNum = 235; // 11101011 eb
uint8_t partitionNum = 1;

int main(int argc, char* argv[])
{
    //.dd file to read in
    //char* filename = "dfr-17-fat.dd";
    char* filename = "dfr-16-fat.dd";
	int result = ReadDiskImage(filename);
    if (result == UNREADABLE)
    {
        printf("Error reading disk image %s\n", filename);
        return 0;
    }

    // Can't use string literal, must allocate a variable to hold file name   
    char fullDirectory[256] = {0};
    memset(fullDirectory, 0, sizeof(fullDirectory));
    //strcpy(fullDirectory, "/W04/W04L01/W04L02/W04F03.TXT");
    strcpy(fullDirectory, "/2-DIR-01/2-01-0~1.txt");

    //ls();
    printf("testing\n");
    cat(fullDirectory);
    shellLoop();

    /*
    // Test case for GetRootEntry
    ROOT_ENTRY* entry = GetRootEntry(fullDirectory);    
    const char* fullFileName = EightDotThreeString(entry->filename, entry->file_exetension);
    printf("Found ROOT_ENTRY* for %s: %p\n", fullFileName, entry);
    HexDump(entry, sizeof(ROOT_ENTRY));

    // Test case for GetFileSize and GetFileData function
    int size = GetFileSize(fullDirectory);
    char* data = GetFileData(fullDirectory);
    if (data == NULL)
    {
        printf("Could not read file %s\n", fullDirectory);
        return 0;
    }
    
    // Output the result of the test
    printf("FILE DUMP: %s (%d bytes)\n", fullDirectory, size);
    HexDump(data, size);
     */
    return 0;
}
