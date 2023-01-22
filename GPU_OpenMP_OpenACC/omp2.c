/* Copyright (c) 2019 CSC Training */
// Copyright (c) 2021 ENCCS
#include <stdio.h>
#include <math.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#define NX 102400000

int main(void)
{
  float vecA[NX],vecB[NX],vecC[NX],vecD[NX];
  float r=2.5378;
  double t_omp_gpu = 0.0;
  double t_omp = 0.0;
  double t_no_omp = 0.0;
  double t1, t2, t3, t4, t5, t6;
  int i, j;
  int flag = 0;

  // int num_devices = omp_get_num_devices();
  // printf("Number of available devices %d\n", num_devices);

/* Initialization of vectors */
  for (int i = 0; i < NX; i++) {
     vecA[i] = vecC[i] = vecD[i] =pow(r, i);
     vecB[i] = 3.19;
    //  vecC[i] = pow(r, i);
  }

/* dot product of two vectors */
  t1 = omp_get_wtime();
  #pragma omp target map(to:vecB[0:NX]) map(tofrom:vecA[0:NX])
  #pragma omp teams distribute parallel for simd
  // for (int k=0; k<0; k++) {
    for (int i = 0; i < NX; i++) {
      for (int j=0; j<1000; j++)
        // for (int k=0; k<10000; k++)
        {
          vecA[i] *= vecB[i];
        }}
    // }
  t2 = omp_get_wtime();
  t_omp_gpu = t2 - t1;

  t3 = omp_get_wtime();
  #pragma omp parallel for private(i,j)
  // for (int k=0; k<1000; k++) {
      for (int i = 0; i < NX; i++) {
        for (int j=0; j<1000; j++)
          {
            vecC[i] *= vecB[i];
          }}
      // }
  t4 = omp_get_wtime();
  t_omp = t4 - t3;

  t5 = omp_get_wtime();
  for (int i = 0; i < NX; i++) {
    for (int j=0; j<1000; j++)
      // for (int k=0; k<10000; k++)
      {
        vecD[i] *= vecB[i];
      }
  }
  t6 = omp_get_wtime();
  t_no_omp = t6 - t5;

  for (int i=0; i<NX; i++) {
    if (fabs (vecA[i] - vecD[i]) > 1.0E-10)
      flag = 1; }
  
  if(flag)
    printf("THE ANSWER IS WRONG!\n");
  else  
    printf("THE ANSWER IS RIGHT!\n");

  printf("The NX = : %d \n", NX);
  // printf("The for loop time j = : %d \n", j);
  // printf("The sum_C is: %f \n", sum_C);
  // printf("The sum_D is: %f \n", sum_D);
  printf("t_omp_gpu = : %8.6f \n", t_omp_gpu);
  printf("t_omp = : %8.6f \n", t_omp);
  printf("t_no_omp = : %8.6f \n", t_no_omp);
  return 0;
}
