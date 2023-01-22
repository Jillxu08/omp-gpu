/* Copyright (c) 2019 CSC Training */
/* Copyright (c) 2021 ENCCS */
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define NX 102400000

int main(void)
{
  double vecA[NX],vecB[NX],vecC[NX],vecD[NX];
  double r=0.2;
  double t_omp = 0.0;
  double t_no_omp = 0.0;
  double t1, t2, t3, t4;

/* Initialization of vectors */
  for (int i = 0; i < NX; i++) {
     vecA[i] = pow(r, i);
     vecB[i] = 1.0;
  }

/* dot product of two vectors */

  t1 = omp_get_wtime();
  #pragma omp target data map(from:vecC[0:NX])
  {
     #pragma omp target map(to:vecA[0:NX],vecB[0:NX])
     for (int i = 0; i < NX; i++) {
        vecC[i] = vecA[i] * vecB[i];
     }

/* Initialization of vectors again */
     for (int i = 0; i < NX; i++) {
        vecA[i] = 0.5;
        vecB[i] = 2.0;
     }

     #pragma omp target map(to:vecA[0:NX],vecB[0:NX])
     for (int i = 0; i < NX; i++) {
        vecC[i] = vecC[i] + vecA[i] * vecB[i];
     }
  }
  t2 = omp_get_wtime();
  t_omp = t2 - t1;
  printf("t_omp is: %8.6f \n", t_omp);

  double sum = 0.0;
  /* calculate the sum */
  for (int i = 0; i < NX; i++) {
    sum += vecC[i];
  }
  printf("The sum_c is: %8.6f \n", sum);



  t3 = omp_get_wtime();
  for (int i = 0; i < NX; i++) {
     vecA[i] = pow(r, i);
     vecB[i] = 1.0;
  }

/* dot product of two vectors */
  for (int i = 0; i < NX; i++) {
     vecD[i] = vecA[i] * vecB[i];
  }

/* Initialization of vectors again */
  for (int i = 0; i < NX; i++) {
     vecA[i] = 1.0;
     vecB[i] = 1.0;
  }

  for (int i = 0; i < NX; i++) {
     vecD[i] = vecD[i] + vecA[i] * vecB[i];
  }
  
  t4 = omp_get_wtime();
  t_no_omp = t4 - t3;
  printf("t_no_omp is: %8.6f \n", t_no_omp);

  double sum_d = 0.0;
  /* calculate the sum */
  for (int i = 0; i < NX; i++)
    sum_d += vecD[i];

  printf("The sum_d is: %8.6f \n", sum);

  return 0;

}
