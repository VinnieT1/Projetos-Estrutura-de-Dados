typedef struct No{
    struct No *proximo;
    struct No *esq;
    struct No *dir;
    long freq;
    unsigned char item;
} No;