/* File:     loop3.c
 *
 * Purpose:  Implement loop 3 from Homework 1 of CS 511
 *
 * Compile:  gcc -g -Wall -o loop3.c
 * Run:      ./loop3
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

void Get_dims(int* n_p, int* s_p);
void Mat_Mat_mult(double A[], double B[], double C[], int n, int s);

/*-------------------------------------------------------------------*/
int main(void) {
   double* a = NULL;
   double* b = NULL;
   double* c = NULL;
   double C = 0.0;
   int n, i, j, s;
   clock_t start, end;
   double cpu_time_used;

   Get_dims(&n, &s);
   a = malloc(n*n*sizeof(double));
   b = malloc(n*n*sizeof(double));
   c = malloc(n*n*sizeof(double));
   if (a == NULL || b == NULL || c == NULL) {
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

   for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
        c[i*n+j] = 0.0;
      }
   }

   start = clock();

   Mat_Mat_mult(a, b, c, n, s);

   end = clock();
   cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
   printf("The Loop Runtime is %f\n", cpu_time_used);

   for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
         C += c[i*n+j];
      }
   }
   C = C / (n*n);
   printf("The average of the matrix is %f\n", C);
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
void Get_dims(int*  n_p  /* out */,
              int*  s_p  /* out */) {
   printf("Enter the dimension of the matrix\n");
   scanf("%d", n_p);

   if (*n_p <= 0) {
      fprintf(stderr, "n must be positive\n");
      exit(-1);
   }

   printf("Enter the cache size for double values\n");
   scanf("%d", s_p);

   if (*s_p <= 0) {
      fprintf(stderr, "s must be positive\n");
      exit(-1);
   }
}  /* Get_dims */

/*-------------------------------------------------------------------
 * Function:   Mat_Mat_mult
 * Purpose:    Multiply a matrix by a matrix
 * In args:    A: the matrix
 *             B: the matrix being multiplied by A
 *             n: the dimension of matrices A and B
 *             s: 'estimated' cache size for double values
 * Out args:   C: the product matrix AB
 *  L1 Cache: 24K - Each double is 64 bits (8 bytes)
 *     Cache should be able to hold ~375 values
 */
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void Mat_Mat_mult(
                   double  A[]  /* in  */,
                   double  B[]  /* in  */,
                   double  C[]  /* out */,
                   int     n    /* in  */,
                   int     s    /* in  */) {
   int i, j, k, w, y, z;

  for (w = 0; w < n; w+=s) {
    for (y = 0; y < n; y+=s) {
      for (z = 0; z < n; z+=s) {
        for (i = w; i < MIN(w+s-1,n); i++) {
            for (k = y; k < MIN(y+s-1,n); k++) {
               for (j = z; j < MIN(z+s-1,n); j++) {
                  C[i*n+j] += A[i*n+k]*B[j*n+k];
               }
            }
        }
      }
    }
  }
}  /* Mat_Mat_mult */
