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
#ifndef STAT_H
#include <sys/stat.h>
#endif
#ifndef STRING_H
#include <string.h>
#endif
#ifndef PARAMETERS_H
#include "./resources/libs/parameters.h"
#endif
#ifndef PLAYER_H
#include "./resources/libs/player.h"
#endif
#ifndef PIECE_H
#include "./resources/libs/piece.h"
#endif
#ifndef MONITOR_H
#include "./resources/libs/monitor.h"
#endif
#ifndef SHM_H
#include <sys/shm.h>
#endif
#ifndef SIGNAL_H
#include <signal.h>
#endif
#ifndef MACRO_H
#include "./resources/libs/macro.h"
#endif
#ifndef DEBUG_H
#include "./resources/libs/debug.h"
#endif
#ifndef TABLE_H
#include "./resources/libs/table.h"
#endif

/*handler per il segnale di interruzione SIGINT*/
void handler(int signum);

/*cleaner per SIGINT*/
void clean();

/*definisce quante flag sono già state create*/
int placed;

/*uccide tutti i processi inizializzati dal processo master*/
void clean_process();

/*master methods*/
void init();
void sem_init();
void shared_table_init();
void playergen(int playernum);
void manual();
void restart();

struct sigaction sa;

/*struttura che definisce le bandiere, visibile solo al master*/
typedef struct
{
    int x;
    int y;
    int score;
    int taken;
} vexillum;

/*id della scacchiera*/
int table;

/*clock per misurare il tempo di esecuzione*/
clock_t cl;

/*clock per il round, resettare a fine round*/
clock_t round_timer;

/*mask per i segnali*/
static sigset_t mask;

/*segmento di memoria condivisa della table*/
cell *master_shared_table;

/* variabile che dice se i giocatori sono stati creati*/
int playercreated = 0;

/*status dell'exit process*/
int status;

/*numero di semafori da attivare, può cambiare in base alle necessità*/
int sem_num;

/* struttura dati per i punteggi */
typedef struct
{
    /* Array contenente i pids di tutti i processi giocatori creati*/
    int *pid;
    /* Array contenente i nomi di tutti i processi giocatori creati*/
    char *name;
    /* Array contenente il punteggio di tutti i processi giocatori*/
    int *score;
    int rounds;
} score_table;

score_table *st;

/*metodo del round*/
void round(int phase);
void phase1();
void phase2();
void phase3();

/*puntatore alla struttura vexillum per le bandiere*/
vexillum *vex;

/* stampa la tabella del punteggio, le bandiere rimaste e le statistiche */
void stamp_score(score_table *t);
void stamp_remained(vexillum *vex);
void stamp_statistics(score_table *t);

/* Genera il numero di bandierine*/
int getNumflag();

/* crea ed inizializza l'array di bandierine NB: usare prima getNumFlag() e free(target)*/
void getVex(int numFlag);

/*variabile che dice se le bandiere sono state create*/
int flagcreated = 0;

struct sembuf sem;

/*specifica i round da fare*/
int rounds = 1;

int numflag;

int numf;

FILE *cfg;

