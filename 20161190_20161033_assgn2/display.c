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

// When a command is given, it is split based on the approproate delemiter .
void TokeniseCommands(char *commands[],int *a,char text[],const char delemiter[])
{
  *a = 0;
  char *token = strtok(text,delemiter);
  while(token != NULL)
  {
    commands[*a] = token;
    token = strtok(NULL,delemiter);
    *a = *a + 1;
  }
  return;
}

// This function displays the user name and system name
void PrintDisplayName(register struct passwd *pw,char systemname[])
{
  char path[10000];
  char subpath[10000];
  int flag = 0;
  int home_len = strlen(homepath);
  int path_len = strlen(getcwd(path,10000));
  if(path_len>=home_len)
  {
    memcpy(subpath,&getcwd(path,1000)[0],home_len);
    subpath[home_len]=0;
    if(strcmp(subpath,homepath) == 0)
    {
      flag = 1;
    }
  }

  if(flag == 1)
  {
    printf("<%s@%s:~%s> ",pw->pw_name,systemname,&(getcwd(path,10000)[home_len]));
  }
  else
  {
    printf("<%s@%s:%s> ",pw->pw_name,systemname,getcwd(path,10000));
  }
  return;
}

// This function gets the original path of the file
int GetOrginalPath(char path[],char token[],int len)
{
  int i,j;
  int home_len = strlen(homepath);
  int top = 0;
  for(i=0;i<len;i++)
  {
    if(token[i]!='~')
    {
      path[top++] = token[i];
    }
    else
    {
      for(j=0;j<home_len;j++)
      {
        path[top++] = homepath[j];
      }
    }
  }
  return top;
}
