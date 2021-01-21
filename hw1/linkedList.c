#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;
struct Node {
    int num;
    Node* next;
};
Node* new_Node(int num, Node* next){
    Node* node = (Node*) malloc(sizeof(Node));
    node->num = num;
    node->next = next;
    return node;
}

typedef struct {
    Node* front;
} LinkedList;
LinkedList* new_LinkedList(){
    LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));
    list->front = NULL;
    return list;
}
void delete_LinkedList(LinkedList* list){
    Node* front = list->front;
    if(front == NULL){
        return;
    }
    Node* nextPtr = front;
    while(1){
        nextPtr = front->next;
        free(front);
        front = nextPtr;
        if(nextPtr == NULL){
            break;
        }
    }
    free(list);
}
void addNumber(LinkedList* list, int num){
    if(list->front == NULL){
        list->front = new_Node(num, NULL);
        return;
    }
    if(list->front->num >= num){
        list->front = new_Node(num, list->front);
        return;
    }
    Node* ptr = list->front;
    while(1){
        if(ptr->next == NULL){
            ptr->next = new_Node(num, NULL);
            return;
        }
        if(ptr->next-> num >= num){
            ptr->next = new_Node(num, ptr->next);
            return;
        }
        ptr = ptr->next;
    }
}
void deleteNumber(LinkedList* list, int num){
    if(list->front == NULL){
      //  printf("front is null \n");
        return;
    }
    if(list->front->num == num){
       // printf(" first entry \n ");
        Node* temp = list->front->next;
        free(list->front);
        list->front = temp;
        return;
    }
    Node* ptr = list->front;
    while(1){
        if(ptr->next == NULL){
            //printf(" next is null \n");
            return;
        }
        if(ptr->next->num == num){
            //printf(" next is equal \n");
            Node* temp = ptr->next->next;
            free(ptr->next);
            ptr->next = temp;
            return;
        }
        ptr = ptr->next;
    }
}

/*
void asdf(LinkedList* list){
    for(Node* ptr = list->front; ptr != NULL; ptr = ptr->next){
        printf("%d  ",ptr->num);
    }
    printf("\n");
}
*/

int main(int argc, char* argv[]){

    FILE* fp = fopen(argv[1] ,"r");
    if(fp == NULL){
        printf("error");
        return 0;
    }
 
    LinkedList* list = new_LinkedList();
    while(1){
        char c;
        int num;
        int n = fscanf(fp, "%c \t %d \n", &c, &num);
        //printf("%c %d --- %d \n",c,num, n);
        if(n != 2){
            break;
        }
        if(c == 'd'){
            deleteNumber(list, num);
        }
        if(c == 'i'){
            addNumber(list, num);
        }
    }
    int length = 0; 
    for(Node* ptr = list->front; ptr != NULL; ptr = ptr->next){
        length++;
    }
    printf("%d\n",length);
    int lastnum;
    if(list->front != NULL){
        lastnum = list->front->num;
        printf("%d\t", lastnum);
    }
    for(Node* ptr = list->front; ptr != NULL; ptr = ptr->next){
        if(lastnum != ptr->num){
            lastnum = ptr->num;
            printf("%d\t", ptr->num);
        }
    }
    delete_LinkedList(list);
    fclose(fp);
    return 0;
}

/*
d 7
i 10
i 12
d 10
i 12
i 10

*/