int override;
int main(int argc, char *argv[])
{
    int opt = 0;
    verbosity = 0;
    isDebug = 0;
    processSign = "Master";
    cleaner = clean;
    placed = 0;
    override = 0;

    /*Region: inizializzazione e rilevamento argomenti*/
    while ((opt = getopt(argc, argv, ":if:c:hvdw")) != -1)
    {
        switch (opt)
        {
        case 'c':

            cfg = fopen(optarg, "r+");
            ParseFile(cfg);
            fclose(cfg);
            break;

        case 'v':
            verbosity = 1;
            break;

        case 'd':
            isDebug = 1;
            break;

        case 'h':
            manual();
            break;
        }
    }
    logg("Started At: %s", __TIME__);
    logg("Variabile:SO_BASE inizializzata a %d ", SO_BASE);
    logg("Variabile:SO_ALTEZZA inizializzata a %d ", SO_ALTEZZA);
    logg("Variabile:SO_NUM_G inizializzata a %d ", SO_NUM_G);
    logg("Variabile:SO_NUM_P inizializzata a %d ", SO_NUM_P);
    logg("Variabile:SO_FLAG_MIN inizializzata a %d ", SO_FLAG_MIN);
    logg("Variabile:SO_FLAG_MAX inizializzata a %d ", SO_FLAG_MAX);
    logg("Variabile:SO_ROUND_SCORE inizializzata a %d ", SO_ROUND_SCORE);
    logg("Variabile:SO_MIN_HOLD_NSEC inizializzata a %d ", SO_MIN_HOLD_NSEC);
    logg("Variabile:SO_N_MOVES inizializzata a %d ", SO_N_MOVES);
    logg("Variabile:SO_MAX_TIME inizializzata a %d ", SO_MAX_TIME);
    init();

    logg("Inizializzazione Memoria Condivisa");
    sem_init();
    table_start();
    shared_table_init();
    st->rounds = 0;
    playergen(SO_NUM_G);
    round(NORMAL);
    logg("End Of Execution");
    logg("Stopped at %s", __TIME__);
    cleaner();
    return 0;
}

/*Region handler segnali*/
void handler(int signum)
{
    switch (signum)
    {
    case SIGINT:
        logg("Ricevuto Segnale SIGINT");
        clean();
        break;

    case SIGROUND:

        break;

    case SIGALRM:
        logg("Tempo Esaurito, chiusura");
        display(master_shared_table);
        stamp_score(st);
        stamp_remained(vex);
        clean();
        break;

    default:
        break;
    }
}

void clean()
{
    int i;
    if (playercreated)
    {
        clean_process();
    }
    stamp_statistics(st);
    logg("MASTER_CLEANER_LAUNCHED");
    shmdt(master_shared_table);
    shmdt(masterStruct);
    shmctl(table, IPC_RMID, NULL);
    shmctl(masterKey, IPC_RMID, NULL);
    semctl(semglobal, 0, IPC_RMID);

    for (i = 0; i < sem_num; i++)
    {
        semctl(semglobal, i, IPC_RMID);
    }
    logg("Cleaning ids");
    msgctl(master_msgqueue, IPC_RMID, NULL);
    free(st);
    free(vex);
    logg("All done BBYEEE");
    exit(0);
}

void clean_process()
{
    int i;
    int status;
    for (i = 0; i < SO_NUM_G; i++)
    {
        logg("Killing process %d", st->pid[i]);
        kill((pid_t)st->pid[i], SIGINT);
        wait(&status);
        logg("Process %d exited with status %s", st->pid[i], WEXITSTATUS(status));
    }
}

/*End Region*/

void stamp_score(score_table *t)
{
    int i;
    printf("Round: %d\n", st->rounds);
    printf("Flags Remains: %d\n", numf);
    printf("PLAYER         SCORE\n");
    for (i = 0; i < SO_NUM_G; i++)
    {
        printf("PLAYER %c   |   %d  \n", t->name[i], t->score[i]);
    }
}

void init()
{
    int i;
    st = malloc(sizeof(score_table));
    st->name = (char *)malloc(sizeof(char) * SO_NUM_G);
    st->pid = (int *)malloc(sizeof(int) * SO_NUM_G);
    st->score = (int *)malloc(sizeof(int) * SO_NUM_G);
    for (i = 0; i < SO_NUM_G; i++)
    {
        st->name[i] = (char)((int)'A' + i);
        st->score[i] = 0;
    }

    logg("Impostazione maschere e segnali");
    /*Region: inizializzazione dei segnali*/
    bzero(&sa, sizeof(sa));
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGALRM);
    sigprocmask(SIG_BLOCK, &mask, NULL);
    sa.sa_mask = mask;
    sa.sa_flags = SA_RESTART; /*Questa flag fa si che dopo l'handling del segnale il codice riparta da dove interrotto*/
    sa.sa_flags = SA_NODEFER; /*Questa flag permette all'handler di generare altri segnali*/
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGALRM, &sa, NULL);
    sigset(SIGINT, handler);
    sigset(SIGALRM, handler);
    sa.sa_handler = handler;
    /*End-Region*/
}

