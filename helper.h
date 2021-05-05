#ifndef HELPER_HEADER
#define HELPER_HEADER
#include <stdint.h>
#include "reader.h"

char* AsHumanSize(uint64_t bytes);
char* FileAttributes(uint8_t byte);
char* MediaType(uint8_t);
char* PartitionTypeName(uint8_t);
void RemoveTrailingSpaces(char* fat_filename_buffer);
void HexDump(void*, int size);
const char* EightDotThreeString(const uint8_t name[8], const uint8_t ext[3]);
ROOT_ENTRY* FindMatchingEntryName(char* filename, ROOT_DIR* dir, int entries);

#endif
