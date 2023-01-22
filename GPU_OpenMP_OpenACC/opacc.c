#include<stdio.h>
#include<openacc.h>
#include<omp.h>
#include <sys/time.h>
#define LEN 100
 
int main(int argc,char**argv)
{
 
	int a[2][LEN];
	for(int i=0;i<2;i++){
		for(int k=0;k<LEN;k++){
			a[i][k]=2;
		}
	}
	
	#pragma omp parallel num_threads(2)
	{
		int th= omp_get_thread_num();
		acc_set_device_num(th,acc_device_nvidia);
		int index=acc_get_device_num(acc_device_nvidia);
		#pragma acc kernels
		for(int j=0;j<LEN;j++){
			a[th][j]=index;
		}
		
	} 
	
	for(int i=0;i<10;i++){
		printf("a[0][%d]=%d\ta[1][%d]=%d\n",i,a[0][i],i,a[1][i]);
	}
	return 0;
}