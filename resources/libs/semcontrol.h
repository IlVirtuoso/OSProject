#define _GNU_SOURCE
#ifndef SIGNAL_H
#include <signal.h>
#endif
#ifndef STDIO_H
#include <stdio.h>
#endif
#ifndef SHM_H
#include <sys/shm.h>
#endif
#ifndef STDLIB_H
#include <stdlib.h>
#endif
#ifndef UNISTD_H
#include <unistd.h>
#endif
#ifndef TYPES_H
#include <sys/types.h>
#endif
#ifndef SYSINFO_H
#include <sys/sysinfo.h>
#endif
#ifndef ERRNO_H
#include <sys/errno.h>
#endif
#ifndef IPC_H
#include <sys/ipc.h>
#endif
#ifndef MSG_H
#include <sys/msg.h>
#endif
#ifndef WAIT_H
#include <sys/wait.h>
#endif
#ifndef SEM_H
#include <sys/sem.h>
#endif
#ifndef TIME_H
#include <time.h>
#endif
#ifndef STRING_H
#include <string.h>
#endif
#ifndef MACRO_H
#include "macro.h"
#endif
#ifndef DEBUG_H
#include "debug.h"
#endif
#ifndef SIGNAL_H
#include <signal.h>
#endif
#ifndef SEMCONTROL_H
#define SEMCONTROL_H
int reserveSem(int semid, int semnum);

int releaseSem(int semid, int semnum);

int reserveSemNoWait(int semid, int semnum);

int waitzeroSem(int semid, int semnum);

int initsemAvailable(int semid, int semnum);

int initsemReserved(int semid, int semnum);



union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
#if defined(__linux__)
    struct seminfo *__buf;
#endif
};

#endif