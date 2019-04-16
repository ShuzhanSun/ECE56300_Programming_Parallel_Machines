#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

#define BILLION  1000000000.0;

int main()
{
	FILE *fp;
	if ((fp = fopen("Output_hw3C.txt", "w+")) == NULL)
	{
		printf("Failure to create the output file!\n");
		return 0;
	}          
	fprintf(fp, "Output from the code: \n");

	int n_array, i;
	float sum;

	n_array = 10000000;

	// C.1 Sequential reduction from 1 to n_array
	for (i = 1, sum = 0; i <= n_array; i++)
	{
		sum += 1.0/i;
	}
	fprintf(fp, "\nC.1 Sequential reduction from 1 to n_array\n", n_array);
	fprintf(fp, "Sum of the sequence: %f \n", sum);

	// C.2 Sequential reduction from n_array to 1
	for (i = n_array, sum = 0; i >= 1; i--)
	{
		sum += 1.0 / i;
	}
	fprintf(fp, "\nC.2 Sequential reduction from n_array to 1\n", n_array);
	fprintf(fp, "Sum of the sequence: %f \n", sum);

	// C.3 OpenMP reduction from 1 to n_array
	sum = 0;
#pragma omp parallel for reduction(+:sum)
	for (i = 1; i <= n_array; i++)
	{
		sum = sum + 1.0/i;
	}
	fprintf(fp, "\nC.3 OpenMP reduction from 1 to n_array\n", n_array);
	fprintf(fp, "Sum of the sequence: %f \n", sum);

	// C.4 OpenMP reduction from 1 to n_array
	sum = 0;
#pragma omp parallel for reduction(+:sum)
	for (i = n_array; i >= 1; i--)
	{
		sum = sum + 1.0 / i;
	}
	fprintf(fp, "\nC.4 OpenMP reduction from n_array to 1\n", n_array);
	fprintf(fp, "Sum of the sequence: %f \n", sum);

	fclose(fp);
}