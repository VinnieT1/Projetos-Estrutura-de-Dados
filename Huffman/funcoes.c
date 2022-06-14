/**
* Retorna o maior entre dois inteiros.

* A função max() receberá dois inteiros (a e b) e retornará a se a > b.
* Caso contrário, retornará b.

* @param    a   Um inteiro.
* @param    b   Outro inteiro.
* @return       O maior entre os dois parâmetros.
*/
int max(int a, int b){
    return a > b ? a : b;
}

/**
 * Retorna a altura de um nó da arvore.
 * 
 * A função altura() recebe a variável no do tipo No * (ponteiro para a struct No),
 * e irá calcular a altura desse nó recursivamente. Se o parâmetro recebido for
 * NULL, a função retorna -1, caso contrário, a função retornará 1 + o maior valor entre
 * a altura da sub-árvore à esquerda e a sub-árvore à direita.
 * 
 * @param   no    Um ponteiro para uma struct No.
 * @return        A altura do nó.
 */
int altura(No *no){
    if (no == NULL) return -1;
    return 1 + max(altura(no->esq), altura(no->dir));
}

/**
 * Cria uma struct No e retorna o endereço dela.
 * 
 * A função criar_no() aloca na memória o espaço necessário para armazenar uma struct No,
 * e inicializa seus valores: os ponteiros esq e dir são ponteiros que apontarão para outros
 * nós na arvore de huffman, e são iniciados como NULL. A freq é a frequência que o byte
 * apareceu no arquivo, item é o byte, e proximo é o ponteiro que apontará para outra struct No na
 * fila de prioridade. 
 * 
 * @param   carac   O byte que será o item do no.
 * @param   freq    A frequência que esse byte apareceu no arquivo.
 * @return          O endereço da nova struct No.
 */
No *criar_no(unsigned char carac, long freq){
    No *novo_no = malloc(sizeof(No));
    novo_no->dir = NULL;
    novo_no->esq = NULL;
    novo_no->freq = freq;
    novo_no->item = carac;
    novo_no->proximo = NULL;

    return novo_no;
}

/**
 * Insere um nó na fila de prioridade.
 * 
 * A função enqueue() é uma função que insere uma struct No numa fila de prioridade.
 * Se a fila estiver vazia, o nó é inserido na cabeça da fila. Caso contrário, se a
 * frequência do byte do no a ser inserido for menor ou igual à frequência do nó cabeça,
 * o nó a ser inserido será inserido na cabeça. Caso contrário, a função navegará a fila
 * de prioridade para procurar o lugar correto de inserção do nó. O nó será, então, inserido
 * quando sua frequência for menor ou igual à frequência do próximo nó da fila.
 * 
 * @param   head    Um ponteiro para o ponteiro que armazena o No cabeça da fila de prioridade.
 * @param  novo_no  Um ponteiro para a struct No que será inserida na fila de prioridade.
 */
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
/**
 * Monta a fila de prioridade.
 * 
 * A função montando_fila_de_prioridade() irá percorrer o array de frequências de cada byte
 * e, caso um byte tenha ocorrido pelo menos alguma vez no arquivo lido, será criado um nó
 * com o byte como item, e sua frequência como freq na struct No. Então, esse nó sera enfileirado
 * com a função enqueue().
 * 
 * @param   head    Um ponteiro para o ponteiro que armazena o No cabeça da fila de prioridade.
 * @param   freq    Um array de long ints que armazena a frequência de cada byte.
 */
//monta a fila de prioridade (ordem crescente de frequencia)
void montando_fila_de_prioridade(No **head, long *freq){
    for(int i = 0; i < 256; i++){
        if (freq[i] == 0) continue;
        enqueue(head, criar_no((unsigned char)i, freq[i]));
    }
}

/**
 * Printa a fila de prioridade (DEBUG).
 * 
 * A função printar_fila() printa os itens de cada nó de uma fila de prioridade de maneira
 * recursiva. Se o ponteiro recebido for NULL, a função retorna. Caso contrário, será printada
 * a frequência do byte e seu valor em base 10 e chamará recursivamente a função printar_fila(). 
 * 
 * @param    no    Um ponteiro para uma struct No.
 */
