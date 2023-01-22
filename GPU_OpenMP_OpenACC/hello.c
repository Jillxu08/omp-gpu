#include <omp.h> 
#include <stdio.h> 

int main() { 
    printf("There are %d devices\n", omp_get_num_devices()); 
}

// #include <omp.h>
//   int main(int argc, char argv[]){
//   int omp_rank;
// #pragma omp parallel
//  {
//    omp_rank = omp_get_thread_num();
//    printf("Hello world! by thread %d", omp_rank);
//   }
//  }