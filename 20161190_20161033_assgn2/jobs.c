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

void AddJob(int pid)
{
    flag_process[no_process] = 1;
    pid_process[no_process] = pid;
    no_process++;
    return;
}
void RemoveJob(int pid)
{
    int i;
    for(i = 0 ; i < no_process ; i++)
    {
      if(pid_process[i] == pid)
      {
        flag_process[i] = -1;
        break;
      }
    }
    return;
}
void PrintJobs(const char delemiter[])
{
    int i;
    int count = 1;
    int val = 0;
    int byte;
    int source;
    char *content;
    char c[1000000],Name[100];
    char temp[1000],Status[100];
    for(i = 0 ; i < no_process ; i++)
    {
      count = 1;
      if(flag_process[i] == -1)
        continue;
      val++;
      sprintf(temp,"/proc/%d/stat",pid_process[i]);
      source = open(temp, O_RDONLY);
      if(source < 0)
      {
          fprintf(stderr,"Invalid process id \n");
          close(source);
      }
      byte = read(source,c,1000000);
      content = strtok(c,delemiter);
      while(content != NULL)
      {
        if(count == 2)
        {
          strcpy(Name,content);
          Name[strlen(Name) - 1] = 0;
        }
        if(count == 3)
        {
          strcpy(Status,content);
        }
        count++;
        content = strtok(NULL,delemiter);
      }
      if(strcmp(Status,"T") == 0)
      {
        printf("[%d] Stopped %s [%d]\n", val, Name + 1, pid_process[i]);
      }
      else
      {
        printf("[%d] Running %s [%d]\n", val, Name + 1, pid_process[i]);
      }
    }
    return;
}
void KillJobs(char *token, const char delemiter[])
{
    int i,val = 0,top = 0, job_id, pid = -1;
    char *argv[20];
    while(token != NULL)
    {
      argv[top++] = token;
      token = strtok(NULL, delemiter);
    }
    if(top != 3)
    {
      perror("Invalid usage of the command\n");
      return;
    }
    job_id = atoi(argv[1]);
    int signal_id = atoi(argv[2]);
    for(i = 0 ; i < no_process ; i++)
    {
      if(flag_process[i] == -1)
      {
        continue;
      }
      val++;
      if(val == job_id)
      {
        pid = pid_process[i];
        break;
      }
    }
    if(pid == -1)
    {
      perror("No process with the given job id exists");
      return;
    }
    if(kill(pid, signal_id) < 0)
    {
      perror("Invalid usage or wrong signal number");
      return;
    }
    return;
}
void OverKill()
{
  int i;
  for(i = 0 ; i < no_process ; i++)
  {
    if(flag_process[i] == -1)
      continue;
      if(kill(pid_process[i],9) < 0)
      {
        perror("No process with the given job id exists");
        return;
      }
  }
  return;
}
void Fg(char *token, const char delemiter[])
{
    char *argv[100];
    int top = 0,i;
    while(token != NULL)
    {
      argv[top++] = token;
      token = strtok(NULL, delemiter);
    }
    if(top != 2)
    {
      perror("Incorrect number of arguments");
      return;
    }
    int job_id = atoi(argv[1]);
    int val = 0,pid = -1,status;
    for(i = 0 ; i < no_process ; i++)
    {
      if(flag_process[i] == -1)
      {
        continue;
      }
      val++;
      if(val == job_id)
      {
        pid = pid_process[i];
        break;
      }
    }
    if(pid == -1)
    {
      perror("No process with the given job id exists");
      return;
    }
    if(waitpid(pid, &status, WUNTRACED) < 0)
    {
      perror("Not able to execute the command");
      return;
    }
    flag_process[i] = -1;
    return;
}
void Bg(char *token, const char delemiter[])
{
    int i,val=0,pid=-1,top=0;
    char *argv[100];
    while(token != NULL)
    {
      argv[top++] = token;
      token = strtok(NULL, delemiter);
    }
    if(top != 2)
    {
      perror("Incorrect number of arguments");
      return;
    }
    int job_id = atoi(argv[1]);
    for(i = 0 ; i < no_process ; i++)
    {
      if(flag_process[i] == -1)
      {
        continue;
      }
      val++;
      if(val == job_id)
      {
        pid = pid_process[i];
        break;
      }
    }
    if(pid == -1)
    {
      perror("No process with the given job id exists");
      return;
    }
    if(kill(pid, 18) < 0)
    {
      perror("Invalid usage or wrong signal number");
      return;
    }
    return;
}
