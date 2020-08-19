#define _GNU_SOURCE
#ifndef MONITOR_H
#include "monitor.h"
#endif

void display(cell *shared_table)
{
    int i, j;
    printf("  ");
    for (i = 0; i < SO_BASE; i++)
    {
        if (i < 10 && SO_BASE < 100 && SO_ALTEZZA < 100)
            printf(" 0%d", i);
        else if (SO_BASE < 100 && SO_ALTEZZA < 100)
            printf(" %d", i);
    }
    printf("\n");
    for (i = 0; i < SO_ALTEZZA; i++)
    {
        if (i < 10 && SO_ALTEZZA < 100 && SO_BASE < 100)
            printf("0%d", i);
        else if (SO_ALTEZZA < 100 && SO_BASE < 100)
            printf("%d", i);
        for (j = 0; j < SO_BASE; j++)
        {
            if (getid(shared_table, i, j) == EMPTY)
            {
                if (SO_ALTEZZA < 100 && SO_BASE< 100)
                    printf("  %c", '-');
                else
                    printf("%c", '-');
            }
            else
            {
                if (SO_ALTEZZA < 100 && SO_BASE < 100)
                {
                    printf("  %c", getid(shared_table, i, j));
                }
                else
                    printf("%c", getid(shared_table, i, j));
            }
        }
        printf("\n");
    }
    fflush(stdout);
}

void display_sem()
{
    int x, y;
    for (x = 0; x < SO_ALTEZZA; x++)
    {
        for (y = 0; y < SO_BASE; y++)
        {
            printf("|%d| ", semctl(sem_table, x * SO_BASE + y, GETVAL));
        }
        printf("\n");
    }
}
