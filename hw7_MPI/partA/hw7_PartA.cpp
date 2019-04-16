#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[])
{
	int pid, numP, len;
	char name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numP);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	MPI_Get_processor_name(name, &len);
	std::cout << "Hello World from processor " << name << ", rank (pid): " << pid << std::endl;
	MPI_Finalize();
}