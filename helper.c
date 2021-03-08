#include <helper.h>

// Luke
// Turns input parameter of "1 byte" into a more human friendly readable string
char* FileAttributes(uint8_t)
{

}

// Yunhu
// Turns input parameter "int bytes" into a more human friendly readable string.
char* AsHumanSize(int bytes)
{

}

// Yunhu
// Read input parameter of 1 byte and returns the media type. Examples: HDD, Floppy, Disc, etc.
char* MediaType(uint8_t)
{

}

// Kevin
// Reads input parameter of 1 byte and responds with telling us the File System. For example, reading the byte will tell us if the file system is FAT16, FAT32, NTFS, etc.
char* PartitionTypeName(uint8_t)
{

}

// Ali
// Reads data at memory address (void*) till the end of given size, then dumps the hex values. Here is an example output:
void HexDump(void*, int size)
{

}


//FixShortFile tests
char* filename1 = "filenametxtAAAAAA"; // "filename.txt"
char* filename2 = "file    docBBBBBB"; // "file.doc"
char* filename3 = "my_dir     CCCCCC"; // "my_dir"

// Alex
// Fixes up the name of data in our Fat Table. Note that this is only designed for short file. 
// Short files can only contain a file name with a max size of 8, and a extension name with a max size of 3.
void FixShortFile(void *rawFilename)
{
    char *pRawFileName = rawFilename;
    char fullFileName[12] = {'\0'};

    memcpy(fullFileName, pRawFileName, 12);
    printf("Modifying: |%s|\n", fullFileName);


    fullFileName[11] = '\0';
    printf("Result: |%s|\n",fullFileName);
    if (fullFileName[8] == ' ')
    {
        printf("Is a Directory.");
    }
    else
    {
        printf("Is a .%c%c%c File", fullFileName[8],fullFileName[9],fullFileName[10]);
    }
    int count = 0;
}


