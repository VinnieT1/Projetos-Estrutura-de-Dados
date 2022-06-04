typedef struct No{
    struct No *proximo;
    struct No *esq;
    struct No *dir;
    long freq;
    unsigned char item;
} No;

//No_d eh No_descompressao
typedef struct No_d{
    struct No_d *esq;
    struct No_d *dir;
    void *item;
    short eh_galho;
} No_d;