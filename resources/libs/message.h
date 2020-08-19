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


#ifndef MESSAGE_H
#define MESSAGE_H
/* struct per message queue; default su MQ = 1 */ 
typedef struct{
    long type;
    int pednum; /* sarà usato per selezionare la pedina a cui inviare il messaggio*/
    char strategy;
    char x;
    char y;
    int ask;
    int id;
    int phase;
}msg_cnt;

typedef struct{
    int playernum;
    int phase;
}mastermem;

/*msgqueue globale*/
mastermem * masterStruct;

/*chiave message queue*/
int key_MO;



#endif