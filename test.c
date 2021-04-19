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

	//printf("\n");
	int result = ReadDiskImage(filename);
	//printf("%d\n", result);

    ROOT_ENTRY* test = GetRootEntry("/0-DIR-01");
    //printf("%p\n", test);

    printf("\n\n-----------------------------------------------directory--------------------------------------------------\n\n");
    int dirsize = GetDirectorySize("/1-DIR-01");
    printf("dirsize: %d\n", dirsize);

    void* buffer = malloc(1000000);
    buffer = ReadFileContents(test, buffer, 1000000);


    HexDump(buffer, sizeof(buffer));



    return 0;
}
