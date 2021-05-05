#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "reader.h"
#include "helper.h"
#include "cmd.h"
#include <stdio.h>


static char dirListBuffer[256];


//Kevin
void ls()
{
    //strcpy(dirListBuffer, "/2-DIR-01");

    //make a pointer entryToList and equal it to NULL
    ROOT_DIR* dirToList = NULL;
    uint32_t size = 0;

    //root of the drive
    if(strcmp(dirListBuffer, "/") == 0)
    {
        dirToList = g_rootDir;
        size = g_fatBoot -> fat_root_directory_entries * sizeof(ROOT_ENTRY);
    }
    else
    {
        ROOT_ENTRY* entryToList = NULL;
        entryToList = GetRootEntry(dirListBuffer);
        size = GetFileSizeFromEntry(entryToList);

        dirToList = malloc(size);
        ReadFileContents(entryToList, (char*)dirToList, size);
    }

    //step through root dir one entry at a time
    ROOT_ENTRY* curr = (ROOT_ENTRY*) dirToList;
    while(curr->filename[0] != '\0')
    {
      const char* filename = EightDotThreeString(curr -> filename, curr -> file_exetension);
      uint32_t filesize = curr->file_size;

      if((curr-> file_attribute & FILE_ATTRIBUTE_DIRECTORY ) == FILE_ATTRIBUTE_DIRECTORY  )
      {
        //prints out DIR
         printf("%-12s DIR\n", filename);
      }

      else if((curr-> file_attribute & FILE_ATTRIBUTE_LFN ) == FILE_ATTRIBUTE_LFN )
      {
          //printf("longfilename\n");
      }
      else
      {
          //Prints out filename
          printf("%-12s %d\n", filename, filesize);

      }

      //move on to next entry
      curr++;
    }
}
//Luke
void pwd()
{
    printf("%s\n", dirListBuffer);
}
//Ali
void cat(char* file_path)
{
    char* filename = malloc(256);
    //Local Directory
    if(file_path[0] != '/')
    {   //The last paramater is not the size of the source.

        strncpy(filename, dirListBuffer, 256);
        strncat(filename, "/", 256-strlen(filename)-1);
        strncat(filename, file_path, 256-strlen(filename)-1);
    }
    //Located Elsewhere
    else
    {
        strncpy(filename, file_path, 256);
    }
    int size = GetFileSize(filename);
    if(size == 0)
    {
        return;
    }
    char* buffer = GetFileData(filename);

    //Cat the file
    HexDump(buffer, size);

    //Maintenance
    free(filename);
    free(buffer);

}
//Alex
void cd(char *path)
{
    if (path == NULL)
    {
        dirListBuffer[0] = '/';
        return;
    }

    RemoveTrailingSpaces(path);
    if (strlen(path) == 0)
    {
        dirListBuffer[0] = '/';
        return;
    }

    //Allocate a char* for the path the user might give (at most 256 chars)
    char buffer[256];

    //Go up Directory
    if (strcmp(path, "..") == 0)
    {
        //printf("Trying %s\n", path);
        //printf("PRE-Updated full dir: %s\n",dirListBuffer);

        /*
        //Move path into buffer so strtok() can be used
        strcpy(buffer, path);

        //Variables for strtok(), which allows parse based off a string/char
        char *parsedDir;
        parsedDir = strtok(buffer, "/");
        //No "/" detected, single directory move
        if (parsedDir == NULL)
        {
            printf("At root, cannot go up a directory...\n");
        }
        //"/" detected, need to move multiple directories
        while (parsedDir)
        {
            //move until we get the dir after the final "/"
            parsedDir = strtok(NULL, "/");
        }

        //get the starting point from the final "/" in the dirListBuffer
        int i = strlen(dirListBuffer) - strlen(parsedDir) + 1;
        //Replace the buffer starting from the final "/" to be null padding
        while (i < strlen(dirListBuffer)){
            dirListBuffer[i] = '\0';
        }*/

        char* last_slash = NULL;
        // get rid of the trailing /
        last_slash = strrchr(dirListBuffer, '/');
        last_slash[0] = '\0';
        // get rid of the child directory
        last_slash = strrchr(dirListBuffer, '/');
        last_slash[1] = '\0';

        //printf("Updated full dir: %s\n",dirListBuffer);
    }
    //Going down a certian amount of directories
    else
    {
        strncpy(buffer, dirListBuffer, 256);
        strncat(buffer, path, 256-strlen(buffer)-1);
        //printf("Trying %s\n", buffer);

        //check to make sure the directory path exists before appending to buffer by calling GetDirSize
        if (GetRootEntry(buffer) != NULL)
        {
            strncpy(dirListBuffer, buffer, 256);
            strncat(dirListBuffer, "/", 256-strlen(buffer)-1);
        }
        else
        {
            printf("'%s' is not a valid directory path\n", path);
        }
    }
}
//Yunhu
void shellLoop()
{
    dirListBuffer[0] = '/';

    while (1)
    {
        char cmd[60];
        const char s[2] = " ";
        char* token;

        printf("> ");
        if (fgets(cmd, 60, stdin) != NULL) // read command
        {
            //lowercase the command
            for (int i = 0; cmd[i]; i++)
            {
                cmd[i] = tolower(cmd[i]);
            }

            int index = strcspn(cmd, "\r\n");
            cmd[index] = '\0';
            token = strtok(cmd, s);// split by space and get first token

            if (token == NULL)
            {
                continue;
            }
            RemoveTrailingSpaces(token);
            if (strlen(token) == 0)
            {
                continue;
            }

            else if (strcmp(token, "ls") == 0)
            {
                ls();
            }

            else if (strcmp(token, "pwd") == 0)
            {
                pwd();
            }

            else if (strcmp(token, "cat") == 0)
            {
                token = strtok(NULL, s);
                printf("'%s'\n", token);
                cat(token);
            }

            else if (strcmp(token, "cd") == 0)
            {
                token = strtok(NULL, s);
                cd(token);
            }

            else if (strcmp(token, "exit") == 0)
            {
                return;
            }

            else
            {
                printf("error: unrecognized command");
            }
        }
        else
        {
            printf("fgets failed.");
        }
    }
}
