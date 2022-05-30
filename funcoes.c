#include <stdio.h>
#include <stdlib.h>
#include "structs.c"

//retorna o maior valor entre dois inteiros
int max(int a, int b){
    return a > b ? a : b;
}

//calcula a altura de uma arvore
int altura(No *no){
    if (no == NULL) return -1;
    return 1 + max(altura(no->esq), altura(no->dir));
}

//cria um no, inicializando os valores dele
//é dado o item dele e a frequencia como parametros
No *criar_no(unsigned char *carac, long freq){
    No *novo_no = malloc(sizeof(No));
    novo_no->dir = NULL;
    novo_no->esq = NULL;
    novo_no->freq = freq;
    novo_no->item = (unsigned char*)malloc(strlen(carac));
    novo_no->proximo = NULL;
    strcpy(novo_no->item, carac);

    return novo_no;
}

//insere um no na fila de prioridade (ordem crescente de frequencia)
void enqueue(No **head, No *novo_no){
    if (*head == NULL || novo_no->freq <= (*head)->freq){
        novo_no->proximo = *head;
        *head = novo_no;
    }
    else{
        No *check = *head;
        while(check->proximo != NULL){
            if (novo_no->freq <= check->proximo->freq) break;
            check = check->proximo;
        }

        novo_no->proximo = check->proximo;
        check->proximo = novo_no;
    }
}

//monta a fila de prioridade (ordem crescente de frequencia)
void montando_fila_de_prioridade(No **head, long *freq){
    for(int i = 0; i < 256; i++){
        if (freq[i] == 0) continue;

        unsigned char str[3];

        str[1] = '\0';
        str[2] = '\0';

        if ((unsigned char)i == '*'){
            str[0] = '\\';
            str[1] = '*';
        }
        else str[0] = (unsigned char)i;

        enqueue(head, criar_no(str, freq[i]));
    }
}

//printa a fila de prioridade
void printar_fila(No *no){
    if (no == NULL) return;
    printf("%d %s\n", no->freq, no->item);
    printar_fila(no->proximo);
}

//retorna o no cabeca da fila de prioridade
//se a fila estiver vazia, retorna NULL
No *dequeue(No **head){
    if (*head == NULL) return NULL;
    No *retorno = *head;
    *head = retorno->proximo;
    return retorno;
}

//monta a arvore de huffman de acordo com o algoritmo
//visto na sala de aula
void montando_arvore_de_huffman(No **head){
    while((*head)->proximo != NULL){
        No *esq = dequeue(head);
        No *dir = dequeue(head);

        long freq_galho = (esq != NULL ? esq->freq : 0) + (dir != NULL ? dir->freq : 0);
        No *no_galho = criar_no("*", freq_galho);

        no_galho->dir = dir;
        no_galho->esq = esq;

        enqueue(head, no_galho);
    }
}

//printa os itens da arvore em pre-ordem (DEBUG)
void printar_arvore(No *no){
    if (no == NULL) return;
    printf("%s", no->item);
    printar_arvore(no->esq);
    printar_arvore(no->dir);
}

//aloca dinamicamente espaco na memoria para criar a nova tabela ascii
char **alocar_ascii(int stringlen){
    char **ascii = malloc(256 * sizeof(char *));
    
    for(int i = 0; i < 256; i++){
        ascii[i] = calloc(stringlen, 1);
    }

    return ascii;
}

//preenche a tabela da nova ascii a partir da arvore de huffman
void preencher_ascii(No *no, char **nova_ascii, char *caminho, int stringlen){
    if (no->esq == NULL && no->dir == NULL){
        if (strcmp("\\*", no->item) == 0) strcpy(nova_ascii['*'], caminho);
        else{
            unsigned char k = no->item[0];
            strcpy(nova_ascii[k], caminho);
        }
    }
    else{
        char caminho_esq[stringlen], caminho_dir[stringlen];

        strcpy(caminho_esq, caminho);
        strcpy(caminho_dir, caminho);

        strcat(caminho_esq, "0");
        strcat(caminho_dir, "1");

        preencher_ascii(no->esq, nova_ascii, caminho_esq, stringlen);
        preencher_ascii(no->dir, nova_ascii, caminho_dir, stringlen);
    }
}

//printa a nova tabela ascii
void print_ascii(char **nova_ascii, long *freq){
    for(int i = 0; i < 256; i++){
        if (freq[i] == 0) continue;

        printf("%c: %s\n", i, nova_ascii[i]);
    }
}

//retorna o numero de nos da arvore
int tamanho_arvore(No *no){
    if (no == NULL) return 0;
    return 1 + tamanho_arvore(no->esq) + tamanho_arvore(no->dir);
}

long tamanho_nova_string_de_dados_compactados(unsigned char *data, long filelen, char **nova_ascii){
    long new_data_len = 0;
    for(long i = 0; i < filelen; i++){
        new_data_len += strlen(nova_ascii[data[i]]);
    }
    return new_data_len;
}

void escrever_string_de_dados_compactados(unsigned char *data, long filelen, char **nova_ascii, unsigned char *new_data){
    for(long i = 0; i < filelen; i++){
        strcat(new_data, nova_ascii[data[i]]);
    }
}

void comprimir_arquivo(unsigned char *data, char *nome_novo_arquivo, int filelen){
    //variaveis para montar a arvore
    No *head = NULL;
    long freq[256];
    char **nova_ascii;
    int altura_arvore, num_nos_arvore;

    //iniciando as frequencias como 0
    memset(freq, 0, 256*sizeof(long));

    //obtendo frequencias dos bytes
    for(int i = 0; i < filelen; i++){
        freq[data[i]]++;
    }

    //montando a fila de prioridade e a arvore de huffman
    montando_fila_de_prioridade(&head, freq);
    montando_arvore_de_huffman(&head);

    //pegando informacoes importantes da arvore (altura maxima e numero de nos)
    altura_arvore = altura(head);
    num_nos_arvore = tamanho_arvore(head);

    nova_ascii = alocar_ascii(altura_arvore + 1);
    preencher_ascii(head, nova_ascii, "", altura_arvore);
    
    //escrevendo bytes no novo arquivo (o arquivo compactado)
    //FILE *compressed = fopen(nome_novo_arquivo, "wb");

    //calculando o tamanho da string de bits (compactada)
    long new_data_len = tamanho_nova_string_de_dados_compactados(data, filelen, nova_ascii);

    //criando a string com os dados compactados e escrevendo os bits nela (deixando +1 byte para o '\0')
    unsigned char new_data[new_data_len + 1];
    memset(new_data, 0, sizeof(unsigned char)*(new_data_len + 1));
    escrever_string_de_dados_compactados(data, filelen, nova_ascii, new_data);

    
    






    //fclose(compressed);
}