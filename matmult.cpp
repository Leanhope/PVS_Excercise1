#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>

// ---------------------------------------------------------------------------
// allocate space for empty matrix A[row][col]
// access to matrix elements possible with:
// - A[row][col]
// - A[0][row*col]

float **alloc_mat(int row, int col)
{
    float **A1, *A2;

	A1 = (float **)calloc(row, sizeof(float *));		// pointer on rows
	A2 = (float *)calloc(row*col, sizeof(float));    // all matrix elements
    for (int i = 0; i < row; i++)
        A1[i] = A2 + i*col;

    return A1;
}

// ---------------------------------------------------------------------------
// random initialisation of matrix with values [0..9]

void init_mat(float **A, int row, int col)
{
    for (int i = 0; i < row*col; i++)
		A[0][i] = (float)(rand() % 10);
}

// ---------------------------------------------------------------------------
// DEBUG FUNCTION: printout of all matrix elements

void print_mat(float **A, int row, int col, char *tag)
{
    int i, j;

    printf("Matrix %s:\n", tag);
    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col; j++) 
            printf("%6.1f   ", A[i][j]);
        printf("\n"); 
    }
}

// ---------------------------------------------------------------------------
//Comparing the elements of two matrices

bool compare(float **A, float **B, int row, int col)
{
    for(int i = 0; i<row; ++i)
        for (int j = 0; j<col; j++)
            if (A[i][j] != B [i][j])
            {
                return false;
                break;
            }
    return true;
}

// ---------------------------------------------------------------------------
//Main Method

int main(int argc, char *argv[])
{
	float **A, **B, **C, **D;	// matrices
    int d1, d2, d3;         // dimensions of matrices
    int i, j, k;			// loop variables
    float startserial, timeserial, startparallel, timeparallel; //Created floating point varialbe to store the elapsed time to an arbitrary point in the the past to calc the executen time


    /* print user instruction */
    if (argc != 4)
    {
        printf ("Matrix multiplication: C = A x B\n");
        printf ("Usage: %s <NumRowA> <NumColA> <NumColB>\n", argv[0]); 
        return 0;
    }

    /* read user input */
    d1 = atoi(argv[1]);		// rows of A and C
    d2 = atoi(argv[2]);     // cols of A and rows of B
    d3 = atoi(argv[3]);     // cols of B and C

    printf("Matrix sizes C[%d][%d] = A[%d][%d] x B[%d][%d]\n", d1, d3, d1, d2, d2, d3);

    /* prepare matrices */
    A = alloc_mat(d1, d2);
    init_mat(A, d1, d2); 
    B = alloc_mat(d2, d3);
    init_mat(B, d2, d3);
    C = alloc_mat(d1, d3);	// no initialisation of C, because it gets filled by matmult
    D = alloc_mat(d1, d3);

    startserial = omp_get_wtime();

    /* serial version of matmult */
    printf("Perform serial matrix multiplication...\n");
    for (i = 0; i < d1; i++)
       for (j = 0; j < d3; j++)
          for (k = 0; k < d2; k++)
             C[i][j] += A[i][k] * B[k][j];

    timeserial = omp_get_wtime() - startserial;
    
    /* parallel version of matmult */    
    printf("Perform parallel matrix multiplication...\n");
    startparallel = omp_get_wtime();

    #pragma omp parallel num_threads(2)
    {
        #pragma omp for collapse(3)
        for (i = 0; i < d1; i++)
            for (j = 0; j < d3; j++)
                for (k = 0; k < d2; k++)
                    D[i][j] += A[i][k] * B[k][j];
    }
    timeparallel = omp_get_wtime() - startparallel;

    /* test output
    print_mat(A, d1, d2, "A"); 
    print_mat(B, d2, d3, "B"); 
    print_mat(D, d1, d3, "C"); */

    std::cout << "Serial matrix multiplication took " << timeserial << " seconds." << std::endl;
    std::cout << "Parallel matrix multiplication took " << timeparallel << " seconds." << std::endl;
    if(compare(C, D, d1, d3))
    {
        std::cout << "Both matrices are identical." << std::endl;
    }
    else
    {
        std::cout << "The matrices are not identical." << std::endl;
    }

    std::cout << "The Speedup is " << timeserial/timeparallel << std::endl;
    printf ("\nDone.\n");

    return 0;
}
