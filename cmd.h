#ifndef CMD_HEADER
#define CMD_HEADER
#include <stdint.h>


void ls();
void pwd();
void cat(char* path);
void cd(char* path);
void shellLoop();

#endif