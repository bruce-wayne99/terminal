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

void OutputRedirection(char *argv[], int start, int end, const char delemiter[], int isappend)
{
    char *cond[10000];
    int top_cond = start;
    int fd = 0,pid;
    int len = 0;
    int stdout = dup(1);
    char command[100000];
    char usecommand[100000];
    command[0] = 0;
    while(top_cond < end)
    {
        cond[top_cond] = argv[top_cond];
        strcat(command,cond[top_cond]);
        len = strlen(command);
        command[len] = ' ';
        command[len + 1] = 0;
        top_cond++;
    }
    command[len] = 0;
    strcpy(usecommand,command);
    char *token = strtok(usecommand, delemiter);
    cond[top_cond] = NULL;
    if(isappend)
    {
      fd = open(argv[end + 1], O_WRONLY | O_APPEND , 0644);
      if(fd == -1)
      {
        fd = open(argv[end + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
      }
    }
    else
    {
      fd = open(argv[end + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }
    if(fd == -1)
    {
      perror("Failed to open file");
      close(fd);
      return;
    }
    if(dup2(fd, 1) != 1)
    {
      perror("dup2 fail\n");
      close(fd);
      return;
    }
    close(fd);
    ExecuteCommand(command, token, delemiter);
    if(dup2(stdout, 1) != 1)
    {
      perror("dup2 fail\n");
      return;
    }
}
void InputRedirection(char *argv[], int start, int end, const char delemiter[])
{
    char *cond[10000];
    int top_cond = start;
    int fd = 0,pid;
    int len = 0;
    int stdin = dup(0);
    char command[100000];
    char usecommand[100000];
    command[0] = 0;
    while(top_cond < end)
    {
        cond[top_cond] = argv[top_cond];
        strcat(command,cond[top_cond]);
        len = strlen(command);
        command[len] = ' ';
        command[len + 1] = 0;
        top_cond++;
    }
    command[len] = 0;
    strcpy(usecommand,command);
    char *token = strtok(usecommand, delemiter);
    cond[top_cond] = NULL;
    fd = open(argv[end + 1], O_RDONLY);
    if(fd == -1)
    {
      perror("Failed to open file");
      close(fd);
      return;
    }
    close(0);
    if(dup2(fd, 0) != 0)
    {
      perror("dup2 fail\n");
      close(fd);
      return;
    }
    close(fd);
    ExecuteCommand(command, token, delemiter);

    if(dup2(stdin, 0) != 0)
    {
      perror("dup2 fail\n");
      return;
    }
}
void ExecuteRedirection(char *token,const char delemiter[])
{
    char *argv[10000];
    int top = 0;
    while(token != NULL)
    {
        argv[top++] = token;
        token = strtok(NULL,delemiter);
    }
    int i = top - 1;
    int start = 0;
    int fd = 0;
    while(strcmp(argv[i],"<") != 0 && strcmp(argv[i],">") != 0 && strcmp(argv[i],">>") != 0 && i >= 0)
    {
      i--;
    }
    if(strcmp(argv[i],">") == 0)
    {
        OutputRedirection(argv, start, i, delemiter, 0);
    }
    if(strcmp(argv[i],"<") == 0)
    {
        InputRedirection(argv, start, i, delemiter);
    }
    if(strcmp(argv[i],">>") == 0)
    {
        OutputRedirection(argv, start, i, delemiter, 1);
    }
    return;
}
