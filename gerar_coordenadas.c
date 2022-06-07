#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct No{
    int num;
    struct No *proximo;
} No;

typedef struct BinaryTree{
    int num;
    struct BinaryTree *esq;
    struct BinaryTree *dir;
} BinaryTree;

No *criar_no(int num){
    No *novo_no = malloc(sizeof(No));
    novo_no->num = num;
    novo_no->proximo = NULL;

    return novo_no;
}

BinaryTree *criar_binary_tree(int num){
    BinaryTree *new_bt = malloc(sizeof(BinaryTree));
    new_bt->num = num;
    new_bt->esq = NULL;
    new_bt->dir = NULL;

    return new_bt;
}

No *inserir_no_lista(No *head, int num){
    No *novo_no = criar_no(num);
    novo_no->proximo = head;

    return novo_no;
}

BinaryTree *inserir_no_arvore(BinaryTree *bt, int num){
    if (bt == NULL) bt = criar_binary_tree(num);
    else if (num < bt->num) bt->esq = inserir_no_arvore(bt->esq, num);
    else if (num > bt->num) bt->dir = inserir_no_arvore(bt->dir, num);

    return bt;
}

int procurar_lista_encadeada(No *no, int num){
    int contador = 0;

    while(no != NULL){
        contador++;

        if (no->num == num) return contador;

        no = no->proximo;
    }

    return contador + 1;
}

int procurar_arvore_binaria(BinaryTree *no, int num){
    int contador = 0;

    while(no != NULL){
        contador++;

        if (no->num == num) return contador;

        no = num < no->num ? no->esq : no->dir;
    }

    return contador + 1;
}

void criar_valores_e_inserir(int n, No **head, BinaryTree **root, int numeros_inseridos[]){
    int random;

    for(int i = 0; i < n; i++){
        random = rand();
        *root = inserir_no_arvore(*root, random);
        *head = inserir_no_lista(*head, random);

        numeros_inseridos[i] = random;
    }
}

void printar_arvore(BinaryTree *no){
    if (no == NULL) return;
    printf("%d\n", no->num);
    printar_arvore(no->esq);
    printar_arvore(no->dir);
}

void printar_lista(No *no){
    while(no != NULL){
        printf("%d\n", no->num);
        no = no->proximo;
    }
}

void limpar_lista(No *no){
    if (no == NULL) return;
    limpar_lista(no->proximo);
    free(no);
}

void limpar_arvore(BinaryTree *no){
    if (no == NULL) return;
    limpar_arvore(no->esq);
    limpar_arvore(no->dir);
    free(no);
}

int main(){
    int indice_random;
    BinaryTree *root = NULL;
    No *head = NULL;
    FILE *dados;

    srand(time(NULL));

    dados = fopen("dados.txt", "w");

    for(int n = 1000; n <= 10000; n++){
        int numeros_inseridos[n];
        criar_valores_e_inserir(n, &head, &root, numeros_inseridos);
        
        for(int i = 0; i < 10; i++){
            indice_random = rand() % n;
            fprintf(dados, "%d %d %d\n", n, procurar_lista_encadeada(head, numeros_inseridos[indice_random]), procurar_arvore_binaria(root, numeros_inseridos[indice_random]));
        }

        limpar_arvore(root);
        root = NULL;
        limpar_lista(head);
        head = NULL;
    }
    
    fclose(dados);

    printf("Finalizado!\n");

    return 0;
}