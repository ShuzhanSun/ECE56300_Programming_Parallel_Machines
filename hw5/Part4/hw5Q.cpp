#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BILLION  1000000000.0;

typedef struct Q {
   int* q;
   int pos;
   int size;
} Q;

struct Q* initQ(int n) {
   //int i;
   struct Q *newQ = (struct Q *) malloc(sizeof(Q));   
   newQ->q = (int*) malloc(sizeof(int)*n);
   newQ->pos = -1;
   newQ->size = n-1;
   return newQ;
}

void putWork(struct Q* workQ) {
   if (workQ->pos < (workQ->size)) {
      workQ->pos++;
      workQ->q[workQ->pos] = (int) (rand( )%2*(workQ->pos / (workQ->size / 10)));
   } else printf("ERROR: attempt to add Q element%d\n", workQ->pos+1);
}

//int getWork(struct Q* workQ) {
//   if (workQ->pos > -1) {
//      int w = workQ->q[workQ->pos];
//      workQ->pos--;
//      return w;
//   } else printf("ERROR: attempt to get work from empty Q%d\n", workQ->pos);
//}

// To avoid race on the workQ->pos--, I rewrite the function getwork 
int getWork(struct Q* workQ, int i) {
	if (i > -1) {
		int w = workQ->q[i];
		//workQ->pos--;
		return w;
	}
	else printf("ERROR: attempt to get work from empty Q%d\n", i);
}

int main() 
{
	int n = 1000000, i;
	struct timespec start, end;
	double time_spent;

	// initialize and add work to the work queue
	struct Q* workQ = initQ(n);
	for (i = 0; i < n; i++)
		putWork(workQ);

	omp_set_num_threads(40);

	clock_gettime(CLOCK_REALTIME, &start);
#pragma omp parallel for 
	for (i = 0; i < n; i++)
		getWork(workQ, i);
	clock_gettime(CLOCK_REALTIME, &end);

	time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION;
	printf("Time elpased is %f seconds \n", time_spent);

}