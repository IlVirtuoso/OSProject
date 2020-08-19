#define _GNU_SOURCE
#ifndef STDIO_H
#include <stdio.h>
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
#ifndef TIME_H
#include <time.h>
#endif
#ifndef STRING_H
#include <string.h>
#endif
#ifndef TABLE_H
#include "table.h"
#endif
#ifndef TIME_H
#include <time.h>
#endif
#ifndef TABLE_H
#include "table.h"
#endif

#ifndef MONITOR_H
#define MONITOR_H

/*permette di mostrare lo stato della tabella, il metodo è costruito per funzionare a frame */
void display();

/*metodo per vedere la tabella dei semafori*/
void display_sem();



#endif