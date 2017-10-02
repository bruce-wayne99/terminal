#include<stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include<unistd.h>
#include<string.h>
#include <dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<time.h>
#include<grp.h>
#include <sys/wait.h>
#include<fcntl.h>
#include "cd.h"
#include "command.h"
#include "display.h"
#include "echo.h"
#include "execute.h"
#include "ls.h"
#include "nightswatch.h"
#include "pinfo.h"
#include "pwd.h"
#include "global.h"
#include "global.c"

// This function stores the pid into a string
void ConvertPID(char stringpid[],int pid)
{
    int temp = pid;
    int count = 0;
    while(temp != 0)
    {
      count++;
      temp = temp/10;
    }
    temp = pid;
    stringpid[count] = 0;
    while(temp != 0)
    {
      stringpid[count-1] = temp%10 + '0';
      temp = temp/10;
      count--;
    }

    return;
}

// THis function executed the function pinfo
void ExecutePINFO(char *token,const char delemiter[])
{
      token = strtok(NULL,delemiter);
      int pid;
      char stringpid[1000],c[1000000];
      if(token == NULL)
      {
        pid = getpid();
        ConvertPID(stringpid,pid);
      }
      else
      {
        strcpy(stringpid,token);
      }
      int source,link;
      char s[1000];
      char p[1000];
      strcpy(s,"/proc/");
      strcpy(p,"/proc/");
      char ExecutePath[1000];
      strcat(s,stringpid);
      strcat(s,"/stat");
      strcat(p,stringpid);
      strcat(p,"/exe");
      source  = open(s,O_RDONLY);
      link = readlink(p,ExecutePath,1000);
      if(source < 0 || link < 0)
      {
        fprintf(stderr,"Invalid process id \n");
        close(source);
        close(link);
        return;
      }
      ExecutePath[link] = 0;
      int byte = read(source,c,1000000);
      int count = 1;
      char Id[100],Status[100],VMemory[100],*content;
      content = strtok(c,delemiter);
      while(content != NULL)
      {
        if(count == 1)
        {
          strcpy(Id,content);
        }
        if(count == 3)
        {
          strcpy(Status,content);
        }
        if(count == 25)
        {
          strcpy(VMemory,content);
        }
        count++;
        content = strtok(NULL,delemiter);
      }
      printf("pid -- %s\n",Id);
      printf("Process Status -- %s\n",Status);
      printf("Virtual Memory -- %s\n",VMemory);
      printf("Executable Path -- %s\n",ExecutePath);
      close(source);
      close(link);
      return;
}