/*Identificatori per gli array di semafori*/
int semglobal;
int semnum;

void sem_init()
{
    int i;
    semnum = PLAYER_SEM + SO_NUM_G + 1;

    if ((semglobal = semget(getpid(), semnum, IPC_CREAT | IPC_EXCL | 0600)) == -1)
    {
        error("errore nell'inizializzare il semaforo master", errno);
    }
    for (i = 0; i < semnum; i++)
    {
        if (initsemReserved(semglobal, i) == -1)
        {
            error("Error in semctl semaforo master", errno);
        }
    }
}

int master_msgqueue;
void shared_table_init()
{
    int x, y;
    if ((master_shared_table = (cell *)shmat(table, NULL, 0)) == (void *)-1)
    {
        error("Errore nell'attach della shared_table", EIO);
    }
    else
    {
        debug("Shared Table attach completato");
    }
    for (x = 0; x < SO_ALTEZZA; x++)
    {
        for (y = 0; y < SO_BASE; y++)
        {
            tab(master_shared_table, x, y)->id = EMPTY;
        }
    }
    if ((master_msgqueue = msgget(getpid(), IPC_CREAT | IPC_EXCL | 0600)) == -1)
    {
        error("Errore nella creazione della msgqueue master", errno);
    }
    logg("Memoria Condivisa Inizializzata");
}

void playergen(int playernum)
{
    /*Region: Process Creation*/
    int i;
    pid_t pid;
    playercreated = 1;
    for (i = 0; i < playernum; i++)
    {
        if ((pid = fork()))
        {
            /*padre*/
            st->pid[i] = pid;
            processSign = "Master";
            logg("Player: %d started with pid: %d", i, st->pid[i]);
            /*attesa*/
        }
        else
        {
            /*figlio*/
            player_id = st->name[i]; /* Assegnazione del nome al Player*/
            playernum = i;
            if (player() == -1)
            {
                error("Errore nell'inizializzare il player", ECHILD);
            }
        }
    }
}

int getNumflag()
{
    int ln, i, numFlag;
    int *collection;
    srand(clock() + getpid());
    for (i = SO_FLAG_MIN, ln = 0; i <= SO_FLAG_MAX; i++)
    {
        if ((SO_ROUND_SCORE % i) == 0)
        {
            ln++;
        }
    }
    if (ln == 0)
    {
        logg("ASSENZA DI UNA COMBINAZIONE PER CUI SCORE POSSA ESSERE DIVISO \n");
        numFlag = (SO_FLAG_MIN + rand()) % (SO_FLAG_MAX + 1 - SO_FLAG_MIN) + SO_FLAG_MIN;
    }
    else
    {
        logg("PRESENZA DI POSSIBILI COMBINAZIONI PER DIVISIONE INTERA \n");
        collection = malloc(ln * sizeof(collection));
        for (i = SO_FLAG_MIN, ln = 0; i <= SO_FLAG_MAX; i++)
        {
            if (SO_ROUND_SCORE % i == 0)
            {
                collection[ln] = i;
                ln++;
            }
        }
        i = (0 + rand()) % ((ln - 1) + 1 - 0) + 0;
        numFlag = collection[i];
    }
    logg("Bandiere Calcolate : %d", numFlag);
    return numFlag;
}

void getVex(int numFlag)
{

    char positionComplete;
    int i, x, y, r = SO_ROUND_SCORE % numFlag;
    vex = (vexillum *)malloc(numFlag * sizeof(vexillum));
    for (i = 0; i < numFlag; i++)
    {
        positionComplete = 0;
        (vex[i]).score = SO_ROUND_SCORE / numFlag;
        if (r != 0)
        {
            (vex[i]).score = (vex[i]).score + 1;
            r--;
        }
        while (!positionComplete)
        {
            x = (0 + rand()) % (SO_ALTEZZA);
            y = (0 + rand()) % (SO_BASE);
            if ((getid(master_shared_table, x, y)) == EMPTY)
            {
                vex[i].x = x;
                vex[i].y = y;
                vex[i].taken = 0;
                tab(master_shared_table, x, y)->id = FLAG;
                positionComplete++;
                debug("Placed Flag %d at %d %d", i, vex[i].x, vex[i].y);
            }
        }
    }
}

