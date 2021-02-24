#include <stdint.h>
#include "helper.h"
#include <string.h>

// 11101011
// eb
uint8_t test = 235;

int main(int argc, char *argv[])
{
    char* attributes = FileAttributes(test);
    printf("%s\n", attributes);
    free(attributes);
}