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

void ExecuteCommand(char command[],char *token,const char delemiter[])
{
      if(strchr(command,'&') != NULL)
      {
        ExecuteOtherCommands(token,delemiter);
      }
      else if(strcmp(token,"cd") == 0)
      {
        ExecuteCD(token,delemiter);
      }
      else if(strcmp(token,"pwd") == 0)
      {
        ExecutePWD();
      }
      else if(strcmp(token,"echo") == 0)
      {
        ExecuteECHO(command,token,delemiter);
      }
      else if(strcmp(token,"ls") == 0)
      {
        ExecuteLS(token,delemiter);
      }
      else if(strcmp(token,"pinfo") == 0)
      {
        ExecutePINFO(token,delemiter);
      }
      else if(strcmp(token,"nightswatch") == 0)
      {
        ExecuteNightsWatch(token,delemiter);
      }
      else
      {
        ExecuteOtherCommands(token,delemiter);
      }
      return;
}
