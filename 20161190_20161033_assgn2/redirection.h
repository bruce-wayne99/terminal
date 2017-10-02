#ifndef REDIRECT
#define REDIRECT

void ExecuteRedirection(char *token,const char delemiter[]);
void OutputRedirection(char *argv[], int start, int end, const char delemiter[], int isappend);
void InputRedirection(char *argv[], int start, int end, const char delemiter[]);

#endif
