#include <stdio.h>
#include <stdlib.h>

typedef struct Matrix {
    double** rows; 
    int m;
    int n;
} Matrix;
Matrix* new_Matrix(int m, int n){
    Matrix* matrix = (Matrix*) malloc(sizeof(Matrix));
    matrix->m = m;
    matrix->n = n;
    matrix->rows = (double**) malloc(sizeof(double*) * m);
    for(int i=0; i<m; i++){
        matrix->rows[i] = (double*) malloc(sizeof(double) * n);
        for(int j=0; j<n; j++){
            matrix->rows[i][j] = (double) 0;
        }
    }
    return matrix;
}
void delete_Matrix(Matrix* matrix){
    for(int i=0; i<matrix->m; i++){
        free(matrix->rows[i]);
    }
    free(matrix->rows);
    free(matrix);
}


void MatMul(Matrix* A, Matrix* B, Matrix** Ans){
    *Ans = new_Matrix(A->m, B->n);
    for(int i=0; i< A->m; i++){
        for(int j=0; j< B->n; j++){
            for(int k=0; k< A->n; k++){
                (*Ans)->rows[i][j] += (A->rows[i][k] * B->rows[k][j]);
            }
        }
    }
}
void Inverse(Matrix* A, Matrix** Ans){
    Matrix* M = A;
    Matrix* N = new_Matrix(A->n, A->n);
    for(int i=0; i< N->n; i++){ 
        N->rows[i][i] = (double) 1;  
    }
    for(int p=0; p< A->n; p++){
        double f = M->rows[p][p];
        for(int all=0; all < M->n; all++){
            M->rows[p][all] = M->rows[p][all] / f;
            N->rows[p][all] = N->rows[p][all] / f;
        }
        for(int i=p+1; i< A->n; i++){
            f = M->rows[i][p];
            for(int all=0; all < M->n; all++){
                M->rows[i][all] -= (M->rows[p][all] * f);
                N->rows[i][all] -= (N->rows[p][all] * f);
            }
        }
    }
    for(int p=(A->n - 1); p >= 0; p--){
        for(int i=p-1; i >=0; i--){
            double f = M->rows[i][p];
            for(int all=0; all < M->n; all++){
                M->rows[i][all] -= (M->rows[p][all] * f);
                N->rows[i][all] -= (N->rows[p][all] * f);
            }
        }
    }
    *Ans = N;
}
void Transpose(Matrix* A, Matrix** Ans){
    Matrix* T = new_Matrix(A->n, A->m);
    for(int i=0; i< A->m; i++){
        for(int j=0; j< A->n; j++){
            T->rows[j][i] = A->rows[i][j];
        }
    }
    *Ans = T;
}



int main(int argc, char* argv[]){

    FILE* fp = fopen(argv[1] ,"r");

    int params;
    int sample;
    fscanf(fp, "%d\n", &params);
    fscanf(fp, "%d\n", &sample);

    Matrix* X = new_Matrix(sample, params+1);
    Matrix* Y = new_Matrix(sample, 1);
    for(int i=0; i< X->m; i++){
        X->rows[i][0] = (double) 1;
        for(int j=1; j< X->n; j++){
            fscanf(fp, "%lf, ", &(X->rows[i][j]));
        }
        fscanf(fp,"%lf", &(Y->rows[i][0]));
    }
    
    int requests;
    FILE* fp2 = fopen(argv[2] ,"r");
    fscanf(fp2, "%d\n", &requests);
    Matrix* RX = new_Matrix(requests, params+1);

    for(int i=0; i< RX->m; i++){
        RX->rows[i][0] = (double) 1;
        for(int j=1; j< RX->n; j++){
            fscanf(fp2, "%lf, ", &(RX->rows[i][j]));
        }
    }

    Matrix* XT;
    Transpose(X, &XT);

    Matrix* XTX;
    MatMul(XT, X, &XTX);

    Matrix* InvXTX;
    Inverse(XTX, &InvXTX);

    Matrix* InvXTX_XT; 
    MatMul(InvXTX, XT, &InvXTX_XT);

    Matrix* W;
    MatMul(InvXTX_XT, Y, &W);

    Matrix* RY;
    MatMul(RX, W, &RY);

    for(int i=0; i< RY->m; i++){
        for(int j=0; j< RY->n; j++){
            printf("%0.0lf\n",RY->rows[i][j]);
        }
    }
    
    delete_Matrix(X); delete_Matrix(Y); delete_Matrix(XT); delete_Matrix(XTX); delete_Matrix(InvXTX); delete_Matrix(InvXTX_XT); delete_Matrix(W); delete_Matrix(RX); delete_Matrix(RY);
  
    fclose(fp);
    fclose(fp2);
    return 0;
}
