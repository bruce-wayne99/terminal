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
#include "redirection.h"
#include "nightswatch.h"
#include "pinfo.h"
#include "pwd.h"
#include "global.h"
#include "global.c"
#include "env.h"

void ExecuteEnv(char *token, const char delemiter[] ,int flag)
{
    char *argv[100];
    int k;
    int top = 0;
    token = strtok(NULL, delemiter);
    while(token != NULL)
    {
      argv[top++] = token;
      token = strtok(NULL, delemiter);
    }
    if(flag == 1 && top >= 1 && top <= 2)
    {
      if(argv[1] != NULL)
      {
        k = setenv(argv[0],argv[1],1);
        if(k < 0)
        {
          perror("unable to set the variable");
        }
      }
      else
      {
        k = setenv(argv[0],"",1);
        if(k < 0)
        {
          perror("unable to set the variable");
        }
      }
    }
    else if(top >= 1 && flag == 0)
    {
      k = unsetenv(argv[0]);
      if(k < 0)
      {
        perror("unable to unset the variable");
      }
    }
    else
    {
      fprintf(stderr,"Wrong usage of command\n");
    }
    return;
}
