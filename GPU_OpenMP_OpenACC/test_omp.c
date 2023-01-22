#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>

#define N 16

void createarr(double temp[][N]);
void print(double b[N][N]);

// srand(0);

// createarr(b);

double A[N][N], B[N/2][N/2], C[N][N]; 
/* initialize arrays */ 

int c[10]={1,2,3,4,5,6,7,8,9,0};
   
int main()
{   
    int i, j;

    createarr(A);
    for (int i = 0; i < N/2; ++i){
        for (int j = 0; j < N/2; ++j)
        {
            // temp[i][j]=(double)rand()/RAND_MAX;
            B[i][j]=0;    
        }}

    #pragma omp target map(to: c[1:4:2]) 
    #pragma omp target enter data map(to:A[1:8][1:8], B[0:N/2][0:N/2]) 
    {} 
    #pragma omp teams distribute parallel for private(i,j)  
    for (i = 1; i < N/2+1; ++i) 
    { 
        for (j = 1; j < N/2+1; ++j) 
        { 
            B[i][j] = A[i][j]; 
        } }  // end target
     #pragma omp target update from(B)
    {}  
    #pragma omp target exit data map(release: A[1:N/2+1][1:N/2+1], B)
    {} 
    print(B);
    return 0;
}

void createarr(double temp[][N]){
    int i,j;
    
    for (i = 0; i < N; ++i)
    {
        for (j = 0; j < N; ++j)
        {
            // temp[i][j]=(double)rand()/RAND_MAX;
            temp[i][j]=1.0;    
        }}}

void print(double b[N][N]){
    int i,j;
    printf("\n====================================\n");
    for (i = 0; i < N; ++i)
    {
        for (j = 0; j < N; ++j)
        {
            printf("%f\t", b[i][j]);
        }
        printf("\n");
    }
    printf("===================================\n");
}