#define _GNU_SOURCE
#ifndef STDIO_H
#include <stdio.h>
#endif
#ifndef STDLIB_H
#include <stdlib.h>
#endif
#ifndef TIME_H
#include <time.h>
#endif
#ifndef STRINGS_H
#include <strings.h>
#endif
#ifndef STRING_H
#include <string.h>
#endif
#ifndef UNISTD_H
#include <unistd.h>
#endif
#ifndef TYPES_H
#include <sys/types.h>
#endif
#ifndef STDARG_H
#include <stdarg.h>
#endif

#ifndef DEBUG_H
#define DEBUG_H
/*identifica se il Debug è o no attivo*/
int isDebug; 

/*variabile indicante la verbosità del log*/
int verbosity;


/*scrive su console e su un file un messaggio, utile per verificare la corretta esecuzione !!USATA SOLO DAL PROCESSO MASTER*/
void logg( const char *__restrict__ message, ...); 

/* segnala un errore e ne mostra la causa, poi invoca clean() per prepararsi all'uscita*/
void error(char message [], int err);

/* funzione per mostrare il [debug] su console, attenzione la variabile isDebug deve essere 1 */
int debug(const char *__restrict__ message, ...);

/*puntatore alla funzione clean ridichiarata in ogni processo*/
void (* cleaner) ();

/* firma di riconoscimento del processo*/
char * processSign;


#endif