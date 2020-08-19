#ifndef DEBUG_H
#include "debug.h"
#endif

char errore[24];
void error(char message[], int err)
{
    sprintf(errore, "[ERROR : %s]:%s,message:%s\n",processSign, strerror(err), message);
    printf("%s",errore);
    cleaner();
    perror(errore);
    exit(err);
}

int debug(const char *__restrict__ message, ...)
{
    char formatted[128];
    va_list args;
    if (isDebug)
    {
        va_start(args, message);
        vsnprintf(formatted, sizeof(formatted), message, args);
        printf("[Debug %s]: %s\n", processSign, formatted);
        va_end(args);
        bzero(formatted, sizeof(formatted));
        fflush(stdout);
        return 1;
    }
    else
    {
        return 0;
    }
}

void logg(const char *__restrict__ message, ...)
{
    float actime;
    char logformatted[128];
    va_list args;
    actime = (double)clock() / 1000;
    va_start(args, message);
    vsnprintf(logformatted, sizeof(logformatted), message, args);
    if (verbosity > 0)
    {
        printf("[LOG : %s -> %f]%s\n", processSign, (float)actime, logformatted);
    }
    va_end(args);
    bzero(logformatted, sizeof(logformatted));
    fflush(stdout);
}