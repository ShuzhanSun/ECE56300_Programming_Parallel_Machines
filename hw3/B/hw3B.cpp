#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

#define BILLION  1000000000.0;

int main()
{
	FILE *fp;
	if ((fp = fopen("Output_hw3B.txt", "w+")) == NULL)
	{
		printf("Failure to create the output file!\n");
		return 0;
	}          
	fprintf(fp, "Output from the code: \n");

	int i, sum, n_array, *myArray,*a, nt;
	struct timespec start, end;
	double time_spent;

	// Initialize the array
	n_array = 10000000;
	myArray = (int *)malloc(n_array * sizeof(int)); 
	if (myArray == NULL) 
	{
		printf("Memory allocation error.\n");
		return 0;
	}
	a = myArray;
	for (i = 0; i < n_array; i++)
	{
		*a = 1;
		a += 1;
	}

	// B.a Sequential reduction
	clock_gettime(CLOCK_REALTIME, &start);
	a = myArray;
	for (i = 0, sum = 0; i < n_array; i++)
	{
		sum += *a;
		a += 1;
	}
	clock_gettime(CLOCK_REALTIME, &end);

	time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION;
	fprintf(fp, "\nB.a \nSum of the array is %d \nTime elpased is %f seconds \n", sum, time_spent);

	// B.b Reduction with omp parallel
	nt = 40;
	omp_set_num_threads(nt);
	int res[40] = { 0 };

	clock_gettime(CLOCK_REALTIME, &start);
	a = myArray;
#pragma omp parallel for 
	for (i = 0; i < n_array; i++)
	{
		res[omp_get_thread_num()] += *a;
		a += 1;
	}

	for (i = 0, sum = 0; i < nt; i++)
	{
		sum += res[i];
	}
	clock_gettime(CLOCK_REALTIME, &end);

	time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION;
	fprintf(fp, "\nB.b \nSum of the array is %d \nTime elpased is %f seconds \n", sum, time_spent);

	// B.c OpenMP reduction
	nt = 40;
	omp_set_num_threads(nt);

	clock_gettime(CLOCK_REALTIME, &start);
	a = myArray;
	sum = 0;
#pragma omp parallel for reduction(+:sum)
	for (i = 0; i < n_array; i++)
	{
		sum = sum + *a;
		a += 1;
	}
	clock_gettime(CLOCK_REALTIME, &end);

	time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION;
	fprintf(fp, "\nB.c \nSum of the array is %d \nTime elpased is %f seconds \n", sum, time_spent);

	fclose(fp);
	free(myArray);
}