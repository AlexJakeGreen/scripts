
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdarg.h>
#include "common.h"
#include "client_main.h"
#include "video_main.h"

void Comm_Printf(char * fmt, ...) {
    va_list argptr;
    char msg[MAXPRINTMSG];

    va_start(argptr, fmt);
    vsprintf(msg, fmt, argptr);
    va_end(argptr);

    printf("%s", msg);
};

void Comm_Error(int code, char *fmt, ...) {
    va_list argptr;
    char msg[MAXPRINTMSG];

    va_start(argptr, fmt);
    vsprintf(msg, fmt, argptr);
    va_end(argptr);

    printf("%s", msg);
    exit(code);
};

void Comm_Init(void) {

    Client_Init();

    while(1) {

        HandleEvents();

    };

};


void signal_handler(int sig) {
    printf("Received signal %d, exiting...\n", sig);
    GL_Shutdown();
    exit(0);
};

void Comm_InitSig(void) {
    signal(SIGTERM, signal_handler);
    signal(SIGQUIT, signal_handler);
};

