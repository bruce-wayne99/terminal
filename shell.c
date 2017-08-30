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

char homepath[10000];
char command[10000];

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
int ExecuteOtherCommands(char *token,const char delemiter[])
{
      char *argv[10000];
      int top = 0;
      int flag = 0;
      int pid;
      int k;
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
               fprintf(stderr,"%s: command not found\n",argv[0]);
            }
            return 0;
          }
          else
          {
            wait(NULL);
            return 0;
          }
      }
      else
      {
        argv[top-1] = NULL;
        pid = fork();
        if(pid == 0)
        {
          k = execvp(argv[0],argv);
          if(k<0)
          {
             fprintf(stderr,"%s: command not found\n",argv[0]);
          }
          return 0;
        }
        else
        {
            printf("%d\n",pid);
            return 0;
        }
      }
}
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
      printf("%s : No such directory\n",token);
    }
    return;
}
void ExecutePWD()
{
  char path[10000];
  printf("%s\n",getcwd(path,10000));
}
void ExecuteECHO(char command[],char *token,const char delemiter[])
{
    token = strtok(NULL,delemiter);
    int ind = 0,i;
    int open = 0;
    int len = strlen(command);
    if(token == NULL)
    {
      printf("\n");
      return;
    }
    while(ind<len&&command[ind]!='"')
    {
      ind++;
    }
    ind++;
    for(i=ind;i<len-1;i++)
    {
      printf("%c",command[i]);
    }
    printf("\n");
    return;
}
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
  }
  strcat(path,name);
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
          token = strtok(command,delemiter);
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
      if(token != NULL && token[0] == '-')
      {
          token = strtok(command,delemiter);
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
          token = strtok(command,delemiter);
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

void ConvertPID(char stringpid[],int pid)
{
    int temp = pid;
    int count = 0;
    while(temp != 0)
    {
      count++;
      temp = temp/10;
    }
    temp = pid;
    stringpid[count] = 0;
    while(temp != 0)
    {
      stringpid[count-1] = temp%10 + '0';
      temp = temp/10;
      count--;
    }

    return;
}
void ExecutePINFO(char *token,const char delemiter[])
{
      token = strtok(NULL,delemiter);
      int pid;
      char stringpid[1000],c[1000000];
      if(token == NULL)
      {
        pid = getpid();
        ConvertPID(stringpid,pid);
      }
      else
      {
        strcpy(stringpid,token);
      }
      int source;
      char s[] = "/proc/";
      // printf("printing the pid %s\n",stringpid);
      strcat(s,stringpid);
      strcat(s,"/stat");
      source  = open(s,O_RDONLY);
      if(source < 0)
      {
        fprintf(stderr,"Invalid process id \n");
        close(source);
        return;
      }
      int byte = read(source,c,1000000);
      int count = 1;
      char Id[100],Status[100],VMemory[100],ExecutePath[100],*content;
      content = strtok(c,delemiter);
      while(content != NULL)
      {
        if(count == 1)
        {
          strcpy(Id,content);
        }
        if(count == 2)
        {
          strcpy(ExecutePath,content);
        }
        if(count == 3)
        {
          strcpy(Status,content);
        }
        if(count == 25)
        {
          strcpy(VMemory,content);
        }
        count++;
        content = strtok(NULL,delemiter);
      }
      printf("pid -- %s\n",Id);
      printf("Process Status -- %s\n",Status);
      printf("Virtual Memory -- %s\n",VMemory);
      printf("Executable Path -- %s\n",ExecutePath);
      return;
}
void ExecuteCommand(char command[],char *token,const char delemiter[])
{
      if(strcmp(token,"cd") == 0)
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
      else
      {
        ExecuteOtherCommands(token,delemiter);
      }
      // else
      // {
      //   fprintf(stderr,"%s: command not found\n",token);
      // }
      return;
}
int main()
{
  int command_len;
  char systemname[1000],PresentPath[1000];
  register struct passwd *pw;
  register uid_t uid;
  char copy[10000];
  const char delemiter[2] = " ";
  char *token;
  uid = geteuid();
  pw = getpwuid (uid);
  systemname[999] = 0;
  gethostname(systemname, 999);
  getcwd(homepath,10000);
  while(1)
  {
    if(pw)
    {
        PrintDisplayName(pw,systemname);
        fgets(command,10000,stdin);
        command_len = strlen(command);
        command[command_len-1] = 0;
        strcpy(copy,command);
        token = strtok(copy,delemiter);
        if(token != NULL)
        {
          ExecuteCommand(command,token,delemiter);
        }
    }
  }
  return 0;
}
