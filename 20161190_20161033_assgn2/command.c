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
#include "redirection.h"
#include "pwd.h"
#include "global.h"
#include "global.c"
#include "pipe.h"
#include "jobs.h"
#include "env.h"
// Function to execute any command based on the input given

void ExecuteCommand(char command[],char *token,const char delemiter[])
{
      if(strchr(command,'|') != NULL)
      {
        ExecutePipe(command,token,delemiter);
      }
      else if(strchr(command,'<') != NULL || strchr(command,'>') != NULL)
      {
        ExecuteRedirection(token,delemiter);
      }
      else if(strchr(command,'&') != NULL)
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
      else if(strcmp(token, "setenv") == 0)
      {
        ExecuteEnv(token, delemiter ,1);
      }
      else if(strcmp(token, "unsetenv") == 0)
      {
        ExecuteEnv(token, delemiter ,0);
      }
      else if(strcmp(token, "quit") == 0)
      {
          exit(0);
      }
      else if(strcmp(token, "jobs") == 0)
      {
        PrintJobs(delemiter);
      }
      else if(strcmp(token, "kjob") == 0)
      {
        KillJobs(token, delemiter);
      }
      else if(strcmp(token, "overkill") == 0)
      {
        OverKill();
      }
      else if(strcmp(token, "fg") == 0)
      {
        Fg(token, delemiter);
      }
      else if(strcmp(token, "bg") == 0)
      {
        Bg(token, delemiter);
      }
      else
      {
        ExecuteOtherCommands(token,delemiter);
      }
      return;
}
