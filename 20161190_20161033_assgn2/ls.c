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

//This function executes the simple ls function which lists the files in the directory
void ExecuteSimpleLS(char *token,int flag)
{
    DIR *d;
    struct dirent *dir;
    char path[10000];
    if(token == NULL)
    {
      d = opendir(".");
      if(d == NULL)
      {
        fprintf(stderr,"ls: cannot access %s : directory or file\n",token);
      }
      else
      {
        while ((dir = readdir(d)) != NULL)
        {
          if(dir->d_name[0]!='.' || flag == 1)
          {
            printf("%s\n", dir->d_name);
          }
        }
      }
    }
    else
    {

      int top = GetOrginalPath(path,token,strlen(token));
      path[top] = 0;
      struct stat file;
      if(stat(path,&file) == 0 && S_ISREG(file.st_mode))
      {
        printf("%s\n",path);
      }
      else
      {
        d = opendir(path);
        if(d == NULL)
        {
          fprintf(stderr,"ls: cannot access %s : directory or file\n",token);
        }
        else
        {
          printf("%s:\n",token);
          while ((dir = readdir(d)) != NULL)
          {
            if(dir->d_name[0]!='.' || flag == 1)
            {
              printf("%s\n", dir->d_name);
            }
          }
        }
      }
    }
    closedir(d);
    return;
}

// this function prints the output of ls -l command
void PrintLongListing(char temp[],char name[],int len,int file)
{
  struct stat fileStat;
  struct passwd *pw;
  struct group  *gr;
  char path[10000];
  char Time[10000];
  strcpy(path,temp);
  if(!file&&path[len-1] != '/')
  {
    path[len] = '/';
    path[len+1] = 0;
  strcat(path,name);
}
  if(stat(path,&fileStat) < 0)
      fprintf(stderr,"Failed to access the %s file\n",path);
  else
  {
    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x " : "- ");
    printf("%lu ",fileStat.st_nlink);
    pw = getpwuid(fileStat.st_uid);
    gr = getgrgid(fileStat.st_gid);
    printf("%s %s ",pw->pw_name,gr->gr_name);
    printf("%-6ld ",fileStat.st_size);
    char t[100];
    strcpy(t,ctime(&fileStat.st_mtime));
    t[16]=0;
    printf("%s ",t);
    printf("%s\n",name);
  }
  return;
}

// This function executes the command ls -l which prints the details of all the files present in the directory
void ExecuteLongLS(char *token,int flag)
{
  DIR *d;
  struct dirent *dir;
  char path[10000];
  if(token == NULL)
  {
    d = opendir(".");
    if(d == NULL)
    {
      fprintf(stderr,"ls: cannot access %s : directory or file\n",token);
    }
    else
    {
      while ((dir = readdir(d)) != NULL)
      {
        if(dir->d_name[0]!='.' || flag == 1)
        {
          PrintLongListing(".",dir->d_name,1,0);
        }
      }
    }
  }
  else
  {

    int top = GetOrginalPath(path,token,strlen(token));
    path[top] = 0;
    struct stat file;
    if(stat(path,&file) == 0 && S_ISREG(file.st_mode))
    {
      PrintLongListing(path,"",top,1);
    }
    else
    {
      d = opendir(path);
      if(d == NULL)
      {
        fprintf(stderr,"ls: cannot access %s : directory or file\n",token);
      }
      else
      {
        printf("%s:\n",token);
        while ((dir = readdir(d)) != NULL)
        {
          if(dir->d_name[0]!='.' || flag == 1)
          {
            PrintLongListing(path,dir->d_name,top,0);
          }
        }
      }
    }
  }
  closedir(d);
  return;
}

// Whenever ls is the first word of the command, this function is invoked which calls the appropriate function
void ExecuteLS(char *token,const char delemiter[])
{
    token = strtok(NULL,delemiter);
    int count = 0;
    if(token == NULL)
    {
      ExecuteSimpleLS(token,0);
    }
    else if(strcmp(token,"-a") == 0)
    {
      token = strtok(NULL,delemiter);
      if(token != NULL && token[0] == '-')
      {
        if(strcmp(token,"-l") == 0)
        {
          token = strtok(NULL,delemiter);
          if(token != NULL && token[0] == '-')
          {
              token = strtok(usecommand,delemiter);
              ExecuteOtherCommands(token,delemiter);
              return;
          }
          while(token != NULL)
          {
            count++;
            ExecuteLongLS(token,1);
            token = strtok(NULL,delemiter);
          }
          if(count == 0)
          {
            ExecuteLongLS(token,1);
          }
          return;
        }
          token = strtok(usecommand,delemiter);
          ExecuteOtherCommands(token,delemiter);
          return;
      }
      while(token != NULL)
      {
        count++;
        ExecuteSimpleLS(token,1);
        printf("\n");
        token = strtok(NULL,delemiter);
      }
      if(count == 0)
      {
        ExecuteSimpleLS(token,1);
      }
    }
    else if(strcmp(token,"-l") == 0)
    {
      token = strtok(NULL,delemiter);
      if(token != NULL &&  token[0] == '-')
      {
        if(strcmp(token,"-a") == 0)
        {
          token = strtok(NULL,delemiter);
          if(token != NULL && token[0] == '-')
          {
              token = strtok(usecommand,delemiter);
              ExecuteOtherCommands(token,delemiter);
              return;
          }
          while(token != NULL)
          {
            count++;
            ExecuteLongLS(token,1);
            token = strtok(NULL,delemiter);
          }
          if(count == 0)
          {
            ExecuteLongLS(token,1);
          }
          return;
        }
          token = strtok(usecommand,delemiter);
          ExecuteOtherCommands(token,delemiter);
          return;
      }
      while(token != NULL)
      {
        count++;
        ExecuteLongLS(token,0);
        token = strtok(NULL,delemiter);
      }
      if(count == 0)
      {
        ExecuteLongLS(token,0);
      }
    }
    else if(strcmp(token,"-al") == 0 || strcmp(token,"-la") == 0)
    {
      token = strtok(NULL,delemiter);
      if(token != NULL && token[0] == '-')
      {
          token = strtok(usecommand,delemiter);
          ExecuteOtherCommands(token,delemiter);
          return;
      }
      while(token != NULL)
      {
        count++;
        ExecuteLongLS(token,1);
        token = strtok(NULL,delemiter);
      }
      if(count == 0)
      {
        ExecuteLongLS(token,1);
      }
    }
    else
    {
        ExecuteSimpleLS(token,0);
    }
    return;
}
