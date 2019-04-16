#include <stdio.h>
#include <omp.h>

int main()
{
	FILE *fp;
	if ((fp = fopen("Output_hw4_4.txt", "w+")) == NULL)
	{
		printf("Failure to create the output file!\n");
		return 0;
	}
	fprintf(fp, "Output from the code: \n");

	int count;
	omp_lock_t lck;
	omp_init_lock(&lck);

	// Using #pragma omp parallel for
	count = 0;
	fprintf(fp, "\nUsing #pragma omp parallel for \n");
#pragma omp parallel for
	for (int i = 0; i < 100; i++)
	{
		omp_set_lock(&lck);
		count++;
		fprintf(fp, "Thread id = %d, current number of loop iterations is: %d \n", omp_get_thread_num, count);
		omp_unset_lock(&lck);
	}

	// Using #pragma omp parallel 
	count = 0;
	fprintf(fp, "\nUsing #pragma omp parallel \n");
#pragma omp parallel 
	{
		for (int i = 0; i < 100; i++)
		{
			omp_set_lock(&lck);
			count++;
			fprintf(fp, "Thread id = %d, current number of loop iterations is: %d \n", omp_get_thread_num, count);
			omp_unset_lock(&lck);
		}
	}

	omp_destroy_lock(&lck);
	fclose(fp);
}