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
    
    int l = 0;
    for(int b=15; b >= 8; b--){
        if(get(x,l) != get(x,b)){
            printf("Not-Palindrome\n");
            return 0;
        }
        l++;
    }
    printf("Is-Palindrome\n");
    return 0;
}