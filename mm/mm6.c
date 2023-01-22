#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<sys/time.h>


#define MATRIX_SIZE 8      

void strassenMul(double* X, double* Y, double* Z, int m);
void matMul(double* A, double* B, double* C, int n);
void matAdd(double* A, double* B, double* C, int m);
void matSub(double* A, double* B, double* C, int m);
double get_wall_time();


int main()
{
    int N;
    int i, j;
    clock_t start, end;
    double start2, end2;
    double elapsed, elapsed2;
    double *X, *Y, *Z, *W, *A, *B, *C;

    printf("-------------------------------------------------------------------------\n\n");
    
    N = MATRIX_SIZE;         // 2的count次方
    printf("Matrix size = %2d\t",N);
    X = malloc(N*N*sizeof(double));
    Y = malloc(N*N*sizeof(double));
    Z = malloc(N*N*sizeof(double));
    W = malloc(N*N*sizeof(double));
    if (X==NULL || Y==NULL || Z==NULL || W==NULL) {
        printf("Out of memory (1)\n");
        return 1;
    }
    srand(0);
    // srand((unsigned)time(NULL));
    for (i=0; i<N*N; i++)
    {
        // X[i] = rand()/(RAND_MAX);
        // Y[i] = rand()/(RAND_MAX);
        X[i] = rand()/(RAND_MAX + 1.);
        Y[i] = rand()/(RAND_MAX + 1.);
    }
    start = get_wall_time();
    matMul(X, Y, W, N);
    end = get_wall_time();
    elapsed = ((double) (end - start));
    printf("naive = %5.4f\t\t",elapsed);

    start = get_wall_time();
    strassenMul(X, Y, Z, N);
    free(W); 
    free(Z); 
    free(Y); 
    free(X); 
    end = get_wall_time();
    elapsed = ((double) (end - start));
    printf("Block = %5.4f\n",elapsed2);
    
    printf("-------------------------------------------------------------------\n\n\n");
}

void matMul(double *A, double *B, double *C, int n)
{
    int i = 0, j = 0, k = 0, row = 0, col = 0;
    double sum;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            sum = 0.0;
            for (k = 0; k < n; k++)
            {
                sum += A[i*n+k] * B[k*n+j];
            }
            C[i*n+j] = sum;
        }
    }
}

double get_wall_time(){
  struct timeval time;
  if (gettimeofday(&time,NULL)){
    return 0;
  }
  return (double)time.tv_sec + (double)time.tv_usec * .000001;
}






// C++
#include <iostream>
#include <vector>

template<typename T>
struct Matrix {
  Matrix(size_t r, size_t c) {
    Data.resize(c, std::vector<T>(r, 0));
  }

  void SetSubMatrix(const int r, const int c, const int rn, const int cn,
                    const Matrix<T>& A, const Matrix<T>& B) {
    for (int cl = c; cl < cn; ++cl)
      for (int rl = r; rl < rn; ++rl)
        Data[cl][rl] = A.Data[cl - c][rl - r] + B.Data[cl - c][rl - r];
  }

  static Matrix<T> SquareMultiplyRecursive(Matrix<T>& A, Matrix<T>& B,
                                           int ar, int ac, int br, int bc, int n) {
    Matrix<T> C(n, n);

    if (n == 1) {
      C.Data[0][0] = A.Data[ac][ar] * B.Data[bc][br];
    } else {
      C.SetSubMatrix(0, 0, n / 2, n / 2,
        SquareMultiplyRecursive(A, B, ar, ac, br, bc, n / 2),
        SquareMultiplyRecursive(A, B, ar, ac + (n / 2), br + (n / 2), bc, n / 2));

      C.SetSubMatrix(0, n / 2, n / 2, n,
        SquareMultiplyRecursive(A, B, ar, ac, br, bc + (n / 2), n / 2),
        SquareMultiplyRecursive(A, B, ar, ac + (n / 2), br + (n / 2), bc + (n / 2), n / 2));

      C.SetSubMatrix(n / 2, 0, n, n / 2,
        SquareMultiplyRecursive(A, B, ar + (n / 2), ac, br, bc, n / 2),
        SquareMultiplyRecursive(A, B, ar + (n / 2), ac + (n / 2), br + (n / 2), bc, n / 2));

      C.SetSubMatrix(n / 2, n / 2, n, n,
        SquareMultiplyRecursive(A, B, ar + (n / 2), ac, br, bc + (n / 2), n / 2),
        SquareMultiplyRecursive(A, B, ar + (n / 2), ac + (n / 2), br + (n / 2), bc + (n / 2), n / 2));
    }

    return C;
  }

  void Print() {
    for (size_t c = 0; c < Data.size(); ++c) {
      for (size_t r = 0; r < Data[0].size(); ++r)
        std::cout << Data[c][r] << " ";
      std::cout << "\n";
    }
    std::cout << "\n";
  }

  std::vector<std::vector<T> > Data;
};

