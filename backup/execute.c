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

int ExecuteOtherCommands(char *token,const char delemiter[])
{
      char *argv[10000];
      int top = 0;
      int flag = 0;
      int pid;
      int pid1,val;
      int k;
      while(token != NULL)
      {
        argv[top++] = token;
        token = strtok(NULL,delemiter);
        if(token == NULL && strcmp(argv[top-1],"&") == 0)
        {
          flag = 1;
        }
      }
      argv[top] = NULL;
      if(flag == 0)
      {
          pid = fork();
          if(pid == 0)
          {
              k = execvp(argv[0],argv);
              if(k<0)
              {
                 fprintf(stderr,"%s: command not found\n",argv[0]);
              }
            return 0;
          }
          else
          {
            wait(NULL);
            return 0;
          }
      }
      else
      {
        argv[top-1] = NULL;
        pid = fork();
        if(pid == 0)
        {
          if(strcmp(argv[0],"cd") == 0)
          {
            usecommand[strchr(usecommand,'&')-usecommand] = ' ';
            token = strtok(usecommand,delemiter);
            ExecuteCD(token,delemiter);
          }
          else
          {
            k = execvp(argv[0],argv);
            if(k<0)
            {
               fprintf(stderr,"%s: command not found\n",argv[0]);
            }
          }
          return 0;
        }
        else
        {
            printf("%d\n",pid);
            return 0;
        }
      }
}
