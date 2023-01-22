#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

/*double currentTime()
{
    return omp_get_wtime();
}*/

double currentTime() {
    struct timeb tm;
    ftime(&tm);
    return (double) tm.time + (double) tm.millitm / 1000.0;
}


int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s \n", argv[0]);
        return 0;
    }

    int n = atoi(argv[1]);

    double* x = (double*)malloc(sizeof(double) * n);
    double* y = (double*)malloc(sizeof(double) * n);

    double idrandmax = 1.0 / RAND_MAX;
    double a = idrandmax * rand();
    int j;
    for (j = 0; j < n; j++)
    {
        x[j] = idrandmax * rand();
//        y[j] = idrandmax * rand();
    }


    double tick = currentTime();
    #pragma omp target data map(tofrom: x[0:n],y[0:n])
     {
     int i;
    #pragma omp target device (0)
    #pragma omp for private(i)
    for (i = 0; i < n; i++)
    y[i] += a * x[i];
    }
    double sec = currentTime()-tick;
    printf("time:\t\t%4f\n", sec * 1.0e3);
    int i;
    double avg = 0.0, min = y[0], max = y[0];
    for ( i = 0; i < n; i++)
    {
        avg += y[i];
        if (y[i] > max) max = y[i];
        if (y[i] < min) min = y[i];
    }

    printf("min = %f, max = %f, avg = %f\n", min, max, avg / n);
    free(x);
    free(y);
    return 0;
}