#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "helper.h"

uint8_t attributeNum = 235; // 11101011 eb

uint8_t partitionNum = 1;

int main(int argc, char* argv[])
{
    char* attributes = FileAttributes(attributeNum);
    printf("%s\n", attributes);
    free(attributes);

	char* partitionType = PartitionTypeName(partitionNum);
	printf("%s\n", partitionType);

	uint64_t bytesBeforeConverted = 47281957121;
	printf("%s\n", AsHumanSize(bytesBeforeConverted));
	uint8_t byte = 0xE5;
	printf("%s\n", MediaType(byte));

	char* dumpTest = "\x00\x00Test...?..sdjfasjdfj\0asd;lasm;d\x00\x001,,0maks,";
	hexDump(dumpTest, 32);
}
