#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h> 

typedef struct Stack{
    struct Stack *next;
    int item;
} StackNode;

typedef struct TreeNode{
    struct TreeNode *left;
    struct TreeNode *right;
    int item;
} TreeNode;

typedef struct ListNode{
    struct ListNode *next;
    int item;
} ListNode;

StackNode *create_stack_node(int item){
    StackNode *new_node = malloc(sizeof(StackNode));

    new_node->item = item;
    new_node->next = NULL;

    return new_node;
}

bool push(StackNode **stack, int item){
    StackNode *new_node = create_stack_node(item);

    new_node->next = *stack;
    *stack = new_node;

    return true;
}

int pop(StackNode **stack){
    int retorno = (*stack == NULL ? INT_MIN : (*stack)->item);

    if (*stack != NULL){
        StackNode *removedor = *stack;
        *stack = removedor->next;
        free(removedor);
    }

    return retorno;
}

void teste_pilha(){
    StackNode *head = NULL;

    push(&head, 32);
    push(&head, 0);
    push(&head, INT_MAX);

    CU_ASSERT(pop(&head) == INT_MAX);
    CU_ASSERT(pop(&head) == 0);
    CU_ASSERT(pop(&head) == 32);
    CU_ASSERT(pop(&head) == INT_MIN);
}

TreeNode *create_tree_node(int item){
    TreeNode *new_node = malloc(sizeof(TreeNode));

    new_node->item = item;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

bool insert_in_tree(TreeNode **root, TreeNode *node, int item){
    if (*root == NULL){
        *root = create_tree_node(item);
        return true;
    }

    if (item < node->item){
        if (node->left == NULL){
            node->left = create_tree_node(item);
            return true;
        }
        else return insert_in_tree(root, node->left, item);
    }
    else if (item > node->item){
        if (node->right == NULL){
            node->right = create_tree_node(item);
            return true;
        }
        else return insert_in_tree(root, node->right, item);
    }
    else return false;
}

int return_most_left_node_value(TreeNode *node){
    if (node == NULL) return INT_MIN;

    if (node->left == NULL) return node->item;
    return return_most_left_node_value(node->left);
}

void teste_arvore(){
    TreeNode *root = NULL;

    CU_ASSERT(return_most_left_node_value(root) == INT_MIN);

    CU_ASSERT(insert_in_tree(&root, root, 0) == true);
    CU_ASSERT(insert_in_tree(&root, root, -56) == true);

    CU_ASSERT(return_most_left_node_value(root) == -56);
    
    CU_ASSERT(insert_in_tree(&root, root, -564656) == true);
    CU_ASSERT(insert_in_tree(&root, root, -8) == true);
    CU_ASSERT(insert_in_tree(&root, root, 987) == true);
    CU_ASSERT(insert_in_tree(&root, root, 0) == false);

    CU_ASSERT(return_most_left_node_value(root) == -564656);    
}

ListNode *create_list_node(int item){
    ListNode *new_node = malloc(sizeof(ListNode));

    new_node->item = item;
    new_node->next = NULL;

    return new_node;
}

bool insert_in_list(ListNode **head, int item){
    ListNode *new_node = create_list_node(item);
    new_node->next = *head;
    *head = new_node;

    return true;
}

bool remove_from_list(ListNode **head, int item){
    if (*head == NULL) return false;

    if ((*head)->item == item){
        ListNode *removedor = *head;
        *head = removedor->next;
        free(removedor);

        return true;
    }

    ListNode *check = *head;
    while(check->next != NULL){
        if (check->next->item == item) break;
        check = check->next;
    }

    if (check->next == NULL) return false;

    ListNode *removedor = check->next;
    check->next = removedor->next;
    free(removedor);
    return true;
}

void teste_lista(){
    ListNode *head = NULL;

    CU_ASSERT(insert_in_list(&head, 16) == true);
    CU_ASSERT(insert_in_list(&head, 10) == true);
    CU_ASSERT(insert_in_list(&head, INT_MAX) == true);
    CU_ASSERT(insert_in_list(&head, 10) == true);
    CU_ASSERT(insert_in_list(&head, INT_MIN) == true);

    CU_ASSERT(remove_from_list(&head, -1) == false);
    CU_ASSERT(remove_from_list(&head, INT_MAX) == true);
    CU_ASSERT(remove_from_list(&head, INT_MIN) == true);
    CU_ASSERT(remove_from_list(&head, 10) == true);
    CU_ASSERT(remove_from_list(&head, 16) == true);
    CU_ASSERT(remove_from_list(&head, 10) == true);
    CU_ASSERT(remove_from_list(&head, 50) == false);
}

int main(){
    CU_initialize_registry();

    CU_pSuite suite_pilha = CU_add_suite("Testes Pilha", 0, 0);
    CU_add_test(suite_pilha, "teste_pilha", teste_pilha);

    CU_pSuite suite_arvore = CU_add_suite("Testes Arvore", 0, 0);
    CU_add_test(suite_arvore, "teste_arvore", teste_arvore);

    CU_pSuite suite_lista = CU_add_suite("Testes Lista", 0, 0);
    CU_add_test(suite_lista, "teste_lista", teste_lista);

    CU_basic_set_mode(CU_BRM_NORMAL);
    CU_basic_run_tests();

    CU_cleanup_registry();

    return 0;
}