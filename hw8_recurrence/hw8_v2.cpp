#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[])
{
	int N = 10000;

	int i, n_run, pid, numP, tot;
	int* arr;
	double t_start, t_end, elapsed[6], t_aver;
	int buffer = 0, dest, source, count = 1, tag[4] = { 1, 2, 3, 4 };
	MPI_Status Stat; // status structure

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

		//Pass data, send/recv, numP -> numP/2
		if (pid % 2 == 0) 
		{
			source = pid + 1;
			MPI_Recv(&buffer, count, MPI_INT, source, tag[0], MPI_COMM_WORLD, &Stat);
		}
		else if (pid % 2 == 1)
		{
			dest = pid - 1;
			MPI_Send(&tot, count, MPI_INT, dest, tag[0], MPI_COMM_WORLD);
		}
		tot += buffer;

		//Pass data, send/recv, numP/2 -> numP/4
		if (pid % 4 == 0)
		{
			source = pid + 2;
			MPI_Recv(&buffer, count, MPI_INT, source, tag[1], MPI_COMM_WORLD, &Stat);
		}
		else if (pid % 4 == 2)
		{
			dest = pid - 2;
			MPI_Send(&tot, count, MPI_INT, dest, tag[1], MPI_COMM_WORLD);
		}
		tot += buffer;

		//Pass data, send/recv, numP/4 -> numP/8
		if (pid % 8 == 0)
		{
			source = pid + 4;
			MPI_Recv(&buffer, count, MPI_INT, source, tag[2], MPI_COMM_WORLD, &Stat);
		}
		else if (pid % 8 == 4)
		{
			dest = pid - 4;
			MPI_Send(&tot, count, MPI_INT, dest, tag[2], MPI_COMM_WORLD);
		}
		tot += buffer;

		//Pass data, send/recv, numP/8 -> numP/16
		if (pid % 16 == 0)
		{
			source = pid + 8;
			MPI_Recv(&buffer, count, MPI_INT, source, tag[3], MPI_COMM_WORLD, &Stat);
		}
		else if (pid % 16 == 8)
		{
			dest = pid - 8;
			MPI_Send(&tot, count, MPI_INT, dest, tag[3], MPI_COMM_WORLD);
		}
		tot += buffer;

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