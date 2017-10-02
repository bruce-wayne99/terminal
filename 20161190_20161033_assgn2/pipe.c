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


void ExecutePipe(char command[],char * token,const char delemiter[])
{
  char *sub_command[10000];
  char temp[1000];
  const char pipe_seperator[] = "|";
  int no_pipes = 0,i;
  char temp2[1000];
  TokeniseCommands(sub_command,&no_pipes,command,pipe_seperator);

  int stdin = dup(0);
  int stdou = dup(1);
  int fdin = dup(stdin),fdout;
  char test[1000];
  for(i=0;i<no_pipes;i++)
  {

      if(dup2(fdin,0) != 0)
      {
          perror("dup2 failed\n");
      }
      close(fdin);

      if(i == no_pipes - 1)
      {
          fdout = dup(stdou);
      }
      else
      {
        int fildes[2];
        if(pipe(fildes) < 0)
        {
          perror("pipes failed");
        }
        fdout = fildes[1];
        fdin = fildes[0];
       }
      if(dup2(fdout,1) != 1)
      {
        perror("dup2 failed\n");
      }
      close(fdout);
      strcpy(temp,sub_command[i]);
      token = strtok(temp,delemiter);
      ExecuteCommand(sub_command[i], token, delemiter);
  }

    dup2(stdin,0);
    dup2(stdou,1);

  return;
}
