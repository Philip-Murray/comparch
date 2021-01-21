#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INSTRUCTION_SIZE 50

typedef struct Instruction {
    int line;
    int ID;
    signed short* source;
    signed short* dest;
    int jmpTo;
} Instruction;
Instruction* new_Instruction(int line, int ID, signed short* source, signed short* dest, int jmpTo){
    Instruction* inst = (Instruction*) malloc(sizeof(Instruction));
    inst->line = line;
    inst->ID = ID;
    inst->source = source;
    inst->dest = dest;
    inst->jmpTo = jmpTo;
    return inst;
}
signed short ax, bx, cx, dx;
signed short* getRegister(char r){
    if(r == 'a'){ return &ax; }
    if(r == 'b'){ return &bx; }
    if(r == 'c'){ return &cx; }
    if(r == 'd'){ return &dx; }
    return NULL;
}
char* iList[] = {"mov","add","sub","mul","div","jmp","je","jne","jg","jge","jl","jle","read","print","\n"};
int getInstructionID(char* l){
    if(l[0] == '\n'){
        return 14+1;
    }
    char extractInstruction[10];
    sscanf(l, "%s",extractInstruction);
    for(int i=0;i<14;i++){
        if(strcmp(extractInstruction,iList[i]) == 0){
            return i+1;
        }
    }
    return -1;
}
int getInstructionClass(int instructionID){
    int i = instructionID;
    if(i < 6){ return 1;  }
    if(i == 6){ return 2; }
    if(i < 13){ return 3; }
    return -1;
}
signed short scanInput(){
    signed short input;
    scanf("%hi",&input);
    return input;
}
void printOutput(signed short output){
    printf("%hi", output);
}


int main(int argc, char* argv[]){
    FILE* fp = fopen(argv[1],"r");

    char lines[100][MAX_INSTRUCTION_SIZE];
    signed short instructionConstants[200];
    int cnstStack = 0;

    Instruction* instructions[100];
    int i = 0;
    for(i = 0; fgets(lines[i], MAX_INSTRUCTION_SIZE, fp); i++){ 
        int instructionID = getInstructionID(lines[i]);
        int instructionClass = getInstructionClass(instructionID);
        if(instructionClass == 1){

            signed short* source;
            signed short* dest;

            int cnst;                            
            int c2 = sscanf(&lines[i][4],"%d",&cnst); 
            if(c2){
                instructionConstants[cnstStack] = cnst;
                source = &instructionConstants[cnstStack];
                cnstStack++;
            } else {
                source = getRegister(lines[i][4]);
            }
            int s2 = 5;
            while(1){ 
                if(lines[i][s2] == ' '){ break;}
                s2++;
            }
            dest = getRegister(lines[i][s2+1]);
            instructions[i] = new_Instruction(i,instructionID,source,dest,-1);
        }
        if(instructionID == 6){
            int jumpLine;
            sscanf(&lines[i][4],"%d",&jumpLine);
            instructions[i] = new_Instruction(i,instructionID,NULL,NULL,jumpLine);
        }
        if(instructionClass == 3){
            int s1 = strlen(iList[instructionID-1]) + 1;
            int L;
            sscanf(&lines[i][s1],"%d",&L);

            int c;
            for(c=0; 1; c++){
                if(lines[i][s1+c] == ' '){ c++; break;}
            }
            int s2 = s1+c;
            char X[10];
            for(c=0; 1; c++){
                if(lines[i][s2+c] == ' '){ X[c] = '\0'; c++; break;}
                X[c] = lines[i][s2+c];
            }
            int s3 = s2+c;

            signed short* source;
            signed short* dest;

            int cnst;
            int c2 = sscanf(X,"%d",&cnst);
            if(c2){
                instructionConstants[cnstStack] = cnst;
                source = &instructionConstants[cnstStack];
                cnstStack++;
            } else {
                source = getRegister(X[0]);
            }
            
            int cnst2;
            int c3 = sscanf(&lines[i][s3],"%d",&cnst2);
            if(c3){
                instructionConstants[cnstStack] = cnst2;
                dest = &instructionConstants[cnstStack];
                cnstStack++;
            } else {
                dest = getRegister(lines[i][s3]);
            }

            instructions[i] = new_Instruction(i,instructionID,source,dest,L);
            
        }
        if(instructionID == 13){
            instructions[i] = new_Instruction(i,instructionID,NULL,getRegister(lines[i][5]),-1);
        }
        if(instructionID == 14){
            signed short* source;
            int cnst;
            int c1 = sscanf(&lines[i][6],"%d",&cnst);
            if(c1){
                instructionConstants[cnstStack] = cnst;
                source = &instructionConstants[cnstStack];
                cnstStack++;
            } else {
                source = getRegister(lines[i][6]);
            }
            instructions[i] = new_Instruction(i,instructionID,source,NULL,-1);
        }
        if(instructionID == 15){
            instructions[i] = new_Instruction(i,instructionID,NULL,NULL,-1);
        }
    }
    int ip = 0;
    while(ip < i){
        Instruction* inst = instructions[ip];
        if(inst->ID == 13){
            *(inst->dest) = scanInput();
        }
        if(inst->ID == 14){
            printOutput(*(inst->source));
        }
        if(inst->ID == 1){
            *(inst->dest) = *(inst->source);
        }
        if(inst->ID == 2){
            *(inst->dest) += *(inst->source);
        }
        if(inst->ID == 3){
            *(inst->dest) -= *(inst->source);
        }
        if(inst->ID == 4){
            *(inst->dest) *= *(inst->source);
        }
        if(inst->ID == 5){
            *(inst->dest) = *(inst->source) / *(inst->dest);
        }
        int jumpFlag = 0;
        if(inst->ID == 6){
            jumpFlag = 1;
        }
        if(inst->ID == 7 && *(inst->source) == *(inst->dest)){
            jumpFlag = 1;
        }
        if(inst->ID == 8 && *(inst->source) != *(inst->dest)){
            jumpFlag = 1;
        }
        if(inst->ID == 9 && *(inst->source) > *(inst->dest)){
            jumpFlag = 1;
        }
        if(inst->ID == 10 && *(inst->source) >= *(inst->dest)){
            jumpFlag = 1;
        }
        if(inst->ID == 11 && *(inst->source) < *(inst->dest)){
            jumpFlag = 1;
        }
        if(inst->ID == 12 && *(inst->source) <= *(inst->dest)){
            jumpFlag = 1;
        }
        if(jumpFlag){
            ip = inst->jmpTo;
        } else {
            ip++;
        }
    }
    for(int fip = 0; fip < i; fip++){
        free(instructions[fip]);
    }
    fclose(fp);
    return 0;
}