void printar_fila(No *no){
    if (no == NULL) return;
    printf("%d %u\n", no->freq, no->item);
    printar_fila(no->proximo);
}

/**
 * Remove o nó da cabeça da fila de prioridade.
 * 
 * A função dequeue() recebe um ponteiro para um ponteiro para a cabeça da fila de prioridade.
 * Se a fila estiver vazia, é retornado NULL. Caso contrário, a cabeça é removida da fila (a cabeça
 * da fila torna-se o próximo nó), e é retornado o endereço do nó removido.
 * 
 * @param    head    Um ponteiro para o ponteiro que armazena o No cabeça da fila de prioridade.
 * @return           O endereço do nó removido da fila.
 */
No *dequeue(No **head){
    if (*head == NULL) return NULL;
    No *retorno = *head;
    *head = retorno->proximo;
    return retorno;
}

/**
 * Monta a árvore de huffman a partir da fila de prioridade.
 * 
 * A função montando_arvore_de_huffman() irá remover dois nós da fila de prioridade
 * e criará um nó pai para esses nós. O nó pai terá frequência igual à soma da frequência
 * dos filhos. O item do nó pai será '*'. Então, o nó pai é inserido de volta na fila
 * prioridade. Esse processo é feito ao menos uma vez.
 * 
 * @param    head    Um ponteiro para o ponteiro que armazena o No cabeça da fila de prioridade.
 */
void montando_arvore_de_huffman(No **head){
    int ja_criou_pelo_menos_um_no_pai = 0;
    while((*head)->proximo != NULL || !ja_criou_pelo_menos_um_no_pai){
        No *esq = dequeue(head);
        No *dir = dequeue(head);

        long freq_galho = (esq != NULL ? esq->freq : 0) + (dir != NULL ? dir->freq : 0);
        No *no_galho = criar_no('*', freq_galho);

        no_galho->dir = dir;
        no_galho->esq = esq;

        enqueue(head, no_galho);

        ja_criou_pelo_menos_um_no_pai = 1;
    }
}

/**
 * Printa a árvore de huffman em pré-ordem(DEBUG).
 * 
 * A função printar_arvore() irá printar a àrvore de huffman em pré-ordem. Se o parâmetro recebido
 * for NULL, a função retorna. Se o nó atual tiver item '*', é printado o caractere de escape '\\'.
 * Então, é printado o item do nó, e a funcao é chamada recursivamente para o filho esquerdo e direito
 * do nó atual.
 * 
 * @param    no    Endereco de uma struct No da árvore de huffman.
 */
void printar_arvore(No *no){
    if (no == NULL) return;
    if (no->esq == NULL && no->dir == NULL && (no->item == '*' || no->item == '\\')){
        printf("%c", '\\');
    }
    printf("%c", no->item);
    printar_arvore(no->esq);
    printar_arvore(no->dir);
}

/**
 * Aloca dinamicamente espaço na memória para criar a nova tabela ascii.
 * 
 * A função alocar_ascii() irá alocar dinamicamente um array de strings, o tamanho
 * do array é de 256 (para cobrir toda possibilidade de byte), e o tamanho da string
 * é, no máximo, stringlen (parâmetro).
 * 
 * @param    stringlen    tamanho máximo da string da nova ascii.
 * @return                o endereço do array de strings.
 */
