#include <stdio.h>
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
void cat(char* path)
{

}
//Alex
void cd(char* path)
{

}
//Yunhu
void shellLoop()
{

}