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

void handler(int signum)
{
  exit(0);
}

// This function executes the nightwatch command
void ExecuteNightsWatch(char *token,const char delemiter[])
{
    char *flag = strtok(NULL,delemiter);
    if(flag == NULL)
    {
      fprintf(stderr,"Invalid usage of nightswatch\n");
      return;
    }
    char *seconds = strtok(NULL,delemiter);
    if(seconds == NULL)
    {
      fprintf(stderr,"Invalid usage of nightswatch\n");
      return;
    }
    char *string = strtok(NULL,delemiter);
    if(string == NULL)
    {
      fprintf(stderr,"Invalid usage of nightswatch\n");
      return;
    }
    int val=0;
    int top=0;
    int len = strlen(seconds);
    char *terms[100];
    int i = 0;
    int fd,byte;
    char s[100000];
    char copy[10000];
    char *data;
    while(i<len)
    {
      val = val*10 + (seconds[i] - '0');
      i++;
    }
    int flag1 = 0;
    int pid;
    char temp[100000];
    if(strcmp(string,"interrupt") == 0)
    {
      pid = fork();
      if(pid == 0)
      {
        signal(SIGINT,handler);
        while(1)
        {
          fd = open("/proc/interrupts",O_RDONLY);
          if(fd < 0)
          {
            perror("Error");
            close(fd);
            return;
          }
          byte = read(fd,s,100000);
          TokeniseCommands(terms,&top,s,"\n");
          i = 0;
          printf("%s\n",terms[0]);
          while(i<top)
          {
            strcpy(copy,terms[i]);
            flag1 = 0;
            token = strtok(copy," \t\r\n\v\f");
            while(token != NULL)
            {
              if(strcmp(token,"i8042") == 0)
              {
                flag1 = 1;
                break;
              }
              token = strtok(NULL," \t\r\n\v\f");
            }
            if(flag1 == 1)
            {
              terms[i][strlen(s)-6] = 0;
              printf("%s\n",terms[i]);
            }
            i++;
          }
          printf("\n");
          sleep(val);
        }
      }
      else
      {
        fgets(temp,100000,stdin);
        if(temp[0] == 'q')
        {
          kill(pid,SIGINT);
        }
      }
    }
    if(strcmp(string,"dirty") == 0)
    {
      pid = fork();
      if(pid == 0)
      {
        signal(SIGINT,handler);
        while(1)
        {
          fd = open("/proc/meminfo",O_RDONLY);
          if(fd < 0)
          {
            perror("Error");
            close(fd);
            return;
          }
          byte = read(fd,s,100000);
          TokeniseCommands(terms,&top,s,"\n");
          i = 0;
          // printf("%d is top\n",top);
          while(i<top)
          {
            strcpy(copy,terms[i]);
            flag1 = 0;
            token = strtok(copy," \t\r\n\v\f");
            while(token != NULL)
            {
              if(strcmp(token,"Dirty:") == 0)
              {
                // printf("%s is token\n",token);
                flag1 = 1;
                break;
              }
              token = strtok(NULL," \t\r\n\v\f");
            }
            if(flag1 == 1)
            {
              // printf("OKOSKFOSDKF\n");
              printf("%s\n",terms[i]);
              break;
            }
            i++;
          }
          printf("\n");
          sleep(val);
        }
      }
      else
      {
        fgets(temp,100000,stdin);
        if(temp[0] == 'q')
        {
          kill(pid,SIGINT);
        }
      }
    }
    return;
}
