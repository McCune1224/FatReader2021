#ifndef HELPER_HEADER
#define HELPER_HEADER
#include <stdint.h>

char* FileAttributes(uint8_t);
char* AsHumanSize(uint64_t bytes);
char* MediaType(uint8_t);
char* PartitionTypeName(uint8_t);
void HexDump(void*, int size);
void FixShortFile(void*);

#endif