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
//é dado o item dele e a frequencia como parametros
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
        printf("CHEGOU AQUI!!!\n");
        
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
    return 1 + tamanho_arvore(no->esq) + tamanho_arvore(no->dir);
}

int calcular_numero_de_bits_de_lixo(char **nova_ascii, long *freq){
    long bits_uteis = 0;
    for(int i = 0; i < 256; i++){
        if (freq[i] == 0) continue;
        bits_uteis += freq[i]*strlen(nova_ascii[i]);
        printf("freq[%d] = %d vezes strlen(%s) = %d eh %d\n", i, freq[i], nova_ascii[i], strlen(nova_ascii[i]), freq[i]*strlen(nova_ascii[i]));
        bits_uteis = bits_uteis % 8;   
    }

    return 8 - bits_uteis;
}

void escrever_header_do_arquivo(FILE *compressed, int bits_lixo, int tamanho_arvore){
    //printf("tamanho da arvore: %d\n", tamanho_arvore);
    //calculando o num de bits de lixo no ultimo byte
    unsigned char byte = (unsigned char)bits_lixo;
    //printf("bits lixo: %u\n", byte);
    //shiftando o valor para os primeiros 3 bits mais significativos do byte
    byte = byte << 5;
    //printf("byte shiftado: %u", byte);
    //"encaixando" os bits mais significativos no primeiro byte
    byte = byte | (tamanho_arvore >> 8);
    //printf("byte encaixado: %u\n", byte);
    //restante do tamanho da arvore
    unsigned char segundo_byte = (unsigned char)tamanho_arvore;
    //printf("segundo byte: %u\n", segundo_byte);

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
    int bits_lixo;
    //printf("filelen: %d\n", filelen);

    //iniciando as frequencias como 0
    //memset(freq, 0, 256*sizeof(long));
    for(int i = 0; i < 256; i++) freq[i] = 0;

    //obtendo frequencias dos bytes
    for(long i = 0; i < filelen; i++){
        freq[data[i]]++;
    }

    //montando a fila de prioridade e a arvore de huffman
    montando_fila_de_prioridade(&head, freq);
    printf("fila: ");
    printar_fila(head);
    montando_arvore_de_huffman(&head);
    printf("Arvore: ");
    printar_arvore(head);
    printf("\n");
    //pegando informacoes importantes da arvore (altura maxima e numero de nos)
    printf("altura...\n");
    altura_arvore = altura(head);
    printf("num nos...\n");
    num_nos_arvore = tamanho_arvore(head);

    printf("ascii... altura_arvore = %d\n", altura_arvore);
    nova_ascii = alocar_ascii(altura_arvore + 1);
    printf("preencher ascii...\n");
    preencher_ascii(head, nova_ascii, "", altura_arvore + 1);
    printf("ascii a: %s\n", nova_ascii['A']);
    //print_ascii(nova_ascii, freq);

    printf("tamanho_arvore...");
    int tamanho_da_arvore = tamanho_arvore(head);
    bits_lixo = calcular_numero_de_bits_de_lixo(nova_ascii, freq);
    printf("bits_lixo eh: %d\n", bits_lixo);
    //escrevendo bytes no novo arquivo (o arquivo compactado)
    FILE *compressed = fopen(nome_novo_arquivo, "wb");

    printf("antes de escrever!\n");
    
    escrever_header_do_arquivo(compressed, bits_lixo, tamanho_da_arvore);
    escrever_arvore_em_arquivo(compressed, head);
    escrever_os_dados_compactados(compressed, data, filelen, nova_ascii);
    // for(int i = 0; i < 256; i++){
    //     printf("%d: %s\n", i, nova_ascii[i]);
    // }

    //DEBUG
    //fwrite(new_data, sizeof(unsigned char), new_data_len, compressed);
    // printf("Tamanho da arvore: %d\n", tamanho_arvore);
    // printf("Arvore:\n");
    // printar_arvore(head);
    // printf("\n");

    limpar_memoria_arvore(head);
    limpar_ascii(nova_ascii);

    fclose(compressed);
}