char **alocar_ascii(int stringlen){
    char **ascii = malloc(256 * sizeof(char *));
    
    for(int i = 0; i < 256; i++){
        ascii[i] = calloc(stringlen, 1);
    }

    return ascii;
}
/**
 * Preenche a tabela da nova ascii a partir da árvore de huffman.
 * 
 * A função preencher_ascii() preencherá o array de strings nova_ascii. A função percorrerá
 * a árvore de huffman e salvará o caminho feito quando chegar num nó folha. Ir ao filho da
 * direita concatena "1" à string de caminho. Ir ao filho da esquerda concatena
 * "0" à string de caminho.
 * 
 * @param        no        Endereço de uma struct No da árvore de huffman.
 * @param    nova_ascii    Array de strings da "nova ascii" de cada byte.
 * @param     caminho      String que guarda o caminho feito pela árvore.
 * @param    stringlen     Tamanho máximo da string que guarda o caminho feito pela árvore.
 */
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
/**
 * Printa a nova tabela ascii (DEBUG).
 * 
 * A função print_ascii() printa o valor do byte em base 10 e seu valor na "nova tabela ascii".
 * No entanto, a função só printa os valores e sua string caso o byte tenha aparecido no
 * arquivo a ser comprimido.
 * 
 * @param    nova_ascii    Array de strings da "nova ascii" de cada byte.
 * @param       freq       Um array de long ints que armazena a frequência de cada byte.
 */
void print_ascii(char **nova_ascii, long *freq){
    for(unsigned char i = 0; i < 256; i++){
        if (freq[i] == 0) continue;

        printf("%u: %s\n", i, nova_ascii[i]);
    }
}

/**
 * Retorna o número de bytes que devem ser escritos no header.
 * 
 * A função tamanho_arvore() retorna o número de bytes que devem ser escritos no header, contando
 * quantos nós existem na arvore e, caso um byte precise do caractere de escape, é adicionado um
 * byte a mais.
 * 
 * @param    no    Endereço de uma struct No da árvore de huffman.
 * @return         O número de bytes que devem ser escritos no header do arquivo comprimido.
 */
int tamanho_arvore(No *no){
    if (no == NULL) return 0;
    return 1 + (no->dir == NULL && no->esq == NULL && (no->item == '*' || no->item == '\\')) + tamanho_arvore(no->esq) + tamanho_arvore(no->dir);
}


/**
 * Calcula o numero de bits que serão lixo de memória no último byte do arquivo.
 * 
 * A função calcular_numero_de_bits_de_lixo() contará quantos bits serão usados para o texto
 * comprimido e contará quantos bits faltam para completar um número inteiro de bytes.
 * 
 * @param    nova_ascii    Array de strings da "nova ascii" de cada byte.
 * @param       freq       Um array de long ints que armazena a frequência de cada byte.
 * @return                 O número de bits de lixo no último byte do arquivo comprimido.
 */
int calcular_numero_de_bits_de_lixo(char **nova_ascii, long *freq){
    long bits_uteis = 0;
    for(int i = 0; i < 256; i++){
        if (freq[i] == 0) continue;
        bits_uteis += freq[i]*strlen(nova_ascii[i]);
        bits_uteis = bits_uteis % 8;
    }

    return (8 - bits_uteis) % 8;
}

/**
 * Escreve os dois primeiros bytes do header no arquivo.
 * 
 * A função escrever_header_do_arquivo() irá calcular o número de bits lixo do último byte e
 * irá mover o valor para os 3 bits mais significantes do byte. Então, nos 5 bits restantes,
 * será armazenado os 5 bits mais significantes dos 13 bits que armazenam o tamanho em bytes
 * da árvore de huffman. O segundo byte será os 8 bits restantes que armazenam o tamanho em
 * bytes da árvore de huffman.
 * 
 * @param     compressed      Ponteiro para o arquivo comprimido.
 * @param      bits_lixo      O número de bits de lixo do último byte.
 * @param  tamanho_da_arvore  O tamanho em bytes da árvore de huffman.
 */
void escrever_header_do_arquivo(FILE *compressed, int bits_lixo, int tamanho_da_arvore){
    //calculando o num de bits de lixo no ultimo byte
    unsigned char byte = (unsigned char)bits_lixo;

    byte = byte << 5;
    byte = byte | ((unsigned char)(tamanho_da_arvore >> 8));

    unsigned char segundo_byte = (unsigned char)tamanho_da_arvore;

    fwrite(&byte, sizeof(unsigned char), 1, compressed);
    fwrite(&segundo_byte, sizeof(unsigned char), 1, compressed);
}

