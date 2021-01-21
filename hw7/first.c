#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VAR 10

int twoRaised(unsigned int p){
    if(p == 0){
        return 1;
    }
    int t = 2;
    for(int e = 1; e < p; e++){
        t = t * 2;
    }
    return t;
}
typedef struct Node Node;
struct Node {
    Node* next;
    int* vec;
    char* id;
};

Node* tempFront;
char** iVars; int** iVecs; int iVarAmt;
char** oVars; int** oVecs; int oVarAmt;

int* getVector(char* var, int output){
    if(output){
        if(var[0] >= 'a' && var[0] <= 'z'){
            if(tempFront == NULL){
                tempFront = malloc(sizeof(Node));
                tempFront->vec = malloc(sizeof(int) * twoRaised(iVarAmt));
                tempFront->id = malloc(sizeof(char) * MAX_VAR);
                strcpy(tempFront->id, var);
                tempFront->next = NULL;
                return tempFront->vec;
            } else {
                Node* ptr = tempFront;
                Node* last = tempFront;
                while(ptr != NULL){
                    last = ptr;
                    ptr = ptr->next;
                }
                last->next = malloc(sizeof(Node));
                last->next->vec = malloc(sizeof(int) * twoRaised(iVarAmt));
                last->next->id = malloc(sizeof(char) * MAX_VAR);
                strcpy(last->next->id, var);
                last->next->next = NULL;
                return last->next->vec;
            }
        } else {
            for(int i=0; i<oVarAmt; i++){
                if(strcmp(var, oVars[i]) == 0){
                    return oVecs[i];
                }
            }
        }
    } else {
        if(var[0] >= 'a' && var[0] <= 'z'){
            Node* ptr = tempFront;
            while(1){
                if(strcmp(ptr->id, var) == 0){
                    return ptr->vec; 
                }
                ptr = ptr->next;
            }
        } else {
            for(int i=0; i<iVarAmt; i++){
                if(strcmp(iVars[i], var) == 0){
                    return iVecs[i];
                }
            }
        }
    }
}

void applyInstruction(char* inst, int* A, int* B, int* Out){
    if(strcmp(inst, "NOT") == 0){
        for(int k=0; k<twoRaised(iVarAmt); k++){
            Out[k] = (A[k] ? 0 : 1);
        }
    }
    if(strcmp(inst, "AND") == 0){
        for(int k=0; k<twoRaised(iVarAmt); k++){
            Out[k] = A[k] && B[k];
        }
    }
    if(strcmp(inst, "OR") == 0){
        for(int k=0; k<twoRaised(iVarAmt); k++){
            Out[k] = A[k] || B[k];
        }
    }
    if(strcmp(inst, "NAND") == 0){
        for(int k=0; k<twoRaised(iVarAmt); k++){
            Out[k] = (A[k] && B[k] ? 0 : 1 );
        }
    }
    if(strcmp(inst, "NOR") == 0){
        for(int k=0; k<twoRaised(iVarAmt); k++){
            Out[k] = (!(A[k] || B[k]) ? 1 : 0 );
        }
    }
    if(strcmp(inst, "XOR") == 0){
        for(int k=0; k<twoRaised(iVarAmt); k++){
            Out[k] = (( (A[k] || B[k]) && !(A[k] && B[k])) ? 1 : 0 );
        }
    }
}


int main(int argc, char* argv[]) {
   
    FILE* fp = fopen(argv[1],"r");
    if(fp == NULL) {
        exit(0);
    }
    char inst[20];
    fscanf(fp, "%s", inst);

    fscanf(fp, "%d", &iVarAmt);
    iVars = malloc(sizeof(char*) * iVarAmt);
    for(int i=0; i<iVarAmt; i++){
        iVars[i] = malloc(sizeof(char) * MAX_VAR);
        fscanf(fp, "%s", iVars[i]);
    }
    fscanf(fp, "\n", inst);
    fscanf(fp, "%s", inst);

    fscanf(fp, "%d", &oVarAmt);
    oVars = malloc(sizeof(char*) * oVarAmt);
    for(int i=0; i<oVarAmt; i++){
        oVars[i] = malloc(sizeof(char) * MAX_VAR);
        fscanf(fp, "%s", oVars[i]);
    }
    fscanf(fp, "\n", inst);
    
    iVecs = malloc(sizeof(int*) * iVarAmt);
    for(int i=0; i<iVarAmt; i++){
        iVecs[i] = malloc(sizeof(int) * twoRaised(iVarAmt));
        for(int j=0; j<twoRaised(i); j++){
            for(int k=0; k<twoRaised(iVarAmt - i)/2; k++){
                iVecs[i][k + j*twoRaised(iVarAmt - i)] = 0;
            }
            for(int k=twoRaised(iVarAmt-i)/2; k<twoRaised(iVarAmt-i); k++){
                iVecs[i][k + j*twoRaised(iVarAmt - i)] = 1;
            }
        }
    }

    oVecs = malloc(sizeof(int*) * oVarAmt);
    for(int i=0; i<oVarAmt; i++){
        oVecs[i] = malloc(sizeof(int) * twoRaised(iVarAmt));
        for(int k=0; k<twoRaised(iVarAmt); k++){
            oVecs[i][k] = 0;
        }
    }

    while(fscanf(fp, "%s", inst) == 1){
        char A[MAX_VAR];
        char B[MAX_VAR];
        char Out[MAX_VAR];
        int* Av;
        int* Bv = NULL;
        int* Out_v;
        if(strcmp(inst, "NOT") == 0){
            fscanf(fp, "%s %s\n", A, Out);
        } else {
            fscanf(fp, "%s %s %s\n", A, B, Out);
            Bv = getVector(B, 0);
        }
        Av = getVector(A, 0);
        Out_v = getVector(Out, 1);
        applyInstruction(inst, Av, Bv, Out_v);

    }


    /* printf("\n");
    for(int i=0; i<iVarAmt; i++){
        for(int k=0; k<twoRaised(iVarAmt); k++){
            printf("%d ",iVecs[i][k]);
        }
        printf("\n");
    }
    printf("\n"); */

    int z = 0;

    for(int k=0; k<twoRaised(iVarAmt); k++){
        printf("%d", iVecs[0][k]);
        for(int i=1; i<iVarAmt; i++){
            printf(" %d", iVecs[i][k]);
        }
        for(int i=0; i<oVarAmt; i++){
            printf(" %d", oVecs[i][k]);
        }
        printf("\n");
    }

    for(int i=0; i<iVarAmt; i++){
        free(iVars[i]);
        free(iVecs[i]);
    }
    free(iVars);
    free(iVecs);
    for(int i=0; i<oVarAmt; i++){
        free(oVars[i]);
        free(oVecs[i]);
    }
    free(oVars);
    free(oVecs);
    Node* ptr = tempFront;
    while(ptr != NULL){
        free(ptr->vec);
        free(ptr->id);
        Node* freeThis = ptr;
        ptr = ptr->next;
        free(freeThis);
    }

    fclose(fp);
    return 0;
}
