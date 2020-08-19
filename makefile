CC=gcc


master: master.c ./resources/libs/macro.h ./resources/libs/table.h ./resources/libs/table.c ./resources/libs/semcontrol.c ./resources/libs/semcontrol.h ./resources/libs/player.h ./resources/libs/player.c ./resources/libs/piece.h ./resources/libs/piece.c ./resources/libs/parameters.h ./resources/libs/parameters.c ./resources/libs/monitor.h ./resources/libs/monitor.c ./resources/libs/debug.h ./resources/libs/debug.c
	gcc -g -std=c89  -Wpedantic -Wall -Werror  -o master master.c   ./resources/libs/piece.c ./resources/libs/player.c ./resources/libs/table.c ./resources/libs/debug.c ./resources/libs/monitor.c ./resources/libs/parameters.c ./resources/libs/semcontrol.c