#include <stdio.h>
#include <omp.h>

int main()
{
	FILE *fp;
	if ((fp = fopen("Output_hw4_2.txt", "w+")) == NULL)
	{
		printf("Failure to create the output file!\n");
		return 0;
	}
	fprintf(fp, "Output from the code: \n");

	int i;

	// Using critical
	i = 0;
	fprintf(fp, "\nUsing critical \nInitialized i = %d \n",i);
#pragma omp parallel 
	{
		int b = 0;
#pragma omp critical
		{
			i++;
			fprintf(fp, "Thread id = %d, current i value is: %d \n", omp_get_thread_num, i);
		}
	}

	// Using master
	i = 0;
	fprintf(fp, "\nUsing master \nInitialized i = %d \n", i);
#pragma omp parallel 
	{
		int b = 0;
#pragma omp master
		{
			i++;
			fprintf(fp, "Thread id = %d, current i value is: %d \n", omp_get_thread_num, i);
		}
	}

	// Using single
	i = 0;
	fprintf(fp, "\nUsing single \nInitialized i = %d \n", i);
#pragma omp parallel 
	{
		int b = 0;
#pragma omp single
		{
			i++;
			fprintf(fp, "Thread id = %d, current i value is: %d \n", omp_get_thread_num, i);
		}
	}

	fclose(fp);
}