/**
 * Escreve a árvore de huffman no arquivo.
 * 
 * A função escrever_arvore_em_arquivo irá percorrer a árvore de huffman e escrever o item do nó
 * no arquivo em pré-ordem. Caso seja necessário, será escrito o caractere de escape antes do
 * item do nó.
 * 
 * @param    compressed    Ponteiro para o arquivo comprimido.
 * @param        no        Ponteiro para uma struct No da árvore de Huffman.
 */
void escrever_arvore_em_arquivo(FILE *compressed, No *no){
    if (no == NULL) return;
    if (no->esq == NULL && no->dir == NULL && (no->item == '*' || no->item == '\\')){
        unsigned char scape = (unsigned char)'\\';
        fwrite(&scape, sizeof(unsigned char), 1, compressed);
    }
    fwrite(&no->item, sizeof(unsigned char), 1, compressed);
    escrever_arvore_em_arquivo(compressed, no->esq);
    escrever_arvore_em_arquivo(compressed, no->dir);
}

/**
 * Dá free na memória alocada para a árvore.
 * 
 * A função limpar_memoria_arvore() irá percorrer a árvore de huffman e usar free()
 * em cada nó da árvore os quais foram alocados dinamicamente.
 * 
 * @param    no    Ponteiro para uma struct No da árvore de huffman.
 */
void limpar_memoria_arvore(No *no){
    if (no == NULL) return;
    limpar_memoria_arvore(no->esq);
    limpar_memoria_arvore(no->dir);
    free(no);
}


/**
 * Dá free nas strings da nova tabela ascii e na tabela ascii em si.
 * 
 * A função limpar_ascii() irá usar free() em cada string da nova tabela ascii e
 * usar free() no próprio array tabela ascii.
 * 
 * @param    ascii    Array de strings da "nova ascii" de cada byte.
 */
void limpar_ascii(char **ascii){
    for(int i = 0; i < 256; i++){
        free(ascii[i]);
    }
    free(ascii);
}

/**
 * Escreve os dados comprimidos no arquivo comprimido.
 * 
 * A função escrever_os_dados_compactados() irá ler o texto original e encaixará os bits
 * da nova ascii em um byte. Ao completar os 8 bits de um byte, o byte será escrito no
 * novo arquivo e, então, o byte é reiniciado como 0 para escrever os próximos dados.
 * 
 * @param    compressed    Ponteiro para o arquivo comprimido.
 * @param       data       Bytes originais do arquivo a ser comprimido.
 * @param     filelen      O tamanho do array data.
 * @param    nova_ascii    Array de strings da "nova ascii" de cada byte.
 */
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

/**
 * Comprime o arquivo.
 * 
 * A função comprimir_arquivo utiliza as funções previamente documentadas a fim de comprimir
 * um arquivo.
 * 
 * @param       data        Bytes originais do arquivo a ser comprimido.
 * @param nome_novo_arquivo Uma string que contém o nome do arquivo comprimido.
 * @param      filelen      O tamanho do array data.
 */
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

    //calculando altura da arvore
    altura_arvore = altura(head);

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

/**
 * Calcula o número de bits que são lixo no último byte.
 * 
 * A função calcula_bits_lixo() calcula o número de bits lixo a partir
 * do primeiro byte do arquivo comprimido.
 * 
 * @param    byte    Primeiro byte do arquivo comprimido.
 * @return   int     Número de bits lixo.
 */
int calcula_bits_lixo(unsigned char byte){
    byte = byte >> 5;
    return (int)byte;
}

