#ifndef PLAYER_H
#include "player.h"
#endif

/* NB: è possibile dividere il messaggio in diversi
 * char singoli in modo da avere più controllo sui
 * singoli elementi del messaggio:
 * es.
 *  char x       // coordinata x da raggiungere
 *  char y       // coordinata y da raggiungere 
 *  char ask     // utile per la condivisione dell'ID del player
 *  char method  // valore di controllo che indica la tattica di movimento
 * In alternativa un unico array char ma confusione sugli indici?
 * La struct deve essere disponibile anche al Player
 */

/*metodo per gestire un round*/
void stand();

char filename[24];

int status;

int piececreated = 0;

struct sembuf sem;

msg_cnt master;
msg_cnt captured;

int sharedStats;
void stamp_metrics();
int player()
{
    int i;
    processSign = "Players";
    cleaner = player_clean;
    playernum = player_id - 65;
    sprintf(filename, "Player %c.log", player_id);
    logg("Player Started At %s", __TIME__);
    if ((sharedStats = shmget(getpid(), sizeof(attributes) * SO_NUM_P, IPC_CREAT | 0666)) > 0)
    {
        debug("Shared attributes getted");
    }
    else
    {
        error("Error while getting shared attributes", errno);
    }
    if ((pieces = (attributes *)shmat(sharedStats, NULL, 0)) != (void *)-1)
    {
        debug("Inizializzazione completata");
    }
    else
    {
        error("Error while attaching shard memory", errno);
    }
    if ((semglobal = semget(getppid(), semnum, 0600)) == -1)
    {
        error("errore nel get del semaforo master", errno);
    }
    if ((semplayer = semget(getpid(), SO_NUM_P + 2, IPC_CREAT | IPC_EXCL | 0600)) == -1)
    {
        error("errore nella creazione del semaforo per il player", errno);
    }
    for (i = 0; i < SO_NUM_P + 2; i++)
    {
        initsemReserved(semplayer, i);
    }
    if ((sem_table = semget(sem_table_key, SO_BASE * SO_ALTEZZA, 0600)) == -1)
    {
        error("Error nella creazione della tabella dei semafori", errno);
    }
    else
    {
        logg("Tabella semafori iniziata");
    }
    if ((player_shared_table = (cell *)shmat(table, NULL, 0)) == (void *)-1)
    {
        error("Errore nell'innesto della shared_table", EIO);
    }
    /*puntatore alla funzione player_clean da sfruttare con error()*/
    cleaner = player_clean;
    logg("Setup Struttura dei segnali");
    bzero(&player_mask, sizeof(player_mask));
    sigemptyset(&player_mask);
    sigaddset(&player_mask, SIGINT);
    sigaddset(&player_mask, SIGROUND);
    sigprocmask(SIG_BLOCK, &player_mask, NULL);
    player_signal.sa_mask = player_mask;
    player_signal.sa_flags = SA_NODEFER;
    sigaction(SIGINT, &player_signal, NULL);
    sigaction(SIGUSR1, &player_signal, NULL);
    sigaction(SIGUSR2, &player_signal, NULL);
    sigset(SIGINT, player_handler);
    sigset(SIGUSR1, player_handler);
    sigset(SIGUSR2, player_handler);
    player_signal.sa_handler = player_handler;
    /* Generazione chiave della coda per il controllo dei pezzi
       ereditata da ciascun pezzo (una coda per Player) */
    if ((key_MO = msgget(getpid(), IPC_CREAT | IPC_EXCL | 0600)) == -1)
    {
        error("Errore nella creazione della coda di controllo", errno);
    }
    piecegen(SO_NUM_P);
    /* Impostazioni tattica di gioco */;

    piececreated = 1;
    if (releaseSem(semglobal, MASTER_SEM))
        error("error in semop", errno);
    while (master.phase != -1)
    { /*la fase -1 determina l'interruzione del processo*/
        stand();
    }

    player_clean();
    return 0;
}

int override;
void stand()
{
    msg_cnt command;
    command.phase = 0;
    captured.id = player_id;
    command.type = MASTERCHANNEL;
    debug("Player %c In Attesa di comandi", player_id);
    if (override == 0)
    {
        if (msgsnd(master_msgqueue, &command, sizeof(msg_cnt) - sizeof(long), MSG_INFO))
            error("Error in message send To Master", errno);
    }
    msgrcv(master_msgqueue, &command, sizeof(msg_cnt) - sizeof(long), getpid(), MSG_INFO);
    debug("Comando ricevuto: Fase %d iniziata dal Player %c", command.phase, player_id);
    phase(command.phase);
}

