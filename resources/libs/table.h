

#define _GNU_SOURCE
#ifndef SHM_H
#include <sys/shm.h>
#endif
#ifndef MESSAGE_H
#include "message.h"
#endif
#ifndef DEBUG_H
#include "debug.h"
#endif
#ifndef STDIO_H
#include <stdio.h>
#endif
#ifndef STDLIB_H
#include <stdlib.h>
#endif
#ifndef STAT_H
#include <sys/stat.h>
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
#ifndef PARAMETERS_H
#include "parameters.h"
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
#ifndef MACRO_H
#include "macro.h"
#endif
#ifndef SEMCONTROL_H
#include "semcontrol.h"
#endif
#ifndef TABLE_H
#define TABLE_H

/* Valore assoluto di x */
#define ABSOLUTE(x) ((x) >= 0 ? (x) : (x) * (-1))

/*definizione della struttura della cella della tabella*/
typedef struct
{
    char id;
    int isFull;
    int flag;
    long  int player_n_moves;
} cell;

typedef struct
{
    int x;
    int y;
} position;

/*metodo per muoversi nella tabella*/
cell *tab(cell *table, int x, int y);

/**
 * metodo che cerca la posizione di un target all'interno di una tabella
 * @param shared_table: indirizzo alla table condivisa
 * @param  b, h: base e altezza da cui partire
 * @param target: target cercato sulla tabella
 * @param moves: numero di iterazioni di ricerca
 * */
position search(cell *shared_table, int b, int h, char target, int itera);

/* Ritorna la distanza tra il punto (x,y) ed il punto (x_targ, y_targ)*/
int getDistance(int x, int y, int x_targ, int y_targ);

/**
 * @param moves: numero di mosse rimaneneti, tipicamente piece_attr.moves
 * @param x,y: coordinate attuali della pedina
 * @param x_targ,y_targ: coordinate obiettivo della pedina
 * @return -1: se il numero di mosse non è sufficiente a raggiungere la cella target
 * @return n_moves: se il numero di mosse è sufficiente a raggiungere target
 */
int reachable(int moves, int x, int y, int x_targ, int y_targ);

/*id della table*/
int table;

/*id della tabella semaforica*/
int sem_table;

key_t sem_table_key;

/*inizializza la scacchiera in memoria condivisa*/
void table_start();

/*metodo usato per leggere l'id' di una cella*/
char getid(cell *shared_table, int x, int y);

/**
 * Metodo usato dalle pedine per controllare lo stato di una cella, utile per il metodo move() 
 * [!!!]rilascia automaticamente il semaforo di una cella prima di settare quella successiva
 * [!!!] se previous_x e previous_y sono impostati a -1 il metodo non rilascia nessun semaforo
 * @return  1: successo, 0:errore
 * @param shared_table: Puntatore alla tabella condivisa
 * @param x: Riga della tabella a cui puntare
 * @param y: colonna della tabella a cui puntare
 * @param id: id da scrivere nella cella al campo cell->id
 * @param previous_x: riga della cella da rilasciare prima di spostarsi[tipicamente: piece_attr.x]
 * @param previous_y : colonna della cella da rilasciare prima di spostarsi[tipicamente: piece_attr.y]
 */

/*metodo usato dal master per piazzare le bandiere*/
void placeflag(cell *shared_table, int x, int y);

/*metodo usato dal master per rimuovere le bandiere*/
void removeflag(cell *shared_table, int x, int y);

/**
 * Metodo usato dal master per settare il valore dell'ultima cella dell'array, nascosta al resto 
 * del gioco, che serve per far ripartire i ROUNDS
 */
void setRestartCell(cell *shared_table, int id);

char getRestartCell(cell *shared_table);

int semglobal;

int semplayer;

struct sembuf sem_t;

#endif