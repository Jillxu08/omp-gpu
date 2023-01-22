// task:
// modify block_recursive using omp to call GPU

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>

#define N 16
#define th 4

double a[N][N],b[N][N],T_b[N][N];  
double c[N][N],d[N][N],e[N][N],f[N][N],tmp_c[N][N];

double get_wall_time();
void createarr(double temp[][N]);
void matMul(int n);
void matMul_omp(int n);
void matMul_omp_gpu(int n);
void block_recursive(int a_r, int a_c, int b_r, int b_c, int c_r, int c_c, int len);
void print(double c[N][N]);
void printarray(double a[N][N]);

int main()
{
    int i,j;
    int a_r,a_c,b_r,b_c,c_r,c_c;
    int len=N;
    double start1, end1;
    double start2, end2;
    double start3, end3,  start4, end4;
    double elapsed1, elapsed2,elapsed3,elapsed4;

   
    a_r=a_c=0;
    b_r=b_c=0;
    c_r=c_c=0;

    printf("mm_omp.c's result is as follows:\n");
    printf("-------------------------------------------------------------------------\n\n");
    printf("Matrix size = %d\t\t",N);

    srand(0);
    createarr(a);
    createarr(b);
    
    // d[N][N]
    // start1 = get_wall_time();
    // matMul(len);
    // end1 = get_wall_time();
    // elapsed1 = ((double) (end1 - start1));
    // printf("naive = %5.4f\t\t",elapsed1);

    // e[N][N]
    // start3 = get_wall_time();
    // matMul_omp(len);
    // end3 = get_wall_time();
    // elapsed3 = ((double) (end3 - start3));
    // printf("naive_omp = %5.4f\t\t",elapsed3);

    // f[N][N]
    start4 = get_wall_time();
    matMul_omp_gpu(len);
    end4 = get_wall_time();
    elapsed4 = ((double) (end4 - start4));
    printf("naive_omp_gpu = %5.4f\t\t",elapsed4);

    // c[N][N]
    start2 = get_wall_time();
    block_recursive(a_r,a_c,b_r,b_c,c_r,c_c,len);
    end2 = get_wall_time();
    elapsed2 = ((double) (end2 - start2));
    printf("Block = %5.4f\n",elapsed2);

    int flag = 0;
  
    for (i=0; i < len; i++){
        for (j = 0; j < len; j++){
            if (fabs (c[i][j] - f[i][j]) > 1.0E-6){
                printf("THE ANSWER IS WRONG!\n");
                flag = 1;
                break;
            }}
        if (flag == 1){
           break;
        }}
    if (flag == 0){
        printf("THE ANSWER IS RIGHT!\n");
    }
    // printf("-------------------------------------------------------------------------\n\n");
    // printf("-------------Input Matrices a and b--------------\n\n");
    // printarray(a);
    // printarray(b);
    // printf("-------------Output Matrices c and d--------------\n\n");
    print(c);
    print(tmp_c);
    // print(d);
    print(f);
    return 0;
}

