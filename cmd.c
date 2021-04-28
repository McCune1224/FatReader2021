#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "reader.h"
#include "helper.h"
#include "cmd.h"
#include <stdio.h>


static char dirListBuffer[256];

//Example case of fgets:

// int main () {
//    FILE *fp;
//    char str[60];

//    /* opening file for reading */
//    fp = fopen("file.txt" , "r");
//    if(fp == NULL) {
//       perror("Error opening file");
//       return(-1);
//    }
//    if( fgets (str, 60, fp)!=NULL ) {
//       /* writing content to stdout */
//       puts(str);
//    }
//    fclose(fp);
   
//    return(0);
// }



//Kevin
void ls()
{

}
//Luke
void pwd()
{

}
//Ali
void cat(char* file_path)
{
    char* filename = malloc(256);
    //Local Directory
    if(file_path[0] != '/')
    {
        strcpy(filename, dirListBuffer);
        strcat(filename, '/');
        strcat(filename, file_path);
    }
    //Located Elsewhere
    else
    {
        filename = file_path;
    }
    
    int size = GetFileSize(filename);
    char* buffer = GetFileData(filename);

    HexDump(filename, size);

    free(filename);
    free(buffer);
    
}
//Alex
void cd(char* path)
{

}
//Yunhu
void shellLoop()
{

}