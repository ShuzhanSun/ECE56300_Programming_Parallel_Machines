#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[])
{
	int N = 10000;

	int i, n_run, pid, numP, tot;
	int* arr;
	double t_start, t_end, elapsed[6], t_aver;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numP);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);

	// Generate the array
	int* arr = new int[N / numP];
	for (i = 0; i < N / numP; i++)
		arr[i] = 1;

	if (pid == 0)
		std::cout << "N = " << N << ", numP = " << numP << std::endl << std::endl;

	// Run the recurrence 6 times
	for (n_run = 0; n_run < 6; n_run++)
	{
		if (pid == 0)
			std::cout << "Test run " << n_run + 1 << ": \n";

		MPI_Barrier(MPI_COMM_WORLD);
		t_start = MPI_Wtime();

		for (i = 0, tot = 0; i < N / numP; i++)
			tot = tot + arr[i];

		//Pass data, collective communication
		if (pid == 0)
			MPI_Reduce(MPI_IN_PLACE, &tot, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		else
			MPI_Reduce(&tot, (void *)NULL, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

		//Print elapsed time and summation result
		t_end = MPI_Wtime();
		elapsed[n_run] = t_end - t_start;
		if (pid == 0)
			std::cout << "Total summation = " << tot << ", elapsed time = " << elapsed[n_run] << "s\n";
	}

	for (n_run = 1, t_aver = 0; n_run < 6; n_run++)
		t_aver += elapsed[n_run];
	t_aver = t_aver / 5;
	if (pid == 0)
		std::cout << "\nAverage elapsed time over run 2 to run 6 (avoid large cache startup effect in first run) is " << t_aver << "s \n";

	delete[] arr;
	MPI_Finalize();
}