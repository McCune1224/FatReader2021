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
void hexDump(void *addr, int size)
{
    int i;
    char temp[17];
    unsigned char *x = (unsigned char*)addr;

    

    // Process every byte in the data.
    for (i = 0; i < size; i++) 
    {
       
        // Multiple of 8 means new line (with line offset).
        if ((i % 8) == 0) 
        {
            // Padding for proper formatting
            if (i != 0){
                printf("  %s\n", temp);}
               
            // Address offset on the left side
            printf("  %08x ", i);
        }

        // Now the hex code for the specific character.
        printf(" %02x", x[i]);

        // If current character is not ASCII then store it as '?'
        //  from  (space) --------->  (~) in ASCII 
        if ((x[i] < 0x20) || (x[i] > 0x7e)) 
        {
            temp[i % 8] = '.';
        } 
        else 
        {
            temp[i % 8] = x[i];
        }
        // Add a Null terminator to the end of the line so it can be printed
        temp[(i % 8) + 1] = '\0';
    }

    // Pad out last line if not exactly 8 characters.
    while ((i % 8) != 0) {
        printf("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf("  %s\n", temp);
}

// Alex
// Fixes up the name of data in our Fat Table. Note that this is only designed for short file. 
// Short files can only contain a file name with a max size of 8, and a extension name with a max size of 3.
void FixShortFile(void*)
{

}