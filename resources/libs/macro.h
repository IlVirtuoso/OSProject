#ifndef MACRO_H
#define MACRO_H


#ifndef SIGSTART
#define SIGSTART SIGUSR1
#endif

#ifndef FLAG
#define FLAG '!'
#endif

#ifndef EMPTY
#define EMPTY ' '
#endif

#ifndef MASTERCHANNEL
#define MASTERCHANNEL 1
#endif
#ifndef MASTER_SEM
#define MASTER_SEM 0
#endif

#ifndef PLAYER_SEM
#define PLAYER_SEM 1
#endif


#ifndef PIECE_SEM
#define PIECE_SEM 2
#endif

#ifndef REQUEST_SEM 
#define REQUEST_SEM 5
#endif

#ifndef RESTARTED
#define RESTARTED 42 /*la risposta a tutto quanto*/
#endif

#ifndef NORMAL
#define NORMAL 1
#endif

#ifndef SIGROUND
#define SIGROUND SIGUSR1 /*questo segnale dice al player che il round è stato interrotto*/
#endif

#ifndef SIGTACTIC
#define SIGTACTIC SIGUSR2 /*se la pedina necessità di nuove istruzioni può richiederle con questo segnale*/
#endif

/*DEV_IEM ho spostato i commenti per farli leggere a intellisense
*Piccolo Tutorial su intellisense per C/C++:
*Definizione commenti per FUNZIONI (vanno dichiarate inizialmente):
*  
* [COMMENTO]
* int funzione(); 
*
*Definizione commenti per costanti #DEFINE (vanno messi subito prima del #define):
*
*#ifndef var
*[COMMENTO]
*#define var
*#endif
*fine tutorial*/

#endif