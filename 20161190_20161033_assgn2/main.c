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
#include "redirection.h"
#include "pinfo.h"
#include "pwd.h"
#include "global.h"
#include "global.c"
void parenthandler(int signum)
{
  return;
}
void otherhandler(int signum)
{
  return;
}
int main()
{
  char systemname[1000],PresentPath[1000];
  register struct passwd *pw;
  register uid_t uid;
  char copy[10000];
  const char delemiter[] = " \t\r\n\v\f";
  const char seperator[] = ";";
  char *token,*multi;
  uid = geteuid();
  pw = getpwuid (uid);
  systemname[999] = 0;
  gethostname(systemname, 999);
  getcwd(homepath,10000);
  signal(SIGINT,parenthandler);
  signal(SIGTSTP,otherhandler);
  while(1)
  {
    if(pw)
    {
        PrintDisplayName(pw,systemname);
        fgets(text,100000,stdin);
        text_len = strlen(text);
        text[text_len-1] = 0;
        TokeniseCommands(command,&no_commands,text,seperator);
        int count = 0;
        while(count < no_commands)
        {
          command_len = strlen(command[count]);
          usecommand_len = command_len;
          strcpy(copy,command[count]);
          strcpy(usecommand,command[count]);
          token = strtok(copy,delemiter);
          if(token != NULL)
          {
            ExecuteCommand(usecommand,token,delemiter);
          }
          count++;
        }
    }
  }
  return 0;
}
