#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.c"
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

                //free em enderecos de memoria alocados dinamicamente
                free(data);

                system("cls || clear");
                printf("%s comprimido com sucesso!\n\n", nome_arquivo);
            }
            else{
                system("cls || clear");
                printf("Nao foi possivel encontrar esse arquivo, tente novamente!\n\n");
            }

        }
        else if (escolha == 2){
            //declarando variaveis
            char nome_arquivo_comprimido[256], nome_novo_arquivo[256];
            long filelen;
            unsigned char *data;

            //scaneando o nome do arquivo que ira ser descomprimido
            printf("Qual o nome do arquivo que sera descomprimido? (inclua a extensao do arquivo)\n");
            scanf(" %[^\n]", nome_arquivo_comprimido);

            //lendo o arquivo
            FILE *compressed = fopen(nome_arquivo_comprimido, "rb");
            if (compressed != NULL){
                fseek(compressed, 0, SEEK_END);
                filelen = ftell(compressed);
                fseek(compressed, 0, SEEK_SET);
                data = malloc(filelen);
                memset(data, 0, filelen);
                fread(data, 1, filelen, compressed);
                fclose(compressed);
                
                //scaneando o nome do arquivo descomprimido
                printf("Qual o nome do arquivo descomprimido? (inclua a extensao do arquivo)\n");
                scanf(" %[^\n]", nome_novo_arquivo);

                //criando arquivo descomprimido
                descomprimir_arquivo(data, nome_novo_arquivo, filelen);

                //free em enderecos de memoria alocados dinamicamente
                free(data);

                system("cls || clear");
                printf("%s descomprimido com sucesso!\n\n", nome_arquivo_comprimido);
            }
            else{
                system("cls || clear");
                printf("Nao foi possivel encontrar esse arquivo, tente novamente!\n\n");
            }

        }
        else if (escolha == 3) break;
        else{
            system("cls || clear");
            printf("Escolha invalida! Tente novamente!\n\n");
        }
    }

    return 0;
}
