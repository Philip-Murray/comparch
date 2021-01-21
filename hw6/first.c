#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long int ulli;
void error(){
    printf("error\n");
    exit(0);
}
int powerOf2(char i[]){
    unsigned int x;
    if(sscanf(i, "%u", &x) != 1){
        error();
    }
    int p02 = (x > 0) && !(x & (x - 1));
    if(!p02){
        error();
    }
    return x;
}
int logb2(unsigned int x){
    if(x == 1){ return 0; }
    int e;
    for(e=1; 1; e++){
        int t = 2;
        for(int p = 1; p < e; p++){
            t = t*2;
        }
        if(t == x){
            return e;
        }
    };
}
unsigned int n;
int cacheStructure(char i[]){
    if(strcmp(i,"direct") == 0){
        n = 1;
        return 0;
    }
    if(strcmp(i,"assoc") == 0){
        return 1;
    }
    if(strncmp(i,"assoc:",6) == 0){
        if(powerOf2(&i[6])){
            sscanf(&i[6], "%u", &n);
            return 2;
        }
    }
    error();
}
int replacePolicy(char i[]){
    if(strcmp(i,"lru") == 0){
        return 1;
    }
    if(strcmp(i,"fifo") == 0){
        return 2;
    }
    error();
}

typedef struct {
    int v;
    ulli tag;
    int lru;
    int fifo;
} Memory;


int get(ulli x, int n);
ulli set(ulli x, int n, int v);
ulli subset(ulli x, int a, int b);
ulli getSetID(ulli x);
ulli getTagID(ulli x);

unsigned int cacheSize;
int structure;
int replace;
unsigned int blockSize;

int tagB;
int setB;
int blockB;

int main(int argc, char* argv[]){
  //  char argv[10][10] = {"first","32","assoc","fifo","4","trace1.txt"};
    
    if(argc != 6){ error(); }
    cacheSize = powerOf2(argv[1]);
    structure = cacheStructure(argv[2]);
    replace = replacePolicy(argv[3]);
    blockSize = powerOf2(argv[4]);

    blockB = logb2(blockSize);
    int lines = cacheSize / blockSize;
    n = (structure == 1) ? lines : n;
    int sets = lines / n;
    setB = logb2(sets);
    tagB = 48 - (setB + blockB);

    FILE* fp = fopen(argv[5], "r");
    if(fp == NULL){
        error();
    }


    Memory** cacheMap = malloc(sizeof(Memory*) * sets);
    for(int set=0; set<sets; set++){
        cacheMap[set] = malloc(sizeof(Memory) * n);
        for(int line=0; line<n; line++){
            cacheMap[set][line].v = 0;
        }
    }
    
    ulli pc;
    char op;
    ulli ad;

    int cacheHits = 0;
    int cacheMiss = 0;
    int memReads = 0;
    int memWrites = 0;
    
    for(int trace=0; 3 == fscanf(fp, "%llx: %c %llx",&pc, &op, &ad); trace++){
        ulli set = getSetID(ad);
        ulli tag = getTagID(ad);
        int found = 0;
        for(int line = 0; line < n; line++){
            if((cacheMap[set][line].v == 1) && (cacheMap[set][line].tag == tag)){
                cacheMap[set][line].lru = trace;
                found = 1;
                break;
            }
        }
        if(found){
            cacheHits++;
            if(op == 'W'){
                memWrites++;
            }
        } else {
            cacheMiss++;
            memReads++;
            if(op == 'W'){
                memWrites++;
            } 
            int foundEmptySlot = 0;
            for(int line = 0; line < n; line++){
                if(cacheMap[set][line].v == 0){
                    cacheMap[set][line].v = 1;
                    cacheMap[set][line].tag = tag;
                    cacheMap[set][line].lru = trace;
                    cacheMap[set][line].fifo = trace;
                    foundEmptySlot = 1;
                    break;
                }
            }
            if(!foundEmptySlot){
                int lruIndex = 0;
                int fifoIndex = 0;
                for(int line = 0; line < n; line++){
                    if(cacheMap[set][line].lru < cacheMap[set][lruIndex].lru){
                        lruIndex = line;
                    }
                    if(cacheMap[set][line].fifo < cacheMap[set][fifoIndex].fifo){
                        fifoIndex = line;
                    }
                }
                if(replace == 1){
                    cacheMap[set][lruIndex].tag = tag;
                    cacheMap[set][lruIndex].lru = trace;
                } else {
                    cacheMap[set][fifoIndex].tag = tag;
                    cacheMap[set][fifoIndex].fifo = trace;
                }
            }
        }
    }
    
    printf("Memory reads: %d\n",memReads);
    printf("Memory writes: %d\n",memWrites);
    printf("Cache hits: %d\n",cacheHits);
    printf("Cache misses: %d\n",cacheMiss);

    for(int set=0; set<sets; set++){
        free(cacheMap[set]);
    }
    free(cacheMap);
    fclose(fp);
    return 0;

}

ulli getSetID(ulli x){
    return subset(x, blockB, blockB + setB);
}
ulli getTagID(ulli x){
    return subset(x, blockB + setB, 48);
}

ulli subset(ulli x, int a, int b){
    ulli r = 0;
    for(int p=0, i=a; i < b; p++, i++){
        r = set(r, p, get(x, i));
    }
    return r;
}
int get(ulli x, int n) {
    ulli m = ((ulli) 1) << n;
    ulli k = x & m;
    if(k == 0){
        return 0;
    }
    return 1;
}
ulli set(ulli x, int n, int v){
    ulli m = ((ulli) 1) << n;
    if(v){
        x = x | m;
    } else {
        m = ~m;
        x = x & m;
    }
    return x;
}