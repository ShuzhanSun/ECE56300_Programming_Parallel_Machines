#include <stdio.h>
#include <omp.h>

int main()
{
	FILE *fp;
	if ((fp = fopen("Output_hw3A.txt", "w+")) == NULL)
	{
		printf("Failure to create the output file!\n");
		return 0;
	}          
	fprintf(fp, "Output from the code: \n");

	// A.1 NUmber of processors
	fprintf(fp, "\nA.1 \nNumber of processors: %d \n", omp_get_num_procs());

	// A.2 Thread ID
	fprintf(fp, "\nA.2 \nUnique thread ID of each thread: \n");
#pragma omp parallel
	fprintf(fp, "Hello world from thread %d\n", omp_get_thread_num());

	// A.3 Master thread
	fprintf(fp, "\nA.3 \nThread ID of master thread in the sequential region: %d\n", omp_get_thread_num());
#pragma omp parallel
	{
#pragma omp master
		fprintf(fp, "Thread that executes master statement in the parallel region: %d\n", omp_get_thread_num());
#pragma omp single
		fprintf(fp, "Thread that executes single statement in the parallel region: %d\n", omp_get_thread_num());
	}
	fclose(fp);
}