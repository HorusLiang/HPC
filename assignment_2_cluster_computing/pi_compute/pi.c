#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
  //initialize variables
  long i;
  // printf("number of thread used now: %d \n",omp_get_num_threads()); // history version
  printf("number of thread used max now: %d \n",omp_get_max_threads());

  long niter[] = {(long)2000000000.0/16.0*omp_get_max_threads()}; // avoid overflow
  // int niter[] = {31250000, 62500000, 125000000, 250000000, 500000000, 1000000000,2000000000}; // history version
  
  for (int j = 0; j < 1; j++) {
    double pi = 0;
    // Get timing
    double start,end;
    start=omp_get_wtime();

    // Calculate PI using Leibnitz sum
    /* Fork a team of threads */
    #pragma omp parallel for reduction(+ : pi)
    for(i = 0; i < niter[j]; i++)
    {
      double t=0.0;
      t= pow(-1, i) * (4.0 / (2*((double) i)+1));
      pi = pi + t;
    } /* Reduction operation is done. All threads join master thread and disband */

    // Stop timing
    end=omp_get_wtime();

    // Print result
    printf("Pi estimate: %.20f, obtained in %f seconds with niter = %ld \n", pi, end - start, niter[j]);
  }
}