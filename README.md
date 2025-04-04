# Parallelization of Matrix Sum Calculation

This project aims to parallelize the calculation of the sum of a randomly generated square matrix using threads. The initial program, `mat-gen-sum.c`, performs the following operations:

1. Generates an \( n \times n \) square matrix of integers with pseudo-random values between 0 and 10.
2. Calculates the sum of the matrix values.
3. Displays the sum of the matrix values.

## Project Steps

### 1. Compilation and Verification of the Initial Program

- **Compilation**: Compile the `mat-gen-sum.c` program to verify its functionality.
- **Execution**: Run the program with the input `0 10 1` to check its determinism.

### 2. Parallelizing the Sum Calculation

#### Version 1: `mat-gen-sum-mt-v1.c`

- **Objective**: Parallelize only the calculation of the matrix sum.
- **Implementation**:
  - Each thread calculates the sum of one row of the matrix.
  - The number of threads is equal to \( n \), where thread \( i \) calculates the sum of row \( i \).
- **Synchronization**: The data for each thread are independent, so no synchronization is needed for this version.

#### Version 2: `mat-gen-sum-mt-v2.c`

- **Objective**: Improve task distribution among threads.
- **Implementation**:
  - Each thread receives two integers \( i \) and \( j \) and calculates the sum of rows \( x \) such that \( i \leq x < j \).
  - The main program distributes the rows evenly among the threads.
- **Assumption**: \( n \) is a multiple of the number of threads.

### 3. Parallelizing Matrix Generation

#### Version 3: `mat-gen-sum-mt-v3.c`

- **Objective**: Also parallelize the generation of the matrix.
- **Dependencies**:
  - In the initial code, matrix generation is sequential.
- **Proposed Method**:
  - Use a deterministic method to pseudo-randomly generate the matrix content, regardless of the number of threads.
  - Implement this method using a reentrant `rand` function like `rand_r`.


