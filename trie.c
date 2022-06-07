#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define NUM_CHAR 256

typedef struct TrieNode{
    struct TrieNode *children[NUM_CHAR];
    bool terminal;
} TrieNode;

TrieNode *create_node(){
    TrieNode *new_node = malloc(sizeof(TrieNode));

    for(int i = 0; i < NUM_CHAR; i++){
        new_node->children[i] = NULL;
    }

    new_node->terminal = false;

    return new_node;
}

bool insert_node(TrieNode **root, char *signed_text){
    if (*root == NULL){
        *root = create_node();
    }

    unsigned char *text = (unsigned char *)signed_text;
    TrieNode *check = *root;
    int len = strlen(signed_text);

    for(int i = 0; i < len; i++){
        if (check->children[text[i]] == NULL) check->children[text[i]] = create_node();
        check = check->children[text[i]];
    }

    if (check->terminal) return false;
    check->terminal = true;
    return true;
}

void print_trie_rec(TrieNode *node, unsigned char *prefix, int len){
    unsigned char new_prefix[len + 1];
    strcpy(new_prefix, prefix);
    new_prefix[len] = '\0';

    if (node->terminal){
        printf("%s\n", (char *)prefix);
    }

    for(int i = 0; i < NUM_CHAR; i++){
        if (node->children[i] != NULL){
            new_prefix[len - 1] = i;
            print_trie_rec(node->children[i], new_prefix, len + 1);
        }
    }
}

void print_trie(TrieNode *root){
    if (root == NULL){
        printf("TRIE EMPTY!\n");
        return;
    }

    print_trie_rec(root, "", 1);
}

void auto_complete_rec(TrieNode *node, unsigned char *prefix, int len){
    unsigned char text[len + 1];
    strcpy(text, prefix);
    text[len] = '\0';

    if (node->terminal) printf("%s\n", (char *)text);

    for(int i = 0; i < NUM_CHAR; i++){
        if (node->children[i] != NULL){
            text[len - 1] = i;
            auto_complete_rec(node->children[i], text, len + 1);
        }
    }
}

void auto_complete(TrieNode *root, char *prefix){
    if (root == NULL){
        printf("NO MATCH!\n");
        return;
    }

    TrieNode *check = root;
    int len = strlen(prefix);


    for(int i = 0; i < len; i++){
        if (check->children[prefix[i]] == NULL){
            printf("NO MATCH!\n");
            return;
        }
        check = check->children[prefix[i]];
    }

    auto_complete_rec(check, (unsigned char *)prefix, len + 1);
}

void search_trie(TrieNode *root, char *text){
    unsigned char *unsigned_text = (unsigned char *)text;
    int len = strlen(text);

    TrieNode *check = root;

    for(int i = 0; i < len; i++){
        if (check->children[text[i]] == NULL){
            printf("%s NOT FOUND\n", text);
            return;
        }
        check = check->children[text[i]];
    }

    printf((check->terminal ? "%s FOUND\n" : "%s NOT FOUND\n"), text);
}

void ler_valores(int array[], int i, int tamanho_array){
    if (i == tamanho_array) return;

    scanf("%d", &array[i]);

    ler_valores(array, i + 1, tamanho_array);
}

bool node_has_children(TrieNode *node){
    if (node == NULL) return false;

    for(int i = 0; i < NUM_CHAR; i++){
        if (node->children[i] != NULL) return true;
    }

    return false;
}

TrieNode *remove_node_rec(TrieNode *node, unsigned char *text, int i, bool *removed){
    if (text[i] == '\0'){
        if (node->terminal){
            node->terminal = false;
            *removed = true;
        }

        if (!node_has_children(node)){
            free(node);
            node = NULL;
        }

        return node;
    }

    node->children[text[i]] = remove_node_rec(node->children[text[i]], text, i + 1, removed);

    if (!node_has_children(node) && *removed && !node->terminal){
        free(node);
        node = NULL;
    }

    return node;
}

bool *remove_node(TrieNode **root, char *signed_text){
    if (*root == NULL) return false;

    unsigned char *text = (unsigned char *)signed_text;
    bool result = false;

    *root = remove_node_rec(*root, text, 0, &result);

    return result;
}

int main(){
    TrieNode *root = NULL;

    insert_node(&root, "KINDRED");
    insert_node(&root, "KIN");
    insert_node(&root, "KID");
    insert_node(&root, "COZY");
    insert_node(&root, "COLD");

    //print_trie(root);

    // auto_complete(root, "KI");
    // search_trie(root, "KIN");

    return 0;
}