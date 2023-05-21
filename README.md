#lp5

## Set the number of threads: 
export OMP_NUM_THREADS=5

## Compile: 
gcc -o objname -fopenmp programfilename.c

## Execute: 
./objname
