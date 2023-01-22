#ifdef _OPENMP
    #include <omp.h>
#else
    #error "OpenMP support required"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int arg, char *argv[]) {
    const unsigned int size = 1 << 16;

    // Platform information
    printf("OpenMP version (yyyymm): %i\n", _OPENMP);
    printf("Number of OpenMP devices: %i\n", omp_get_num_devices());

    printf("* Allocate memory on the host\n");
    double *a = (double *) malloc(size * sizeof(double));
    if (a == NULL) {
        errx(1, "malloc a[] failed");
    }

    printf("* Preprocess / initialize data on the host\n");
    printf("  e.g. read data from storage\n");
    for (int i = 0; i < size; i++) {
        a[i] = 0.;
    }

    printf("* Copy data from the host to the device\n");
    #pragma omp target enter data map(to: a[0:size])
    {}

    printf("* Compute on the device\n");
    #pragma omp target teams distribute parallel for simd
    for (int i = 0; i < size; i++) {
        a[i]++;
    }

    printf("* Transfer data back from the device to the host\n");
    #pragma omp target update from(a[0:size])
    {}

    printf("* Delete data on the device");
    #pragma omp target exit data map(release: a[0:size])
    {}

    printf("* Postprocess data on the host\n");
    printf("  e.g. write data to storage\n");
    for (int i = 0; i < size; i++) {
        if (a[i] != 1.) {
            errx(2, "Computation on GPU failed");
        }
    }

    printf("* Free memory on the host\n");
    free(a);

    return 0;
}
