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

   struct timespec start, end, rqtp;
   rqtp.tv_sec = 0;
   double time_spent;

   int nt = omp_get_num_procs();
   omp_set_num_threads(nt);

   int T = 2;

   clock_gettime(CLOCK_REALTIME, &start);
	for (i = 0; i < 10000; i++)
	{
		rqtp.tv_nsec = w[i] / 1000.0 * BILLION;
		doWork(&rqtp);
	}

   clock_gettime(CLOCK_REALTIME, &end);

   time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION;
   printf("Time elpased is %f seconds \n", time_spent);

   free(w);
}