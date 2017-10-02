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

// this function executes the comman echo
void ExecuteECHO(char command[],char *token,const char delemiter[])
{
    int open = 0;
    int i;
    i=5;
    while(command[i] == ' ' && i < command_len)
    {
      i++;
    }
    for(;i<command_len;)
    {
      if(command[i] == '"')
      {
        while(command[i] == '"' && i<command_len)
        {
          i++;
        }
        while(command[i] != '"' && i<command_len)
        {
          printf("%c",command[i]);
          i++;
        }
        while(command[i] == '"' && i<command_len)
        {
          i++;
        }
      }
      else
      {
          if(command[i] == ' ' && i < command_len)
          {
            printf(" ");
            while(command[i] == ' ' && i<command_len)
            {
              i++;
            }
          }
          while(command[i] != '"' && command[i] != ' ' && i<command_len)
          {
            printf("%c",command[i]);
            i++;
          }
      }
    }
    printf("\n");
    return;
}
