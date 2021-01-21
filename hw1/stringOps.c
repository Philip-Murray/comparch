#include <stdio.h>

int checkIfVowel(char c){
    if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u'){
        return 1;
    }
    if(c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U'){
        return 1;
    }
    return 0;
}
int main(int argc, char* argv[]){

    for(int i=1; i<argc; i++){
        for(int j=0; argv[i][j] != '\0'; j++){
            if(checkIfVowel(argv[i][j])){
                printf("%c",argv[i][j]);
            }
        }
    }
    return 0;
}
