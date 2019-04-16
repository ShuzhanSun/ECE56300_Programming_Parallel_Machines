#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BILLION  1000000000.0;

void doWork(struct timespec *rqtp) {
   nanosleep(rqtp, NULL);
}

int* initWork(int n) {
   int i;
   double r;
   int* wA = (int *) malloc(sizeof(int)*n);   
   if (wA == NULL)
   {
	   printf("Memory allocation error.\n");
	   return NULL;
   }
   for (i = 0; i < n; i++) {
      wA[i] = rand( )%2*i/(n/10);
   }
   return wA;
}

int main (int argc, char *argv[]) {
   int i;
   int *w = initWork(10000);
   for (i = 0; i < 10000; i+=500) {
      printf("w[%d] = %d\n", i, w[i]);
   }

   struct timespec start, end;
   double time_spent;

   omp_set_num_threads(32);

   clock_gettime(CLOCK_REALTIME, &start);
#pragma omp parallel for schedule(guided)
   for (i = 0; i < 10000; i++) 
   {
	   struct timespec rqtp;
	   rqtp.tv_sec = 0;
	   rqtp.tv_nsec = w[i] /1000.0 * BILLION;
	   doWork(&rqtp);
   }
   clock_gettime(CLOCK_REALTIME, &end);

   time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION;
   printf("Time elpased is %f seconds \n", time_spent);

   free(w);
}