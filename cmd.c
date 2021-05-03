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
//#include "reader.c"


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
  strcpy(dirListBuffer, "/2-DIR-01");
  //create an int called path_length
  //the path_length of the directory == the string length of dirListBuffer
  int path_length =  strlen(dirListBuffer);

  //make a pointer entryToList and equal it to NULL
  ROOT_ENTRY* entryToList = NULL;

  //if the path_length == 0
  //root of the drive
  if(path_length == 0)
  {
      entryToList = g_rootDir -> data;
  }
  else
  {
      entryToList = GetRootEntry(dirListBuffer);
  }


  //step through root dir one entry at a time

  uint32_t size = GetFileSizeFromEntry(entryToList);
  printf("%d\n", size);
  char* buffer = malloc(size);
  buffer = ReadFileContents(entryToList, buffer,size);

  HexDump(buffer, size);


  ROOT_ENTRY* curr = (ROOT_ENTRY*) buffer;

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




      //print the filename and file size
      // when it is a directory, the size is not stated as it is not a file
      //printf("%s %d\n", filename, filesize);


      //move on to next entry
      curr++;


  }



}
//Luke
void pwd()
{

}
//Ali
void cat(char* file_path)
{
    printf("%s\n", file_path);
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
    //Allocate a char* for the path the user might give (at most 256 chars)
    char buffer[256];
    //Go up Directory
    if (path == "..")
    {
        printf("Trying %s", path);
        printf("PRE-Updated full dir: %s\n",dirListBuffer);
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
        }
        printf("Updated full dir: %s\n",dirListBuffer);
    }
    //Going down a certian amount of directories
    else{
        printf("Trying %s\n", path);
        //check to make sure the directory path exists before appending to buffer by calling GetDirSize
        if (GetDirectorySize(strcpy(dirListBuffer,path))){
        strcpy(dirListBuffer, path);
        }
        else{
            printf("'%s' is not a valid directory path\n", path);
        }
    }
}
//Yunhu
void shellLoop()
{
    printf("slow testing session\n");
    while (1)
    {
        char cmd[60];
        const char s[2] = " ";
        char* token;

        if (fgets(cmd, 60, stdin) != NULL) // read command
        {    
            //printf("%s\n" , cmd);
            //lowercase the command
            for (int i = 0; cmd[i]; i++)
            {
                //cmd[i] = toupper(cmd[i]);
            }


            token = strtok(cmd, s);// split by space and get first token
            int index = strcspn(token, "\r");
            token[index] = '\0';
            printf("%s\n", token);
            if (strcmp(token, "ls") == 0)
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
                index = strcspn(token, "\r\n");
                token[index] = '\0';
                printf("'%s'\n", token);
                cat(token);
            }

            else if (strcmp(token, "cd") == 0)
            {
                token = strtok(NULL, s);
                cd(token);

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
