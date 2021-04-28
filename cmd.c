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
    char* filename;
    char* temp;
    //Local Directory
    if(file_path[0] != '\\')
    {
        filename = file_path;
    }
    //Located Elsewhere
    else
    {
        for(int i = strlen(file_path)-1; file_path[i] != '\\'; i--)
        {
            temp+= file_path[i];
        }
        for(int i = strlen(temp)-1; i>0; i--)
        {
            filename+= temp[i];
        }
    }
    char* buffer = GetFileData(filename);
    int size = sizeof(buffer);
    ROOT_ENTRY* entry = GetRootEntry(filename);
    temp = ReadFileContents(entry, buffer, size);
    HexDump(temp, size);
    free(filename);
    free(buffer);
    free(temp);
    
}
//Alex
void cd(char* path)
{

}
//Yunhu
void shellLoop()
{

}