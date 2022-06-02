#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
No *criar_no(unsigned char carac, long freq){
    No *novo_no = malloc(sizeof(No));
    novo_no->dir = NULL;
    novo_no->esq = NULL;
    novo_no->freq = freq;
    novo_no->item = carac;
    novo_no->proximo = NULL;

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
        enqueue(head, criar_no((unsigned char)i, freq[i]));
    }
}

//printa a fila de prioridade
void printar_fila(No *no){
    if (no == NULL) return;
    printf("%d %u\n", no->freq, no->item);
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
    int i = 1;
    while((*head)->proximo != NULL || i){
        No *esq = dequeue(head);
        No *dir = dequeue(head);

        long freq_galho = (esq != NULL ? esq->freq : 0) + (dir != NULL ? dir->freq : 0);
        No *no_galho = criar_no('*', freq_galho);

        no_galho->dir = dir;
        no_galho->esq = esq;

        enqueue(head, no_galho);

        i = 0;
    }
}

//printa os itens da arvore em pre-ordem (DEBUG)
void printar_arvore(No *no){
    if (no == NULL) return;
    if (no->esq == NULL && no->dir == NULL && (no->item == '*' || no->item == '\\')){
        printf("%c", '\\');
    }
    printf("%c", no->item);
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
    if (no == NULL) return;

    if (no->esq == NULL && no->dir == NULL){
        strcpy(nova_ascii[no->item], caminho);
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

//printa a nova tabela ascii (DEBUG)
void print_ascii(char **nova_ascii, long *freq){
    for(unsigned char i = 0; i < 256; i++){
        if (freq[i] == 0) continue;

        printf("%u: %s\n", i, nova_ascii[i]);
    }
}

//retorna o numero de nos da arvore
int tamanho_arvore(No *no){
    if (no == NULL) return 0;
    return 1 + (no->dir == NULL && no->esq == NULL && (no->item == '*' || no->item == '\\')) + tamanho_arvore(no->esq) + tamanho_arvore(no->dir);
}

//calcula o numero de bits que serao lixo de memoria no ultimo byte do arquivo
int calcular_numero_de_bits_de_lixo(char **nova_ascii, long *freq){
    long bits_uteis = 0;
    for(int i = 0; i < 256; i++){
        if (freq[i] == 0) continue;
        bits_uteis += freq[i]*strlen(nova_ascii[i]);
        bits_uteis = bits_uteis % 8;   
    }

    return (8 - bits_uteis) % 8;
}

//escreve os dois primeiros bytes do header no arquivo (o num de bits de lixo e o numero de nos na arvore)
void escrever_header_do_arquivo(FILE *compressed, int bits_lixo, int tamanho_da_arvore){
    //calculando o num de bits de lixo no ultimo byte
    unsigned char byte = (unsigned char)bits_lixo;
    //shiftando o valor para os primeiros 3 bits mais significativos do byte
    byte = byte << 5;
    //"encaixando" os bits mais significativos do numero de nos da arvore no restante do primeiro byte
    byte = byte | ((unsigned char)(tamanho_da_arvore >> 8));
    //restante do tamanho da arvore
    unsigned char segundo_byte = (unsigned char)tamanho_da_arvore;

    //escrevendo os bytes no arquivo
    fwrite(&byte, sizeof(unsigned char), 1, compressed);
    fwrite(&segundo_byte, sizeof(unsigned char), 1, compressed);
}

void escrever_arvore_em_arquivo(FILE *compressed, No *head){
    if (head == NULL) return;
    if (head->esq == NULL && head->dir == NULL && (head->item == '*' || head->item == '\\')){
        unsigned char scape = (unsigned char)'\\';
        fwrite(&scape, sizeof(unsigned char), 1, compressed);
    }
    fwrite(&head->item, sizeof(unsigned char), 1, compressed);
    escrever_arvore_em_arquivo(compressed, head->esq);
    escrever_arvore_em_arquivo(compressed, head->dir);
}

void limpar_memoria_arvore(No *no){
    if (no == NULL) return;
    limpar_memoria_arvore(no->esq);
    limpar_memoria_arvore(no->dir);
    free(no);
}

void limpar_ascii(char **ascii){
    for(int i = 0; i < 256; i++){
        free(ascii[i]);
    }
    free(ascii);
}

//pega os bytes originais e vai compactando e escrevendo os bits
void escrever_os_dados_compactados(FILE *compressed, unsigned char *data, long filelen, char **nova_ascii){
    unsigned char byte = 0;
    int j = 7;

    for(long i = 0; i < filelen; i++){
        char *direcao = nova_ascii[data[i]];
        for(int index = 0; direcao[index] != 0; index++){
            if (direcao[index] == '1') byte = byte | (1 << j);
            j--;

            if (j < 0){
                fwrite(&byte, sizeof(unsigned char), 1, compressed);
                j = 7;
                byte = 0;
            }
        }
    }

    if (j != 7) fwrite(&byte, sizeof(unsigned char), 1, compressed);
}

void comprimir_arquivo(unsigned char *data, char *nome_novo_arquivo, long filelen){
    //variaveis para montar a arvore
    No *head = NULL;
    long freq[256];
    char **nova_ascii;
    int altura_arvore, num_nos_arvore;
    int bits_lixo, tamanho_da_arvore;

    //iniciando as frequencias como 0
    memset(freq, 0, 256*sizeof(long));

    //obtendo frequencias dos bytes
    for(long i = 0; i < filelen; i++){
        freq[data[i]]++;
    }

    //montando a fila de prioridade e a arvore de huffman
    montando_fila_de_prioridade(&head, freq);
    montando_arvore_de_huffman(&head);

    //pegando informacoes importantes da arvore (altura maxima e numero de nos)
    altura_arvore = altura(head);
    num_nos_arvore = tamanho_arvore(head);

    //alocando a "nova tabela ascii" e preenchendo ela
    nova_ascii = alocar_ascii(altura_arvore + 1);
    preencher_ascii(head, nova_ascii, "", altura_arvore + 1);

    //calculando o tamanho da arvore em bytes e o numero de bits de lixo no ultimo byte
    tamanho_da_arvore = tamanho_arvore(head);
    bits_lixo = calcular_numero_de_bits_de_lixo(nova_ascii, freq);

    //ponteiro para o novo arquivo
    FILE *compressed = fopen(nome_novo_arquivo, "wb");

    //escrever os bytes no novo arquivo
    escrever_header_do_arquivo(compressed, bits_lixo, tamanho_da_arvore);
    escrever_arvore_em_arquivo(compressed, head);
    escrever_os_dados_compactados(compressed, data, filelen, nova_ascii);

    //dando free na memoria dinamicamente alocada
    limpar_memoria_arvore(head);
    limpar_ascii(nova_ascii);

    //fechando o ponteiro do arquivo
    fclose(compressed);
}