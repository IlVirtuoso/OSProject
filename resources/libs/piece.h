#define _GNU_SOURCE
#ifndef DEBUG_H
#include "debug.h"
#endif
#ifndef SHM_H
#include <sys/shm.h>
#endif
#ifndef TABLE_H
#include "table.h"
#endif
#ifndef SIGNAL_H
#include <signal.h>
#endif
#ifndef MONITOR_H
#include "monitor.h"
#endif
#ifndef MESSAGE_H
#include "message.h"
#endif

#ifndef PIECE_H
#define PIECE_H
/*funzione che inizializza la pedina*/
int piece();

typedef struct{
    int piece_id; /*id del pezzo, tipicamente è la sua posizione nell'array dei pezzi*/
    int x;
    int y; /*posizione della pedina*/ 
    long n_moves; /* numero mosse disponibili*/
    int strategy;
    pid_t piecepid;
}attributes;

int sharedStats;


attributes * piece_attr;
attributes * all_pieces;
char player_id;

int piece_id;

/*messagequeue con il player*/
int player_msgqueue;

void piece_handler(int signum);

void piece_cleaner();

/*variabile di controllo, dice se ha o meno accesso a una cella della tabella*/
int hasaccess;

int master_msgqueue;

/*segmento di memoria condivisa della table*/
cell * piece_shared_table;

/*funzionano come sempre*/
sigset_t piece_mask;

/*buffer per i messaggi modificati*/
char piece_logbuffer[128];

/**
 * muove la pedina fino al punto da raggiungere
 * @param x  intero che indica la riga
 * @param y  intero che indica la colonna
 * @param method char che indica che metodo usare nello switch per raggiungere quel punto
 */
int goto_loc(int target_x, int target_y, char strategy);

/* Verifica se la cella obiettivo è libera */
char cond_free(int x, int y);

/* Verifica se la cella bersaglio non è stata già percorsa nell'immediato */
char cond_old(int x, int y);

/* Verifica se la cella bersaglio non eccede i limiti della tabella */
char cond_valid(int x, int y);

/* Verifica che la cella bersaglio sia ottimale per lo spostamento */
char cond(int x, int y);

int move(int x, int y);

struct sigaction piece_signal;

/*semaforo*/

int setpos(int x, int y);

int semglobal;

int semplayer;

int semnum;
enum{
    X_BEFORE,
    Y_BEFORE,
    DIAGONAL,
    CHAOS_THEORY,
    EVASION_Y,
    EVASION_X,
    STAND,
    STRAIGHT_TO
};

/* Struttura adibita a memorizzare le coordinate della cella target */
position target;

#endif