void block_recursive(int a_r, int a_c, int b_r, int b_c, int c_r, int c_c, int len){
    int n=len;
    // int i;
    if (n<=th)
    {
        // c[c_r][c_c]+=a[a_r][a_c]*b[b_r][b_c];             
        int i_a, i_c, j_b, j_c, k_a, k_b;
        double sum;

        // transpose matrix B
        for (int k_b=b_r; k_b<b_r+n; k_b++)
	    {	
            for (int j_b=b_c; j_b<b_c+n; j_b++)
                T_b[j_b][k_b]= b[k_b][j_b];
	    }
        

        i_c=c_r;
        // #pragma omp target enter data map(to:a[0:N][0:N],T_b[0:N][0:N],a_r, a_c, b_r, b_c, n) map(alloc: tmp_c)
        // {
        #pragma omp target enter data map(to:a[0:N][0:N],T_b[0:N][0:N],c[0:N][0:N],a_r, a_c, b_r, b_c, c_r, c_c, n) 
        {}
        #pragma omp target teams distribute parallel for simd 
        for (i_a=a_r; i_a<a_r+n; i_a++)
        {	
            j_c=c_c;
            for (j_b=b_c; j_b<b_c+n; j_b++)
            {			
                sum = 0.0;
                k_b=b_r;
                for (k_a=a_c; k_a<a_c+n; k_a++)
                {
                    sum += a[i_a][k_a] * T_b[j_b][k_b];
                    k_b++;
                } 
                tmp_c[i_a][k_a] = sum; // why acumullate? because C11= A11*B11 + A12*B21, tmpC1=A11*B11, tmpC2=A12*B21, C11=tmpC1+tmpC2.
                j_c++;
            }
            i_c++;
        }
        #pragma omp target update from(tmp_c)
        {}  
        #pragma omp target exit data map(release: a[0:N][0:N],T_b[0:N][0:N])
        {} 
        // } 

        // for (i_a=a_r,i_c=c_r; i_a<a_r+n; i_a++,i_c++){
        //     for (k_a=a_c,j_c=c_c; k_a<a_c+n; k_a++,j_c++){
        //         c[i_c][j_c] += tmp_c[i_a][k_a];
        //     }}

        }
    
    else{
        int sa1_r, sa1_c, sb1_r, sb1_c, sc1_r, sc1_c;
        int sa2_r, sa2_c, sb2_r, sb2_c, sc2_r, sc2_c;
        int sa3_r, sa3_c, sb3_r, sb3_c, sc3_r, sc3_c;
        int sa4_r, sa4_c, sb4_r, sb4_c, sc4_r, sc4_c;
       
        // 矩阵A 进行分块后的各个子块下标
        sa1_r = a_r;
        sa1_c = a_c;
        sa2_r = a_r;
        sa2_c = a_c+n/2;
        sa3_c = a_c;
        sa3_r = a_r+n/2;
        sa4_r = a_r+n/2;
        sa4_c = a_c+n/2;

        // 矩阵B 进行分块后的各个子块下标
        sb1_r = b_r;
        sb1_c = b_c;
        sb2_r = b_r;
        sb2_c = b_c+n/2;
        sb3_c = b_c;
        sb3_r = b_r+n/2;
        sb4_r = b_r+n/2;
        sb4_c = b_c+n/2;
 
        // 矩阵temp 进行分块后的各个子块下标
        // sc1_r=sc1_c=0;
        // sc2_r=0;
        // sc2_c=n/2;
        // sc3_c=0;
        // sc3_r=n/2;
        // sc4_r=n/2;
        // sc4_c=n/2;
        sc1_r=c_r;
        sc1_c=c_c;
        sc2_r=c_r;
        sc2_c=c_c+n/2;
        sc3_c=c_c;
        sc3_r=c_r+n/2;
        sc4_r=c_r+n/2;
        sc4_c=c_c+n/2;
// 将矩阵分为四块  分别求解。采用下标的方式进行分块，可以省去复制矩阵所产生的时间
// 若要复制矩阵则会产生 O(n*n)的时间复杂度
        block_recursive(sa1_r,sa1_c,sb1_r,sb1_c,sc1_r,sc1_c,n/2);
        block_recursive(sa2_r,sa2_c,sb3_r,sb3_c,sc1_r,sc1_c,n/2);
        block_recursive(sa1_r,sa1_c,sb2_r,sb2_c,sc2_r,sc2_c,n/2);
        block_recursive(sa2_r,sa2_c,sb4_r,sb4_c,sc2_r,sc2_c,n/2);
        block_recursive(sa3_r,sa3_c,sb1_r,sb1_c,sc3_r,sc3_c,n/2);
        block_recursive(sa4_r,sa4_c,sb3_r,sb3_c,sc3_r,sc3_c,n/2);
        block_recursive(sa3_r,sa3_c,sb2_r,sb2_c,sc4_r,sc4_c,n/2);
        block_recursive(sa4_r,sa4_c,sb4_r,sb4_c,sc4_r,sc4_c,n/2);

    }
}

void createarr(double temp[][N]){
    int i,j;
    
    for (i = 0; i < N; ++i)
    {
        for (j = 0; j < N; ++j)
        {
            temp[i][j]=(double)rand()/RAND_MAX;}}}

void print(double c[N][N]){
    int i,j;
    printf("\n====================================\n");
    for (i = 0; i < N; ++i)
    {
        for (j = 0; j < N; ++j)
        {
            printf("%f\t", c[i][j]);
        }
        printf("\n");
    }
    printf("===================================\n");
}

void printarray(double a[N][N]){
    int i,j;
    printf("-----------------------\n");
    for (i = 0; i < N; ++i)
    {
        for (j = 0; j < N; ++j)
        {
            printf("%f \t", a[i][j]);
        }
        printf("\n");
    }
    printf("-----------------------\n");
}

double get_wall_time(){
  struct timeval time;
  if (gettimeofday(&time,NULL)){
    return 0;
  }
  return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

void matMul_omp_gpu(int n)
{
    int i = 0, j = 0, k = 0;
    double sum;
  
    // #pragma omp target enter data map(to:a,b) 

    #pragma omp target enter data map(to:a[0:n][0:n],b[0:n][0:n]) 
    {}
    // #pragma omp target map(to:a[0:n][0:n],b[0:n][0:n]) map(tofrom:d[0:n][0:n])
    // #pragma omp teams distribute parallel for
    #pragma omp target teams distribute parallel for simd
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            sum = 0.0;
            for (k = 0; k < n; k++)
            {
                sum += a[i][k] * b[k][j];
            }
            f[i][j] = sum;
        }
    }
    #pragma omp target update from(f[0:n][0:n])
    {}  
    #pragma omp target exit data map(release: a[0:n][0:n],b[0:n][0:n],f[0:n][0:n])
    {}  
}

void matMul_omp(int n)
{
    int i = 0, j = 0, k = 0;
    double sum;

    // #pragma omp parallel for private(i,j,k)
    #pragma omp parallel for simd
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            sum = 0.0;
            for (k = 0; k < n; k++)
            {
                sum += a[i][k] * b[k][j];
            }
            e[i][j] = sum;
        }
    }}

void matMul(int n)
{
    int i = 0, j = 0, k = 0;
    double sum;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            sum = 0.0;
            for (k = 0; k < n; k++)
            {
                sum += a[i][k] * b[k][j];
            }
            d[i][j] = sum;
        }
    }}
    // }
// }
