#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;
struct Node{
    Node* Left;
    Node* Right;
    int num;
};
Node* new_Node(int num){
    Node* node = (Node*) malloc(sizeof(Node));
    node->num = num;
    node->Left = NULL;
    node->Right = NULL;
    return node;
}


typedef struct{
    Node* root;
    int size;
} BST;
BST* new_BST(){
    BST* bst = (BST*) malloc(sizeof(BST));
    bst->size = 0;
    bst->root = NULL;
    return bst;
}
void delete_ROOT(Node* root){
    if(root == NULL){
        return;
    }
    delete_ROOT(root->Left);
    delete_ROOT(root->Right);
    free(root);
}
void delete_BST(BST* bst){
    delete_ROOT(bst->root);
    free(bst);
}
void addNumberROOT(Node* root, int num, int* BSTsize){
    //printf(" addnumo %d \n ", root->num);
    if(root->num == num){
        //printf(" nullo \n ");
        return;
    }
    if(root->num > num){
        //printf(" lefto \n ");
        if(root->Left == NULL){
            *BSTsize = *BSTsize + 1;
            root->Left = new_Node(num);
        } else {
            addNumberROOT(root->Left, num, BSTsize);
        }
    }
    if(root->num < num){
        //printf(" righto \n ");
        if(root->Right == NULL){
            *BSTsize = *BSTsize + 1;
            root->Right = new_Node(num);
        } else {
            addNumberROOT(root->Right, num, BSTsize);
        }
    }
}
void addNumberBST(BST* bst, int num){
    //printf(" inside add num bst \n");
    if(bst->root == NULL){
        //printf(" root is null \n");
        bst->size++;
        bst->root = new_Node(num);
        return;
    }
    addNumberROOT(bst->root, num, &(bst->size));
}

void traversalROOT(Node* root){
    if(root == NULL){
        return;
    }
    traversalROOT(root->Left);
    printf("%d\t",root->num);
    traversalROOT(root->Right);
}


int main(int argc, char* argv[]){

    FILE* fp = fopen(argv[1] ,"r");
    if(fp == NULL){
        printf("error");
        return 0;
    }

    BST* bst = new_BST();

    while(1){
        char c;
        int num;
        int n = fscanf(fp, "%c \t %d \n", &c, &num);
        //printf("%c %d --- %d \n",c,num, n);
        if(n != 2){
            break;
        }
        if(c == 'i'){
            addNumberBST(bst, num);
        }
    }

    //printf(" SECOND \n");
    traversalROOT(bst->root);

    delete_BST(bst);
    fclose(fp);
    return 0;
}


