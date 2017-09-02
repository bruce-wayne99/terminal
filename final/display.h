#ifndef DISPLAY
#define DISPLAY

void TokeniseCommands(char *commands[],int *a,char text[],const char delemiter[]);
void PrintDisplayName(register struct passwd *pw,char systemname[]);
int GetOrginalPath(char path[],char token[],int len);

#endif
