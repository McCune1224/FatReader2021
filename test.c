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

//.dd file to read in and deciding whether its a file or directory
//Test case for GetFileSize
char buffer[256] = {0};
strcpy(buffer, "/2-DIR-01/2-01-0~1.txt");
int file_size = GetFileSize(buffer);
printf("%d\n", file_size);
   
     return 0;
}
