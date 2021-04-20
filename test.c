#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "helper.h"
#include "reader.h"

uint8_t attributeNum = 235; // 11101011 eb

uint8_t partitionNum = 1;

int main(int argc, char* argv[])
{
   //.dd file to read in
    char* filename = "dfr-16-fat.dd";

	printf("\n");
	int result = ReadDiskImage(filename);
	printf("%d\n", result);

    char* fullDirectory = (char*)malloc(256);
    strcpy(fullDirectory, "/2-DIR-01/2-01-0~1.txt");
    ROOT_ENTRY* entry = GetDirEntry(fullDirectory);
    printf("\nT\n\n");
    //const char* fullFileName = EightDotThreeString(entry->filename, entry->file_exetension);
    printf("Entry Found: %s\n", entry->filename);
    HexDump(entry, sizeof(ROOT_ENTRY));

    return 0;
}
