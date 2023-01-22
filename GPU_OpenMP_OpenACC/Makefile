CC=gcc
CXX=g++
COPT=-mcmodel=medium -O3
COPT_OMP=-fopenmp -foffload="-lm -latomic"
COPT_MATH=-lm

TARGETS = \
mm_omp \
mm_omp_gpu1 \

.phony: all

all: $(TARGETS)

# Serial version
mm_omp: mm_omp.c
	$(CC) $(COPT) $(COPT_OMP) -o $@ $^ $(COPT_MATH)

mm_omp_gpu1: mm_omp_gpu1.c
	$(CC) $(COPT) $(COPT_OMP) -o $@ $^ $(COPT_MATH)
