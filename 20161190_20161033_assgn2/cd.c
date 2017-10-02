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

// Function to execute the command 'CD'
void ExecuteCD(char *token,const char delemiter[])
{
    int i,j;
    token = strtok(NULL,delemiter);
    int len;
    if(token == NULL)
    {
      token = homepath;
    }
    len = strlen(token);
    char path[100000];
    int top = GetOrginalPath(path,token,len);
    path[top]=0;
    int val = chdir(path);
    if(val<0)
    {
      perror("Error");
    }
    return;
}
