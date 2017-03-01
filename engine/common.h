#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#define MAXPRINTMSG     4096

void Comm_Printf(char * fmt, ...);
void Comm_Error(int code, char *fmt, ...);
void Comm_Init(void);



#endif // COMMON_H_INCLUDED
