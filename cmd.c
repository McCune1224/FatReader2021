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
    {   //The last paramater is not the size of the source.

        strncpy(filename, dirListBuffer, 256);
        strncat(filename, '/', 256-strlen(filename)-1);
        strncat(filename, file_path, 256-strlen(filename)-1);
    }
    //Located Elsewhere
    else
    {
        filename = file_path;
    }
    int size = GetFileSize(filename);
    char* buffer = GetFileData(filename);

    //Cat the file
    HexDump(buffer, size);

    //Maintenance
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
    while (1)
    {
        char cmd[60];
        const char s[2] = " ";
        char* token;

        if (fgets(cmd, 60, stdin) != NULL) // read command
        {
            //lowercase the command
            for (int i = 0; cmd[i]; i++)
            {
                cmd[i] = tolower(cmd[i]);
            }

            token = strtok(test, s);// split by space and get first token
            if (strcmp(token, "ls" == 0))
            {
                ls();
            }

            else if (strcmp(token, "pwd" == 0))
            {
                pwd();
            }

            else if (strcmp(token, "cat" == 0))
            {
                token = strtok(NULL, s);
                cat(token);
            }

            else if (strcmp(token, "cd" == 0))
            {
                token = strtok(NULL, s);
                cd(token);
            }

            else
            {
                printf("error: unrecognized command")
            }
        }

        else
        {
            printf("fgets failed.");
        }
    }
}