#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No{
    struct No *proximo;
    struct No *esq;
    struct No *dir;
    long freq;
    char *item;
} No;