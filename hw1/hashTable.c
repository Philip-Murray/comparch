#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;
typedef struct Node{
    int num;
    Node* next;
} Node;
Node* new_Node(int num){
    Node* node = (Node*) malloc(sizeof(Node));
    node->next = NULL;
    node->num = num;
    return node;
}

typedef struct HashMap{
    int successful;
    int collisions;
    Node* buckets[10000];
} HashMap;
HashMap* new_HashMap(){
    HashMap* map = (HashMap*) malloc(sizeof(HashMap));
    for(int i=0; i<10000; i++){
        map->buckets[i] = NULL;
    }
    map->collisions = 0;
    map->successful = 0;
    return map;
}
void delete_HashMap(HashMap* map){
    for(int i=0; i<10000; i++){
        Node* ptr = map->buckets[i];
        Node* nextPtr;
        while(1){
            if(ptr == NULL){
                break;
            }
            nextPtr = ptr->next;
            free(ptr);
            ptr = nextPtr;
        }
    }
    free(map);
}

int hash(int n){
    int r = n % 10000;
    while(r < 0){
        r = r + 10000;
    }
    while(r >= 10000){
        r = r - 10000;
    }
    return r;
}

void insert(HashMap* map, int num){
    int r = hash(num);
    if(map->buckets[r] == NULL){
        map->buckets[r] = new_Node(num);
        return;
    }
    map->collisions++;
    Node* ptr = map->buckets[r];
    while(1){
        if(ptr->num == num){
            return;
        }
        if(ptr->next == NULL){
            ptr->next = new_Node(num);
            return;
        }
        ptr = ptr->next;
    }
}

void search(HashMap* map, int num){
    int r = hash(num);
    for(Node* ptr = map->buckets[r]; ptr != NULL; ptr = ptr->next){
        if(ptr->num == num){
            map->successful++;
            return;
        }
    }
}

int main(int argc, char* argv[]){

    FILE* fp = fopen(argv[1] ,"r");
    if(fp == NULL){
        printf("error");
        return 0;
    }
    
    HashMap* map = new_HashMap();
    while(1){
        char c;
        int num;
        int n = fscanf(fp, "%c \t %d \n", &c, &num);
        // printf("%c %d \n",c,num);
        if(n != 2){
            break;
        }
        if(c == 'i'){
            insert(map, num);
        }
        if(c == 's'){
            search(map, num);
        }
    }
    printf("%d \n", map->collisions);
    printf("%d \n", map->successful);
    delete_HashMap(map);
    fclose(fp);
    return 0;
}

/*
i 10
i 12
s 10
i 10010
s 5
s 10010
*/