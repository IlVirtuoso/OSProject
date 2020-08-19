#ifndef STDLIB_H
#include <stdlib.h>
#endif
#ifndef STDIO_H
#include <stdio.h>
#endif
#ifndef STRING_H
#include <string.h>
#endif
#ifndef DEBUG_H
#include "debug.h"
#endif
#ifdef ERROR_H
#include "error.h"
#endif
#ifndef ERRNO_H
#include "errno.h"
#endif
#ifndef PARAMETERS_H
#define PARAMETERS_H

/*altezza della tabella*/
int SO_ALTEZZA;

/*larghezza della tabella*/
int SO_BASE;

/*massimo numero di flag*/
int SO_FLAG_MAX;

/*minimo numero di flag*/
int SO_FLAG_MIN;

/*Tempo massimo di un round*/
int SO_MAX_TIME;

/*tempo minimo di occupazione di una cella in nanosecondi*/
int SO_MIN_HOLD_NSEC;

/*numero di mosse rimanenti*/
int SO_N_MOVES;

/*numero di giocatori*/
int SO_NUM_G;

/*numero di pedine*/
int SO_NUM_P;

/*punteggio da raggiungere*/
int SO_ROUND_SCORE;

/*parsifica un file di configurazione aggiornando le variabili*/
void ParseFile(FILE * file);


#endif