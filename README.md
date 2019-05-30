# MPI-PrimeSieve
A simple prime number sieve using MPich


## Idea behind
- Process 0 is a generator, which passes numbers starting from 2 to the following processes.
- The first integer which reaches the process(except process 0) is stored as PrimeHere, which is the next prime number produced.
- The following integers are decided to pass to the following processes based on if it is divisible by PrimeHere of the current process.

## How to run
1. make sure that a MPI library is installed, **mpich** is recommended(the rest of the tutorial assume mpich is installed)
2. run ` mpicc -o mpiPrime mpiPrime.c` to complie
3. run ` mpiexec -np #ofProcesses ./mpiPrime ` where `#ofProcesses` is the number of processes wanted, and the program returns (`#ofProcesses` - 1) prime numbers.