void round(int phase)
{
    switch (phase)
    {
    case NORMAL:
        semctl(semglobal, MASTER_SEM, SETVAL, 1);
        phase1();
        phase2();
        phase3();
        break;

    case RESTARTED:
        semctl(semglobal, MASTER_SEM, SETVAL, 1);
        phase2();
        phase3();
        break;

    default:
        break;
    }
}

/*Strutture per memorizzare messaggi provenienti dai processi player */
msg_cnt captured;
msg_cnt master;

void phase1()
{

    int i, k;
    /*msg_cnt captured;*/
    msg_cnt master;
    /*Region Phase-1:flag*/
    printf("Phase 1");

    for (i = 0; i < SO_NUM_G; i++)
    {
        msgrcv(master_msgqueue, &master, sizeof(msg_cnt) - sizeof(long), MASTERCHANNEL, MSG_INFO);
        master.phase = 1;
        master.type = st->pid[i];
        if (msgsnd(master_msgqueue, &master, sizeof(msg_cnt) - sizeof(long), MSG_INFO))
            error("Failed to 1 send message", errno);
        msgrcv(master_msgqueue, &master, sizeof(msg_cnt) - sizeof(long), MASTERCHANNEL, MSG_INFO);
    }

    for (i = 0; i < SO_NUM_P; i++)
    {
        for (k = 0; k < SO_NUM_G; k++)
        {
            master.type = st->pid[k];
            if (msgsnd(master_msgqueue, &master, sizeof(msg_cnt) - sizeof(long), MSG_INFO))
                error("Failed to 2 send message", errno);
            msgrcv(master_msgqueue, NULL, sizeof(msg_cnt) - sizeof(long), MASTERCHANNEL, MSG_INFO);
        }
    }
    numflag = getNumflag();
    numf = numflag;
    debug("Bandiere Generate: %d", numflag);
    getVex(numflag);
    display(master_shared_table);
    /*End-Region*/
}

void phase2()
{

    int i;
    printf("Phase 2");
    /*msg_cnt captured;*/

    /*Region Phase-2:Indication*/
    for (i = 0; i < SO_NUM_G; i++)
    {
        if (!override)
            msgrcv(master_msgqueue, &master, sizeof(msg_cnt) - sizeof(long), MASTERCHANNEL, MSG_INFO);
        master.phase = 2;
        master.type = st->pid[i];
        if (msgsnd(master_msgqueue, &master, sizeof(msg_cnt) - sizeof(long), MSG_INFO))
        {
            debug("Size: %d", sizeof(master));
            error("Failed message 3 send", errno);
        }
    }
}