int main() {
  Matrix<int> A(2, 2);
  Matrix<int> B(2, 2);

  A.Data[0][0] = 2; A.Data[0][1] = 1;
  A.Data[1][0] = 1; A.Data[1][1] = 2;

  B.Data[0][0] = 2; B.Data[0][1] = 1;
  B.Data[1][0] = 1; B.Data[1][1] = 2;

  A.Print();
  B.Print();

  Matrix<int> C(Matrix<int>::SquareMultiplyRecursive(A, B, 0, 0, 0, 0, 2));

  C.Print();
}











    
	//创建一个随机数构成的n*n矩阵
	public static int[][] initializationMatrix(int n){
    
		int[][] result = new int[n][n];//创建一个n*n矩阵
		for(int i = 0;i < n;i++){
    
			for(int j = 0;j < n;j++){
    
				result[i][j] = (int)(Math.random()*10); //随机生成1~10之间的数
			}
		} 
		return result; 
	}

	//蛮力法求矩阵相乘
	public static int[][] BruteForce(int[][] p,int[][] q,int n){
    
		int[][] result = new int[n][n];
		for(int i=0;i<n;i++){
    
			for(int j=0;j<n;j++){
    
				result[i][j] = 0;
				for(int k=0;k<n;k++){
    
					result[i][j] += p[i][k]*q[k][j];
				}
			}
		}  
		return result;
	}

	//分治法求矩阵相乘
	public static int[][] DivideAndConquer(int[][] p,int[][] q,int n){
    
		int[][] result = new int[n][n];//创建一个n*n矩阵
		//当n为2时，用蛮力法求矩阵相乘，返回结果结果
		if(n == 2){
    
			result = BruteForce(p,q,n); 
			return result;
		}
	 
		//当n大于3时，采用分治法，递归求最终结果
		if(n > 2){
    
			int m = n/2;
			
			//将矩阵p分成四块
			int[][] p1 = QuarterMatrix(p,n,1);
			int[][] p2 = QuarterMatrix(p,n,2);
			int[][] p3 = QuarterMatrix(p,n,3);
			int[][] p4 = QuarterMatrix(p,n,4);
			
			//将矩阵q分成四块
			int[][] q1 = QuarterMatrix(q,n,1);
			int[][] q2 = QuarterMatrix(q,n,2);
			int[][] q3 = QuarterMatrix(q,n,3);
			int[][] q4 = QuarterMatrix(q,n,4);
			
			//将结果矩阵分成同等大小的四块
			int[][] result1 = QuarterMatrix(result,n,1);
			int[][] result2 = QuarterMatrix(result,n,2);
			int[][] result3 = QuarterMatrix(result,n,3);
			int[][] result4 = QuarterMatrix(result,n,4);
		
			//最关键的步骤，递归调用DivideAndConquer()函数，并用公式相加
			result1 = AddMatrix(DivideAndConquer(p1,q1,m),DivideAndConquer(p2,q3,m),m);//y=ae+bg
			result2 = AddMatrix(DivideAndConquer(p1,q2,m),DivideAndConquer(p2,q4,m),m);//s=af+bh
			result3 = AddMatrix(DivideAndConquer(p3,q1,m),DivideAndConquer(p4,q3,m),m);//t=ce+dg
			result4 = AddMatrix(DivideAndConquer(p3,q2,m),DivideAndConquer(p4,q4,m),m);//u=cf+dh
			
			//合并，将四块小矩阵合成整体
			result = TogetherMatrix(result1,result2,result3,result4,m);//把分成的四个小矩阵合并成一个大矩阵
		}
		return result;
	}
	
	//strassen法
	public static int[][] Strassen(int[][] p,int[][] q,int n){
    
		int[][] result = new int[n][n];//创建一个n*n矩阵
		if( n == 2){
    
			result = BruteForce(p,q,n);
			return result;
		}
		int m = n/2;
		
		//将矩阵p分成四块
		int[][] p1 = QuarterMatrix(p,n,1);
		int[][] p2 = QuarterMatrix(p,n,2);
		int[][] p3 = QuarterMatrix(p,n,3);
		int[][] p4 = QuarterMatrix(p,n,4);
		
		//将矩阵q分成四块
		int[][] q1 = QuarterMatrix(q,n,1);
		int[][] q2 = QuarterMatrix(q,n,2);
		int[][] q3 = QuarterMatrix(q,n,3);
		int[][] q4 = QuarterMatrix(q,n,4);
				int[][] m1 = DivideAndConquer(AddMatrix(p1,p4,m),AddMatrix(q1,q4,m),m);
		int[][] m2 = Strassen(AddMatrix(p3,p4,m),q1,m);
		int[][] m3 = Strassen(p1,ReduceMatrix(q2,q4,m),m);
		int[][] m4 = Strassen(p4,ReduceMatrix(q3,q1,m),m);
		int[][] m5 = Strassen(AddMatrix(p1,p2,m),q4,m);
		int[][] m6 = Strassen(ReduceMatrix(p3,p1,m),AddMatrix(q1,q2,m),m);
		int[][] m7 = Strassen(ReduceMatrix(p2,p4,m),AddMatrix(q3,q4,m),m);
		
		//将结果矩阵分成同等大小的四块
		int[][] result1 = QuarterMatrix(result,n,1);
		int[][] result2 = QuarterMatrix(result,n,2);
		int[][] result3 = QuarterMatrix(result,n,3);
		int[][] result4 = QuarterMatrix(result,n,4);
	
		result1 = AddMatrix(ReduceMatrix(AddMatrix(m1,m4,m),m5,m),m7,m);
		result2 = AddMatrix(m3,m5,m);
		result3 = AddMatrix(m2,m4,m);
		result4 = AddMatrix(AddMatrix(ReduceMatrix(m1,m2,m),m3,m),m6,m);
		
		result = TogetherMatrix(result1,result2,result3,result4,m);//把分成的四个小矩阵合并成一个大矩阵
		
		return result;
	}
	
	
	
	
	//获取矩阵的四分之一，number用来确定返回哪一个四分之一
	public static int[][] QuarterMatrix(int[][] p,int n,int number){
    
		int rows = n/2;  //行数减半
		int cols = n/2;  //列数减半
		int[][] result = new int[rows][cols];
		switch(number){
    
		//左上
		case 1 :
		{
    
			for(int i=0;i<rows;i++)
				for(int j=0;j<cols;j++)
					result[i][j] = p[i][j];
			break;
		}
		//右上
		case 2 :
		{
    
			for(int i=0;i<rows;i++)
				for(int j=0;j<n-cols;j++)
					result[i][j] = p[i][j+cols];
			break;
		}
		//左下
		case 3 :
		{
    
			for(int i=0;i<n-rows;i++)
				for(int j=0;j<cols;j++)
					result[i][j] = p[i+rows][j];
			break;
		}
		//右下
		case 4 :
		{
    
			for(int i=0;i<n-rows;i++)
				for(int j=0;j<n-cols;j++)
					result[i][j] = p[i+rows][j+cols];
			break;
		}
		default:
			break;
		}
	
		return result;
	}

	//把均分为四分之一的矩阵，合成一个矩阵
	public static int[][] TogetherMatrix(int[][] a,int[][] b,int[][] c,int[][] d,int n){
    
		int[][] result = new int[2*n][2*n];
		for(int i=0;i<2*n;i++){
    
			for(int j=0;j<2*n;j++){
    
				if(i<n){
    
					if(j<n)
						result[i][j] = a[i][j];
					else
						result[i][j] = b[i][j-n];
				}else{
    
					if(j<n)
						result[i][j] = c[i-n][j];
					else
						result[i][j] = d[i-n][j-n];
				}
			}
		}
		return result;
	}


	//求两个矩阵相加结果
	public static int[][] AddMatrix(int[][] p,int[][] q,int n){
    
		int[][] result = new int[n][n];
		for(int i=0;i<n;i++){
    
			for(int j=0;j<n;j++){
    
				result[i][j] = p[i][j]+q[i][j];
			}
		}
		return result;
	}
	
	//求两个矩阵相减结果
	public static int[][] ReduceMatrix(int[][] p,int[][] q,int n){
    
		int[][] result = new int[n][n];
		for(int i=0;i<n;i++){
    
			for(int j=0;j<n;j++){
    
				result[i][j] = p[i][j]-q[i][j];
			}
		}
		return result;
	}
	
	//输出矩阵的函数
	public static void PrintfMatrix(int[][] matrix,int n){
    
		for(int i=0;i<n;i++){
    
			for(int j=0;j<n;j++)
				System.out.printf("% 4d",matrix[i][j]);
			System.out.println();
		}
	
	}

	public static void main(String args[]){
    
		int[][] p = initializationMatrix(8);
		int[][] q = initializationMatrix(8);
		//输出生成的两个矩阵
		System.out.println("p：");
		PrintfMatrix(p,8);
		System.out.println();
		System.out.println("q：");
		PrintfMatrix(q,8);
 
		//输出分治法矩阵相乘后的结果
		int[][] bru_result = BruteForce(p,q,8);//新建一个矩阵存放矩阵相乘结果
		System.out.println();
		System.out.println("\nA*B(蛮力法)：");
		PrintfMatrix(bru_result,8);
		
		//输出分治法矩阵相乘后的结果
		int[][] dac_result = DivideAndConquer(p,q,8);//新建一个矩阵存放矩阵相乘结果
		System.out.println();
		System.out.println("A*B(分治法)：");
		PrintfMatrix(dac_result,8);
		
		//输出strassen法矩阵相乘后的结果
		int[][] stra_result = Strassen(p,q,8);//新建一个矩阵存放矩阵相乘结果
		System.out.println("\nA*B(strassen法)：");
		PrintfMatrix(stra_result,8);
		
	}
 
}