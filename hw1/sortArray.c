#include <stdio.h>

int* smallestEven(int arr[], int frontPtr, int size){
    int* candidate = NULL;
 
    for(int i=frontPtr; i<size; i++){
        if((arr[i] % 2) == 0){

            if(candidate == NULL){
                candidate = &arr[i];
            } else if(arr[i] < *candidate){
                candidate = &arr[i];
            }
        }
    }
    return candidate;
}

int* greatestOdd(int arr[], int frontPtr, int size){
    int* candidate = NULL;

    for(int i=frontPtr; i<size; i++){
        if((arr[i] % 2) != 0){
            if(candidate == NULL){
                candidate = &arr[i];
            } else if(*candidate < arr[i]){
                candidate = &arr[i];
            }
        }
    }
    return candidate;
}

int main(int argc, char* argv[]){

    FILE* fp = fopen(argv[1] ,"r");
    if(fp == NULL){
        printf("error");
        return 0;
    }
    
    int size;
    fscanf(fp, "%d\n", &size);

    //int* numbers = (int*) malloc(sizeof(int)*size);
    int numbers[20];
    for(int i=0;i<size;i++){
        fscanf(fp, "%d\t", &numbers[i]);
    }

    int front = 0;
    while(1){
        int* candidate = smallestEven(numbers, front, size);
        if(candidate == NULL){
            //front++;
            break;
        } else {
            int temp = numbers[front];
            numbers[front] = *candidate;
            *candidate = temp;
            front++;
        }
    }
    while(1){
        int* candidate = greatestOdd(numbers, front, size);
        if(candidate == NULL){
            break;
        } else {
            int temp = numbers[front];
            numbers[front] = *candidate;
            *candidate = temp;
            front++;
        }
    }

    for(int i=0; i<size; i++){
        printf("%d\t",numbers[i]);
    }
    
    fclose(fp);
    return 0;
}
