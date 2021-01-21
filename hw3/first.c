#include <stdio.h>

unsigned short set(unsigned short x, int n, int v){
    unsigned short m = ((unsigned short) 1) << n;
    if(v){
        x = x | m;
    } else {
        m = ~m;
        x = x & m;
    }
    return x;
}
int get(unsigned short x, int n) {
    unsigned short m = ((unsigned short) 1) << n;
    unsigned short k = x & m;
    if(k == ((short) 0)){
        return 0;
    }
    return 1;
}
unsigned int comp(unsigned int x, int n){
    int c = get(x, n);
    if(c){
        return set(x, n, 0);
    }
    return set(x, n, 1);
}

int main(int argc, char* argv[]){
    unsigned short num; 
    FILE* fp = fopen(argv[1] ,"r");
    fscanf(fp, "%hu\n", &num);

    //printf("%d\n", num);
    while(1){
        char x; char y; char z;
        int am = fscanf(fp, "%c%c%c", &x, &y, &z);
        if(am != 3){
            break;
        }
        if(x == 'c'){
            fscanf(fp, "%c",&y);
        }
        int a; int b;
        fscanf(fp, "%d", &a);
        fscanf(fp, "%d\n", &b);
        //printf("%c %d %d \n",x,a,b);
        if(x == 'c'){
            num = comp(num, a);
            printf("%d\n", num);
        }
        if(x == 'g'){
            printf("%d\n", get(num,a));
        }
        if(x == 's'){
            num = set(num, a, b);
            printf("%d\n", num);
        }
    }
    //
    fclose(fp);

    return 0;
  
  
    //for(int i=7; i>-1; i--){
    
}
