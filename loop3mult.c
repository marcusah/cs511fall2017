/* File:     loop3.c
 *
 * Purpose:  Implement loop 3 multithreaded from Homework 1 of CS 511
 *
 * Compile:  gcc -g -Wall -o loop3mult.c
 * Run:      ./loop3mult
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
#include <pthread.h>

void *Mat_Mat_mult();
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

struct readThreadParams {
    double* a;
    double* b;
    double* c;
    int n;
};

/*-------------------------------------------------------------------*/
int main(void) {
   struct readThreadParams readParams;
   readParams.a = malloc(readParams.n*readParams.n*sizeof(double));
   readParams.b = malloc(readParams.n*readParams.n*sizeof(double));
   readParams.c = malloc(readParams.n*readParams.n*sizeof(double));
   readParams.n = 1000;
   double C = 0.0;
   int i, j;
   clock_t start, end;
   double cpu_time_used;
   pthread_t thread1, thread2;
   int  iret1, iret2;

   if (readParams.a == NULL || readParams.b == NULL || readParams.c == NULL) {
      fprintf(stderr, "Can't allocate storage\n");
      exit(-1);
   }

   for (i = 0; i < readParams.n; i++) {
      for (j = 0; j < readParams.n; j++) {
        readParams.a[i*readParams.n+j] = 2.0;
      }
   }

   for (i = 0; i < readParams.n; i++) {
      for (j = 0; j < readParams.n; j++) {
        readParams.b[i*readParams.n+j] = 3.0;
      }
   }

   for (i = 0; i < readParams.n; i++) {
      for (j = 0; j < readParams.n; j++) {
        readParams.c[i*readParams.n+j] = 0.0;
      }
   }

   start = clock();

   /* Create independent threads each of which will execute function */

   iret1 = pthread_create( &thread1, NULL, &Mat_Mat_mult, (void*) readParams);
   iret2 = pthread_create( &thread2, NULL, &Mat_Mat_mult, (void*) readParams);

   pthread_join( thread1, NULL);
   pthread_join( thread2, NULL);

   end = clock();
   cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
   printf("The Loop Runtime is %f ", cpu_time_used);

   for (i = 0; i < readParams.n; i++) {
      for (j = 0; j < readParams.n; j++) {
         C += readParams.c[i*readParams.n+j];
      }
   }
   C = C / (readParams.n*readParams.n);
   printf("The average of the matrix is %f ", C);
   free(readParams.a);
   free(readParams.b);
   free(readParams.c);
   return 0;
}  /* main */

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
 *     N-Dimensions is 1000
 */
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void *Mat_Mat_mult( void *ptr) {
    struct *readParamsOrig;
    readParamsOrig = (struct *) ptr;
   double* O = NULL;
   long id = pthread_self();
   int i, j, k, it, jt, kt;

  if (id == 0) {
      for (it = 0; it < readParamsOrig.n/2; it+=375) {
      for (kt = 0; kt < readParamsOrig.n/2; kt+=375) {
      for (jt = 0; jt < readParamsOrig.n/2; jt+=375) {
         for (i = it; i < MIN(it+374,readParamsOrig.n/2); i++) {
            for (k = kt; k < MIN(kt+374,readParamsOrig.n/2); k++) {
               for (j = jt; j < MIN(jt+374,readParamsOrig.n/2); j++) {
                  O[i*(readParamsOrig.n/2)+j] += readParamsOrig.a[i*(readParamsOrig.n/2)+k]*readParamsOrig.b[j*(readParamsOrig.n/2)+k];
               }
            }
         }
      }
      }
      }
  } else {
      for (it = (readParamsOrig.n/2); it < readParamsOrig.n; it+=375) {
      for (kt = (readParamsOrig.n/2); kt < readParamsOrig.n; kt+=375) {
      for (jt = (readParamsOrig.n/2); jt < readParamsOrig.n; jt+=375) {
         for (i = it; i < MIN(it+374,readParamsOrig.n); i++) {
            for (k = kt; k < MIN(kt+374,readParamsOrig.n); k++) {
               for (j = jt; j < MIN(jt+374,readParamsOrig.n); j++) {
                  O[i*readParamsOrig.n+j] += readParamsOrig.a[i*readParamsOrig.n+k]*readParamsOrig.b[j*readParamsOrig.n+k];
               }
            }
         }
      }
      }
      }
  }

  pthread_mutex_lock( &mutex1 );
    for (i = 0; i < readParamsOrig.n; i++) {
      for (j = 0; j < readParamsOrig.n; j++) {
        if ( O[i*readParamsOrig.n+j] != 0) {
            readParamsOrig.c[i*readParamsOrig.n+j] = O[i*readParamsOrig.n+j];
        }
      }
    }
  pthread_mutex_unlock( &mutex1 );

}  /* Mat_Mat_mult */
