#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "helper.h"


int main(int argc, char* argv[])
{
	uint64_t bytesBeforeConverted = 47281957121;
	printf("%s\n", AsHumanSize(bytesBeforeConverted));
	uint8_t byte = 0xE5;
	printf("%s", MediaType(byte));
}