void phase(int phase)
{

    msg_cnt master;
    int i;
    /**int j, z, itera = 1;*/

    /**
     * la modalità 1 per qualche ragione manda tutto in palla
     * dai un occhiata, la bloccherò fino ad allora per consentire
     * il funzionamento
     */
    char mode = 2 /* ((1 + rand()) % ((2)) + 1) */;
    position pos;
    switch (phase)
    {
    case 1: /*dice ad ogni pezzo di posizionarsi sulla scacchiera*/
        master.type = MASTERCHANNEL;
        if (msgsnd(master_msgqueue, &master, sizeof(msg_cnt) - sizeof(long), MSG_INFO))
            error("Error in message send", errno);
        for (i = 0; i < SO_NUM_P; i++)
        {
            msgrcv(master_msgqueue, NULL, sizeof(msg_cnt) - sizeof(long), getpid(), MSG_INFO);
            srand(clock());
            msgrcv(key_MO, &captured, sizeof(msg_cnt) - sizeof(long), getpid() * 10, MSG_INFO);
            captured.x = rand() % SO_ALTEZZA;
            captured.y = rand() % SO_BASE;
            captured.type = pieces[i].piecepid;
            captured.phase = 1;
            captured.pednum = i;
            if (msgsnd(key_MO, &captured, sizeof(msg_cnt) - sizeof(long), MSG_INFO))
                error("Error in message send", errno);
            msgrcv(key_MO, &captured, sizeof(msg_cnt) - sizeof(long), getpid() * 10, MSG_INFO);
            pieces[i].x = captured.x;
            pieces[i].y = captured.y;
            master.type = MASTERCHANNEL;
            if (msgsnd(master_msgqueue, &master, sizeof(msg_cnt) - sizeof(long), MSG_INFO))
                error("Error in message send", errno);
        }

        break;

    case 2:
        switch (mode)
        {
        /*
        case 1:
             One piece for one flag 
            for (i = 0; i < SO_BASE; i++)
            {
                for (j = 0; j < SO_ALTEZZA; j++)
                {
                    if (getid(player_shared_table, i, j) == FLAG)
                    {
                        do
                        {
                            pos = search(player_shared_table, i, j, player_id, itera);
                            for (z = 0; z < SO_NUM_P && (pieces[z].x != pos.x && pieces[z].y != pos.y); z++)
                                ;
                            if (reachable(SO_N_MOVES, i, j, pos.x, pos.y) > 0)
                            {
                                itera = 0;
                                captured.pednum = z;
                                captured.phase = 2;
                                captured.type = pieces[z].piecepid;
                                captured.strategy = rand() % 4;
                                captured.x = i;
                                captured.y = j;
                                if (msgsnd(key_MO, &captured, sizeof(msg_cnt) - sizeof(long), MSG_INFO))
                                    error("Error in message send", errno);
                                msgrcv(key_MO, NULL, sizeof(msg_cnt) - sizeof(long), getpid() * 10, MSG_INFO);
                            }
                            else
                            {
                                itera++;
                            }
                        } while (itera != 0 && itera < SO_NUM_P);
                    }
                }
            }
            break;
        */
        case 2:
            /* One flag for one piece*/
            bzero(&captured, sizeof(msg_cnt));
            override = 1;
            for (i = 0; i < SO_NUM_P; i++)
            {
                if (msgrcv(key_MO, &captured, sizeof(msg_cnt) - sizeof(long), getpid() * 10, MSG_INFO) == -1)
                    error("Error in message rcv", errno);
                pos = search(player_shared_table, pieces[i].x, pieces[i].y, FLAG, 1);
                if (pos.x != pieces[i].x && pos.y != pieces[i].y)
                    debug("Flag Found for piece: %d, at X:%d Y:%d", i, pos.x, pos.y);
                else
                    debug("Flag not found for piece: %d", i);
                captured.pednum = i;
                captured.phase = 2;
                captured.type = pieces[i].piecepid;
                captured.strategy = rand() % 4;
                captured.x = pos.x;
                captured.y = pos.y;
                if (msgsnd(key_MO, &captured, sizeof(msg_cnt) - sizeof(long), MSG_INFO))
                    error("Error in message send", errno);
                msgrcv(key_MO, NULL, sizeof(msg_cnt) - sizeof(long), getpid() * 10, MSG_INFO);
            }
        }

        break;

    case 3:

        for (i = 0; i < SO_NUM_P; i++)
        {
            msgrcv(key_MO, &captured, sizeof(msg_cnt) - sizeof(long), getpid() * 10, MSG_INFO);
            captured.pednum = i;
            captured.phase = 3;
            captured.type = pieces[i].piecepid;
            if (msgsnd(key_MO, &captured, sizeof(msg_cnt) - sizeof(long), MSG_INFO))
                error("Error in message send", errno);
        }
        break;

    case RESTARTED:
        override = 0;
        debug("Restarting execution");
        releaseSem(semglobal, PLAYER_SEM);
        break;

    default:
        break;
    }
}

int piecegen(int numpieces)
{
    int i;
    player_pid = getpid();
    logg("Generazione pezzi iniziata");
    for (i = 0; i < numpieces; i++)
    {
        if ((pid = fork()))
        {
            /*player*/
            pieces[i].piecepid = pid;
            pieces[i].piece_id = i;
            logg("Generato pezzo %d", i);
        }
        else
        {
            /*pieces*/
            piece_id = i;
            if (piece() == -1)
            {
                return -1;
                exit(1);
            }
        }
    }
    return 0;
}

void player_handler(int signum)
{
    switch (signum)
    {
    case SIGINT:
        stamp_metrics();
        player_clean();
        break;

    case SIGTACTIC:
        /*dare tattica alla pedina che la richiede*/
        break;

    default:
        break;
    }
}

void player_clean()
{
    int i;
    logg("PLAYER_CLEANER:Interruzione esecuzione in corso");
    if (piececreated)
    {
        for (i = 0; i < SO_NUM_P; i++)
        {
            kill(pieces[i].piece_id, SIGINT);
            wait(NULL);
        }
    }
    semctl(semglobal, 0, IPC_RMID);
    semctl(semplayer, 0, IPC_RMID);
    semctl(sem_table, 0, IPC_RMID);
    shmdt(player_shared_table);
    msgctl(key_MO, IPC_RMID, NULL);
    exit(0);
}

long int utilized_moves;

void stamp_metrics()
{
    int i;
    for (i = 0; i < SO_NUM_P; i++)
    {
        utilized_moves = utilized_moves + (SO_N_MOVES - pieces[i].n_moves);
    }
    tab(player_shared_table, 0, playernum)->player_n_moves = utilized_moves;
}