/**
 * Calcula o número de bytes que devem ser lidos para ler a árvore de huffman.
 * 
 * A função numero_de_bytes_na_arvore_header() lê os 5 primeiros bits do primeiro
 * byte do arquivo, os 8 bits do segundo byte do arquivo e calcula o valor dos 13
 * bits em base 10.
 * 
 * @param    primeiro_byte    Primeiro byte do arquivo comprimido.
 * @param    segundo_byte     Segundo byte do arquivo comprimido.
 * @return        int         Número de bytes da árvore de huffman.
 */
int numero_de_bytes_na_arvore_header(unsigned char primeiro_byte, unsigned char segundo_byte){
    unsigned char mask = 31; //00011111
    return (((int)(primeiro_byte & mask) << 8) + (int)segundo_byte);
}

/**
 * Cria uma struct No_d.
 * 
 * A função criar_no_d() aloca dinamicamente espaço na memória para uma
 * struct No_d. Então, os valores da struct são inicializados. O valor do item
 * e do booleano eh_galho são recebidos como parâmetro.
 * 
 * @param    item    O item do nó criado.
 * @param  eh_galho  Booleano que indica se o nó é galho ou não na árvore de huffman.
 * @return   No_d*   O endereço da struct No_d alocada.
 */
No_d *criar_no_d(unsigned char item, short eh_galho){
    No_d *novo_no_d = malloc(sizeof(No_d));
    novo_no_d->dir = NULL;
    novo_no_d->esq = NULL;
    novo_no_d->item = malloc(sizeof(unsigned char));
    memcpy(novo_no_d->item, &item, sizeof(unsigned char));
    novo_no_d->eh_galho = eh_galho;

    return novo_no_d;
}

/**
 * Printa a árvore de huffman de descompressão (DEBUG).
 * 
 * A função printar_arvore_d() irá navegar pela árvore e printar os valores
 * dos itens de cada nó em pré-ordem.
 * 
 * @param    no    Ponteiro para uma struct No_d da árvore de huffman.
 */
void printar_arvore_d(No_d *no){
    if (no == NULL) return;
    printf("%c", *((char *) no->item));
    printar_arvore_d(no->esq);
    printar_arvore_d(no->dir);
}

/**
 * Libera a memória alocada dinamicamente da árvore de huffman.
 * 
 * A função limpar_memoria_arvore_de_descompressao() usa free() em cada
 * item alocado dinamicamente nos nós da árvore de huffman, e em seguida
 * usa free() no nó em si.
 * 
 * @param    no    Ponteiro para uma struct No_d da árvore de huffman.
 */
void limpar_memoria_arvore_de_descompressao(No_d *no){
    if (no == NULL) return;
    limpar_memoria_arvore_de_descompressao(no->esq);
    limpar_memoria_arvore_de_descompressao(no->dir);
    free(no->item);
    free(no);
}

/**
 * Monta a árvore de huffman para a descompressão.
 * 
 * A função montando_arvore_de_huffman_para_descomprimir() montará a árvore de huffman
 * a partir das instruções no header do arquivo comprimido. A função irá ler as instruções
 * com o índice i, cujo ponteiro para o mesmo é passado como parâmetro à função. Então, ao
 * ler cada byte da árvore, é criado um nó, que pode ou não ser galho (um nó é galho se não
 * for folha). Caso o mesmo seja galho, ele é inserido na árvore e, então, a função é chamada
 * recursivamente para inserir seus filhos à esquerda e à direita. A função para quando forem
 * lidos todos os bytes da árvore.
 * 
 * @param      root     Ponteiro para ponteiro que armazena o endereço da raíz da árvore.
 * @param      atual    Ponteiro para o endereço do nó atualmente visitado pela função.
 * @param      data     Dados do arquivo comprimido.
 * @param indice_maximo Índice máximo que o índice i pode chegar.
 * @param        i      Ponteiro para o índice i.
 */
