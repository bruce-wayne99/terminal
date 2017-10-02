#ifndef JOBS
#define JOBS

void AddJob(int pid);
void RemoveJob(int pid);
void PrintJobs(const char delemiter[]);
void KillJobs(char *token, const char delemiter[]);
void OverKill();
void Fg(char *token, const char delemiter[]);
void Bg(char *token, const char delemiter[]);

#endif
