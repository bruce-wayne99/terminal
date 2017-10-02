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
#include "redirection.h"
#include "jobs.h"
#include "global.h"
#include "global.c"

//handler for printing when a background process exits.
void childhandler(int signum)
{
	  pid_t childpid;
    int status;
    while ((childpid = waitpid( -1, &status, WNOHANG)) > 0)
    {
        RemoveJob(childpid);
        if (WIFEXITED(status)) {
            printf("\nChild process with pid %d exited normally\n", childpid);
        }

        if (WIFSIGNALED(status)) {
            printf("\nChild process with pid %d exited because of signal\n", childpid);
        }
    }
    return;
}

// This function is to execute backgroung and foreground commands
int ExecuteOtherCommands(char *token,const char delemiter[])
{
      char *argv[10000];
      int top = 0;
      int flag = 0;
      int pid;
      int pid1,val;
      int k;
			int status;
			int cpid;
      while(token != NULL)
      {
        argv[top++] = token;
        token = strtok(NULL,delemiter);
        if(token == NULL && strcmp(argv[top-1],"&") == 0)
        {
          flag = 1;
        }
      }
      argv[top] = NULL;
      if(flag == 0)
      {
          pid = fork();
          if(pid == 0)
          {
              k = execvp(argv[0],argv);
              if(k<0)
              {
                 fprintf(stderr,"Invalid command\n");
              }
            	exit(0);
          }
          else
          {
            // wait(NULL);
						do
						{
							cpid = waitpid(pid,&status,WUNTRACED);
						} while(!WIFEXITED(status) && !WIFSIGNALED(status) && !WIFSTOPPED(status));

            return 0;
          }
      }
      else
      {
        argv[top-1] = NULL;
        pid = fork();
        if(pid == 0)
        {
          if(strcmp(argv[0],"cd") == 0)
          {
            usecommand[strchr(usecommand,'&')-usecommand] = ' ';
            token = strtok(usecommand,delemiter);
            ExecuteCD(token,delemiter);
          }
          else
          {
            k = execvp(argv[0],argv);
            if(k<0)
            {
               fprintf(stderr,"Invalid Command\n");
            }
          }
          exit(0);
        }
        else
        {
            AddJob(pid);
            signal(SIGCHLD, childhandler);
            printf("%d\n",pid);
            return 0;
        }
      }
}
