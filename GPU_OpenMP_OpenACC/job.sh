#!/bin/bash
#PJM -L "rscunit=ito-b" 
#PJM -L "rscgrp=ito-g-4"
#PJM -L "vnode=1"
#PJM -L "vnode-core=36"
#PJM -L "elapse=30:00"
#PJM -j
#PJM -X

# ./mm_omp
./test_omp
# pjsub job.sh