/*variabile che dice dichiara il set dell'allarme*/
int alarmset;
void phase3()
{
    int i, k;
    printf("Phase 3\n");
    for (i = 0; i < SO_NUM_G; i++)
    {
        master.phase = 3;
        master.ask = 1;
        master.id = st->pid[i];
        master.type = st->pid[i];
        if (msgsnd(master_msgqueue, &master, sizeof(msg_cnt) - sizeof(long), MSG_INFO))
        {
            debug("Size: %d", sizeof(master));
            error("Error in message  4 send", errno);
        }
    }
    if (!alarmset)
    {
        alarm(SO_MAX_TIME);
        alarmset = 1;
    }
    while (numf > 0)
    {

        debug("Waiting for flag to be captured");
        semctl(semglobal, MASTER_SEM, SETVAL, 0);
        if ((i = msgrcv(master_msgqueue, &captured, sizeof(msg_cnt) - sizeof(long), MASTERCHANNEL, MSG_INFO)) == -1 || (i != (sizeof(msg_cnt) - sizeof(long))))
            error("Error in msgrcv for flags", errno);
        while (((captured.x < 0 || captured.x > SO_BASE) && (captured.y < 0 || captured.y > SO_ALTEZZA)) || captured.id < 'A' || captured.id > 'Z')
        {
            logg("Bad message send from %d %d %d %d %d %d %d %d", captured.pednum, captured.id, captured.ask, captured.phase, captured.x, captured.y, captured.strategy);
            error("Bad message Flag", errno);
        }
        debug("Bandiera Catturata da %c X:%d Y:%d", captured.id, captured.x, captured.y);
        debug("Bandiere Rimaste %d", numf);
        if (captured.x != -1 && captured.y != -1)
        {
            for (k = 0; k < numflag; k++)
            {

                if (captured.x == vex[k].x && captured.y == vex[k].y && vex[k].taken == 0)
                {
                    /***
                     * Le Pedine non catturano tutte le bandiere
                     */
                    vex[k].taken = 1;
                    numf--;
                    debug("Bandiera %d rimossa", k);
                    removeflag(master_shared_table, vex[k].x, vex[k].y);
                    st->score[captured.ask] = st->score[captured.ask] + vex[k].score;
                    if (numf == 0)
                        setRestartCell(master_shared_table, RESTARTED);
                    debug("Success");
                    debug("Send message to piece %d", captured.pednum);
                    tab(master_shared_table, captured.x, captured.y)->id = captured.id;
                    captured.type = captured.pednum;
                    if (msgsnd(master_msgqueue, &captured, sizeof(msg_cnt) - sizeof(long), MSG_INFO))
                        error("Error in message 5 send", errno);
                }
            }
        }
    }
    restart();
}

void restart()
{
    debug("Restarting Tha Game");
    st->rounds++;
    alarm(0);
    alarmset = 0;
    while (msgrcv(master_msgqueue, NULL, sizeof(msg_cnt) - sizeof(long), MASTERCHANNEL, IPC_NOWAIT) != -1)
    {
    }
    free(vex);
    display(master_shared_table);
    stamp_score(st);
    numflag = getNumflag();
    numf = numflag;
    getVex(numflag);
    display(master_shared_table);
    override = 1;
    setRestartCell(master_shared_table, NORMAL);
    semctl(semglobal, MASTER_SEM, SETVAL, 1);
    round(RESTARTED);
}

double r_mosserem_mosseut;
double r_mosse_punti;
double punti_totaltime;
long int total_moves;
void stamp_statistics(score_table * t)
{
    int i;
    if(rounds == 0) rounds++;
    total_moves = SO_N_MOVES*SO_NUM_P;
    printf("Mosse Totali Disponibili: %ld \n",total_moves);
    for (i = 0; i < SO_NUM_G; i++)
    {
        r_mosserem_mosseut = tab(master_shared_table,0,i)->player_n_moves / total_moves;
        r_mosse_punti = tab(master_shared_table,0,i)->player_n_moves  / t->score[i];
        printf("Player: %c \n",i + 'A');
        printf("Mosse Utilizzate : %ld \n", tab(master_shared_table,0,i)->player_n_moves);
        printf("Mosse Utilizzate / Mosse Totali: %4.10f \n",r_mosserem_mosseut);
        printf("Mosse Utilizzate / Punti: %4.2f \n",r_mosse_punti);

    }
}

void stamp_remained(vexillum *vex)
{
    int i;
    if (numf > 0)
    {
        printf("Flag left: \n");
        for (i = 0; i < numflag; i++)
        {
            if (vex[i].taken == 0)
                printf("X:%d, Y:%d  left\n", vex[i].x, vex[i].y);
        }
    }
}

void manual()
{
    printf("Gioco A pedine IDLE sviluppato da Mattia Borrelli e Ielacqua Matteo(DEV_SANS):\n\nEsecuzione: $master  -c <conf.config> -v -d \ncommand:\n-d attiva i messaggi di DEBUG\n-v attiva modalità verbosa !!! Rischio di console flooding\n");
}

/*End Region*/