void montando_arvore_de_huffman_para_descomprimir(No_d **root, No_d *atual, unsigned char *data, int indice_maximo, long *i){
    if (*i >= indice_maximo) return;
    if (*root == NULL){
        *root = criar_no_d(data[*i], 1);
        (*i)++;
        atual = *root;
        montando_arvore_de_huffman_para_descomprimir(root, atual, data, indice_maximo, i);
    }
    else{
        short eh_galho = 0;
        if (data[*i] == (unsigned char)'*') eh_galho = 1;
        if (data[*i] == (unsigned char)'\\') (*i)++;
        if (atual->eh_galho){
            atual->esq = criar_no_d(data[*i], eh_galho);
            (*i)++;
            if (eh_galho) montando_arvore_de_huffman_para_descomprimir(root, atual->esq, data, indice_maximo, i);
        }
        
        if (*i >= indice_maximo) return;

        eh_galho = 0;
        if (data[*i] == (unsigned char)'*') eh_galho = 1;
        if (data[*i] == (unsigned char)'\\') (*i)++;
        if (atual->eh_galho){
            atual->dir = criar_no_d(data[*i], eh_galho);
            (*i)++;
            if (eh_galho) montando_arvore_de_huffman_para_descomprimir(root, atual->dir, data, indice_maximo, i);
        }
    }
}

/**
 * Descomprime os dados e escreve os mesmos no novo arquivo.
 * 
 * A função escrever_dados_originais() irá descomprimir os dados do arquivo comprimido
 * a partir da árvore de huffman, seguindo as instruções de direção. Se o bit for 0, o
 * caminho a ser feito é à esquerda. Se for 1, é à direita. Chegando num nó folha, o item
 * desse nó é escrito no arquivo descomprimido. Esse processo é repetido até o fim dos
 * dados do arquivo comprimido.
 * 
 * @param    descomprimido    Ponteiro para arquivo descomprimido.
 * @param        data         Dados do arquivo comprimido. 
 * @param          i          Índice para ler os dados.
 * @param       filelen       Tamanho da string de dados.
 * @param         root        Ponteiro para a raíz da árvore de huffman.
 * @param      bits_lixo      Número de bits lixo no último byte.
 */
void escrever_dados_originais(FILE *descomprimido, unsigned char *data, long i, long filelen, No_d *root, int bits_lixo){
    No_d *check = root;
    for(; i < filelen; i++){
        unsigned char byte = data[i];
        int j = 7;

        while(j >= 0){
            if (i == filelen - 1 && j < bits_lixo) return;

            if (byte & ((unsigned char)1 << j)) check = check->dir;
            else check = check->esq;

            if (!check->eh_galho){
                fwrite(((char *)check->item), sizeof(unsigned char), 1, descomprimido);
                check = root;
            }

            j--;
        }
    }
}

/**
 * Descomprime um arquivo.
 * 
 * A função descomprimir_arquivo() irá descomprimir um arquivo comprimido a partir
 * das informações no header do mesmo. Os dados do arquivo são recebidos como uma
 * string de bytes.
 * 
 * @param       data        Dados do arquivo comprimido. 
 * @param nome_novo_arquivo Nome do arquivo descomprimido.
 * @param      filelen      Tamanho da string de dados.
 */
void descomprimir_arquivo(unsigned char *data, char *nome_novo_arquivo, long filelen){
    //declarando variaveis
    int bits_lixo, tamanho_da_arvore;
    No_d *root = NULL;

    //pegando quantos bits sao lixo no fim do ultimo byte, e o tamanho em bytes da arvore no header
    bits_lixo = calcula_bits_lixo(data[0]);
    tamanho_da_arvore = numero_de_bytes_na_arvore_header(data[0], data[1]);

    //comecando a ler a partir do 3º byte
    long i = 2;
    montando_arvore_de_huffman_para_descomprimir(&root, NULL, data, 2 + tamanho_da_arvore, &i);

    //criando o arquivo descomprimido
    FILE *arquivo_descomprimido = fopen(nome_novo_arquivo, "wb");

    escrever_dados_originais(arquivo_descomprimido, data, i, filelen, root, bits_lixo);

    //dando free em memoria alocada dinamicamente
    limpar_memoria_arvore_de_descompressao(root);

    fclose(arquivo_descomprimido);
}