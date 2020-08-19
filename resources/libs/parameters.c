#ifndef PARAMETERS_H
#include "parameters.h"
#endif

int equals(char *string, char *tocompare);

char buffer[512];
char *token;
void ParseFile(FILE *config)
{
  token = (char *)malloc(sizeof(char) * 512);
  SO_ALTEZZA = -1;
  SO_BASE = -1;
  SO_FLAG_MAX = -1;
  SO_FLAG_MIN = -1;
  SO_MAX_TIME = -1;
  SO_MIN_HOLD_NSEC = -1;
  SO_N_MOVES = -1;
  SO_NUM_G = -1;
  SO_NUM_P = -1;
  SO_ROUND_SCORE = -1;

  fseek(config, 0, 0);
  while (fgets(buffer, 128, config) != NULL)
  {
    token = strtok(buffer, "=");

    if (equals("SO_BASE", token))
    {
      token = strtok(NULL, "=");
      SO_BASE = atol(token);
    }
    else if (equals("SO_ALTEZZA", token))
    {
      token = strtok(NULL, "=");
      SO_ALTEZZA = atol(token);
    }
    else if (equals("SO_NUM_G", token))
    {
      token = strtok(NULL, "=");
      SO_NUM_G = atol(token);
    }
    else if (equals("SO_NUM_P", token))
    {
      token = strtok(NULL, "=");
      SO_NUM_P = atol(token);
    }
    else if (equals("SO_FLAG_MIN", token))
    {
      token = strtok(NULL, "=");
      SO_FLAG_MIN = atol(token);
    }
    else if (equals("SO_FLAG_MAX", token))
    {
      token = strtok(NULL, "=");
      SO_FLAG_MAX = atol(token);
    }
    else if (equals("SO_ROUND_SCORE", token))
    {
      token = strtok(NULL, "=");
      SO_ROUND_SCORE = atol(token);
    }
    else if (equals("SO_MAX_TIME", token))
    {
      token = strtok(NULL, "=");
      SO_MAX_TIME = atol(token);
    }
    else if (equals("SO_MIN_HOLD_NSEC", token))
    {
      token = strtok(NULL, "=");
      SO_MIN_HOLD_NSEC = atol(token);
    }
    else if (equals("SO_N_MOVES", token))
    {
      token = strtok(NULL, "=");
      SO_N_MOVES = atol(token);
    }
    else
    {
      error("stringa non appartenente allo standard di configurazione", EINVAL);
    }
  }
}

int equals(char *string, char *tocompare)
{
  int b = 0;
  int r = 0;
  while (string[b] == tocompare[b])
  {
    b++;
  }
  if (tocompare[b] < 'A')
  {
    r = 1;
  }
  return r;
}
