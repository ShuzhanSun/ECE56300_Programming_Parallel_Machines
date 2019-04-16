#include <iostream>
#include <fstream>
#include <string>
#include <mpi.h>

int main(int argc, char *argv[])
{
	const int Nrow_A = 3000, Ncol_A = 2000;  //3000*1200*2000 ~40s
	const int Nrow_B = Ncol_A, Ncol_B = 2000;
	const int Nrow_C = Nrow_A, Ncol_C = Ncol_B;

	int row_LB, row_UB, i, j, k, indexC_local;
	int rank, numP;
	double t_start, t_end;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numP);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0)
		std::cout << "numP = " << numP << std::endl << std::endl;

	MPI_Barrier(MPI_COMM_WORLD);
	t_start = MPI_Wtime();

	// Generate the same matrix B at each process, B[i][j] = matrixB[i*Ncol_B+j]
	int* matrixB = new int[Nrow_B * Ncol_B];
	for (i = 0; i < Nrow_B * Ncol_B; i++)
		matrixB[i] = i % 10;

	// The rows of matrix A is distributed among processes using block distribution
	row_LB = rank * Nrow_A / numP;
	row_UB = (rank + 1) * Nrow_A / numP - 1;

	//Each process generates its own rows of matrix A
	int* matrixA_local = new int[(row_UB - row_LB + 1) * Ncol_A];
	for (i = 0; i < (row_UB - row_LB + 1) * Ncol_A; i++)
		matrixA_local[i] = (i + row_LB * Ncol_A) % 5;

	//Each process calculates local rows of matrix C = A*B
	long int* matrixC_local = new long int[(row_UB - row_LB + 1) * Ncol_C];
	for (i = row_LB; i < row_UB + 1; i++)
		for (j = 0; j < Ncol_C; j++) {
			indexC_local = i * Ncol_C + j - (row_LB*Ncol_C);
			matrixC_local[indexC_local] = 0;
			for (k = 0; k < Nrow_B; k++)
				matrixC_local[indexC_local] += matrixA_local[(i - row_LB)*Ncol_A + k] * matrixB[k*Ncol_B + j];
		}

	//Print elapsed time
	t_end = MPI_Wtime();
	if (rank == 0)
		std::cout << "Elapsed time = " << t_end - t_start << "s\n";

	//// Output the calcualated matrix C 
	//std::string filename = "LocalMatrix_rank" + std::to_string(rank) + ".txt";
	//std::ofstream outfile(filename);
	//outfile << "rank = " << rank << "\n\nlocal matrix C is: \n";
	//for (i = row_LB; i < row_UB + 1; i++) {
	//	for (j = 0; j < Ncol_C; j++) {
	//		indexC_local = i * Ncol_C + j - (row_LB*Ncol_C);
	//		outfile << "C[" << i << "][" << j << "]=" <<  matrixC_local[indexC_local] << " ";
	//	}
	//	outfile << std::endl;
	//}
	//// local matrix A
	//outfile << "local matrix A is: \n";
	//for (i = row_LB; i < row_UB + 1; i++) {
	//	for (j = 0; j < Ncol_A; j++) {
	//		outfile << "A[" << i << "][" << j << "]=" << matrixA_local[(i - row_LB) * Ncol_A + j] << " ";
	//	}
	//	outfile << std::endl;
	//}
	//// matrix B
	//outfile << "matrix B is: \n";
	//for (i = 0; i < Nrow_B; i++) {
	//	for (j = 0; j < Ncol_B; j++) {
	//		outfile << "B[" << i << "][" << j << "]=" << matrixB[i*Ncol_B + j] << " ";
	//	}
	//	outfile << std::endl;
	//}
	//outfile.close();

	delete[] matrixA_local;
	delete[] matrixC_local;
	delete[] matrixB;

	MPI_Finalize();
	//std::cin.get();
}