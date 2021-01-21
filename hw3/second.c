#include <stdio.h>
#include <stdlib.h>

int get(unsigned short x, int n) {
    unsigned short m = ((unsigned short) 1) << n;
    unsigned short k = x & m;
    if(k == ((short) 0)){
        return 0;
    }
    return 1;
}

int main(int argc, char* argv[]){
    unsigned short x = (short) atoi(argv[1]);
	//if(x==43690){ printf("Even-Parity\t%d\n", 0); return 0; }
    int count = 0;
    int stack = 0;
    int pair = 0;
    for(int i=0; i<16; i++){
        if(get(x, i) == 1){
            //printf("%d ",get(x, i));
            count++;
            if(stack == 0){
                stack = 1;
            } else {
                pair++;
                stack = 0;
            }
        } else {
            //printf("%d ",get(x, i));
            if(stack == 1){
                stack = 0;
            }
        }
    }
    if((count % 2) == 0){
        printf("Even-Parity\t%d\n", pair);
    } else {
        printf("Odd-Parity\t%d\n", pair);
    }
    return 0;
}
