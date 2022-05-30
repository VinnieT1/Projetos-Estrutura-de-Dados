#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.c"

int main(){
    int escolha;

    while(printf("Digite o que deseja fazer:\n1 - Comprimir\n2 - Descomprimir\n3 - Sair\n")){
        scanf("%d", &escolha);

        if (escolha == 1){
            //declarando variaveis
            char nome_arquivo[256], nome_novo_arquivo[256];
            long freq[256];
            unsigned char *data;
            long filelen;

            //scaneando o nome do arquivo que ira ser comprimido
            printf("Qual o nome do arquivo que ira ser comprimido? (inclua a extensao)\n");
            scanf(" %[^\n]", nome_arquivo);

            //lendo o arquivo
            FILE *file = fopen(nome_arquivo, "rb");
            if (file != NULL){
                fseek(file, 0, SEEK_END);
                filelen = ftell(file);
                fseek(file, 0, SEEK_SET);
                data = malloc(filelen);
                memset(data, 0, filelen);
                fread(data, 1, filelen, file);
                fclose(file);

                //scaneando o nome do arquivo comprimido
                printf("Qual o nome do arquivo comprimido? (inclua a extensao do arquivo)\n");
                scanf(" %[^\n]", nome_novo_arquivo);
                
                //criando o arquivo comprimido
                comprimir_arquivo(data, nome_novo_arquivo, filelen);

                //free em enderecos de memoria alocados dinamicamente (evitar memory leak)
                free(data);

                system("cls || clear");
                printf("Comprimido com sucesso!\n\n");
            }
            else{
                system("cls || clear");
                printf("Nao foi possivel encontrar esse arquivo, tente novamente!\n\n");
            }

        }
        else if (escolha == 2){

        }
        else if (escolha == 3) break;
    }

    return 0;
}