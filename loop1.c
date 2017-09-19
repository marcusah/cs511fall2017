/* File:     loop1.c
 *
 * Purpose:  Implement loop 1 from Homework 1 of CS 511
 *
 * Compile:  gcc -g -Wall -o HW1 loop1.c
 * Run:      ./loop1
 *
 * Input:    Dimensions of the matrix a (n = number of rows, n
 *              = number of columns)
 *           n x n dimensional matrix b
 * Output:   Product matrix c = a*b
 *
 * Errors:   if the number of user-input rows or column isn't
 *           positive, the program prints a message and quits.
 * Note:     Define DEBUG for verbose output
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void Get_dims(int* m_p, int* n_p);
void Avg_matrix(double A[], int n, double avg);
void Mat_Mat_mult(double A[], double B[], double C[], int n);

/*-------------------------------------------------------------------*/
int main(void) {
   double* a = NULL;
   double* b = NULL;
   double* c = NULL;
   double C = 0.0;
   int n, i, j;
   clock_t start, end;
   double cpu_time_used;

   Get_dims(&n);
   a = malloc(n*n*sizeof(double));
   b = malloc(n*n*sizeof(double));
   c = malloc(n*n*sizeof(double));
   if (A == NULL || x == NULL || y == NULL) {
      fprintf(stderr, "Can't allocate storage\n");
      exit(-1);
   }

   for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
        a[i*n+j] = 2.0;
      }
   }

   for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
        b[i*n+j] = 3.0;
      }
   }

   start = clock();

   Mat_Mat_mult(a, b, c, n);

   end = clock();
   cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
   printf("The Loop Runtime is %f ", cpu_time_used);

   Avg_matrix(c,n,C);
   printf("The average of the matrix is %f ", C);
   free(a);
   free(b);
   free(c);
   return 0;
}  /* main */


/*-------------------------------------------------------------------
 * Function:   Get_dims
 * Purpose:    Read the dimensions of the matrix from stdin
 * Out args:   m_p:  number of rows
 *             n_p:  number of cols
 *
 * Errors:     If one of the dimensions isn't positive, the program
 *             prints an error and quits
 */
void Get_dims(int*  n_p  /* out */) {
   printf("Enter the dimension of the matrix\n");
   scanf("%d", n_p);

   if (*n_p <= 0) {
      fprintf(stderr, "n must be positive\n");
      exit(-1);
   }
}  /* Get_dims */

/*-------------------------------------------------------------------
 * Function:   Avg_matrix
 * Purpose:    Average the matrix to get the average
 * In args:    A:  the matrix
 *             n:  dimension of matrix
 * Out arg:    avg:  the average of matrix
 */
void Avg_matrix(
                 double  A[]       /* in */,
                 int     n         /* in  */,
                 double  avg         /* out */) {
   int i, j;

   for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
         avg += A[i*n+j];
      }
  }
  avg = avg / (n**2);
}  /* Avg_matrix */

/*-------------------------------------------------------------------
 * Function:   Mat_Mat_mult
 * Purpose:    Multiply a matrix by a matrix
 * In args:    A: the matrix
 *             B: the matrix being multiplied by A
 *             n: the dimension of matrices A and B
 * Out args:   C: the product matrix AB
 */
void Mat_Mat_mult(
                   double  A[]  /* in  */,
                   double  B[]  /* in  */,
                   double  C[]  /* out */,
                   int     n    /* in  */) {
   int i, j, k;

   for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
         C[i*n+j] = 0.0;
         for (k = 0; k < n; k++) {
            C[i*n+j] += A[i*n+j]*B[j*n+k];
         }
      }
   }
}  /* Mat_Mat_mult */
