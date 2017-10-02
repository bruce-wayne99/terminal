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

// intiliazation of global variables
char homepath[10000];
char *command[10000];
char usecommand[10000];
int usecommand_len;
char text[100000];
int command_len;
int text_len;
int no_commands;
int flag_process[10000];
int pid_process[10000];
int no_process;
