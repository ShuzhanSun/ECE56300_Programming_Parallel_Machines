#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <mpi.h>

int main(int argc, char *argv[])
{
	const int Nrow_A = 5, Ncol_A = 6;  //3000*1200*2000 ~40s
	const int Nrow_B = Ncol_A, Ncol_B = 3;
	const int Nrow_C = Nrow_A, Ncol_C = Ncol_B;

	int rowA_LB, rowA_UB, colA_LB, colA_UB; 
	int rowB_LB, rowB_UB, colB_LB, colB_UB;
	int rowC_LB, rowC_UB, colC_LB, colC_UB;

	int i, j, k, index_local, block_i, block_j, size_blockA, size_blockB, temp_blockIndex, step;
	int rank, numP, sqrt_numP;
	double t_start, t_end;

	int prev, next, tagA = 1, tagB = 2;
	MPI_Request reqs[4];
	MPI_Status stats[4];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numP);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0)
		std::cout << "numP = " << numP << std::endl << std::endl;

	// Check if the numP satisfies
	sqrt_numP = (int) sqrt(numP);
	if (sqrt_numP != sqrt(numP)) {
		std::cout << "Error! sqrt(numP) is not integer.";
		return 0;
	}

	MPI_Barrier(MPI_COMM_WORLD);
	t_start = MPI_Wtime();

	// block index at each rank
	block_i = rank / sqrt_numP;
	block_j = rank % sqrt_numP;

	// Matrix A: the rows and columns are distributed among processes using block distribution
	rowA_LB = block_i * Nrow_A / sqrt_numP;
	rowA_UB = (block_i + 1) * Nrow_A / sqrt_numP - 1;
	colA_LB = block_j * Ncol_A / sqrt_numP;
	colA_UB = (block_j + 1) * Ncol_A / sqrt_numP - 1;

	// Matrix B: the columns are block distributed, rows are determined by A
	rowB_LB = block_i * Nrow_B / sqrt_numP;
	rowB_UB = (block_i + 1) * Nrow_B / sqrt_numP - 1;
	colB_LB = block_j * Ncol_B / sqrt_numP;
	colB_UB = (block_j + 1) * Ncol_B / sqrt_numP - 1;

	// Rearrange the blocks, block Aij shifts left i positions, block Bij shifts up j positions
	temp_blockIndex = (block_j + block_i) % sqrt_numP;
	colA_LB = temp_blockIndex * Ncol_A / sqrt_numP;
	colA_UB = (temp_blockIndex + 1) * Ncol_A / sqrt_numP - 1;
	rowB_LB = temp_blockIndex * Nrow_B / sqrt_numP;
	rowB_UB = (temp_blockIndex + 1) * Nrow_B / sqrt_numP - 1;

	// Matrix C = A*B: columns are determinded by B, rows are determined by A
	rowC_LB = rowA_LB;
	rowC_UB = rowA_UB;
	colC_LB = colB_LB;
	colC_UB = colB_UB;

	// Assigned heap size for block A and block B, larger than required
	size_blockA = (Nrow_A / sqrt_numP + 1)*(Ncol_A / sqrt_numP + 1);
	size_blockB = (Nrow_B / sqrt_numP + 1)*(Ncol_B / sqrt_numP + 1);

	//Each process generates its own local block of matrix A and B
	int* matrixA_local = new int[size_blockA];
	for (i = rowA_LB; i <= rowA_UB; i++)
		for (j = colA_LB; j <= colA_UB; j++) {
			index_local = (i - rowA_LB)*(colA_UB - colA_LB + 1) + (j - colA_LB);
			matrixA_local[index_local] = (i*Ncol_A + j);
		}
	int* matrixB_local = new int[size_blockB];
	for (i = rowB_LB; i <= rowB_UB; i++)
		for (j = colB_LB; j <= colB_UB; j++) {
			index_local = (i - rowB_LB)*(colB_UB - colB_LB + 1) + (j - colB_LB);
			matrixB_local[index_local] = (i*Ncol_B + j);
		}

	//Each process calculates local block of matrix C = A*B, step 0
	long int* matrixC_local = new long int[(rowC_UB - rowC_LB + 1) * (colC_UB - colC_LB + 1)];
	for (i = rowC_LB; i <= rowC_UB; i++)
		for (j = colC_LB; j <= colC_UB; j++) {
			index_local = (i - rowC_LB)*(colC_UB - colC_LB + 1) + (j - colC_LB);
			matrixC_local[index_local] = 0;
			for (k = rowB_LB; k <= rowB_UB; k++)
				matrixC_local[index_local] += matrixA_local[(i - rowA_LB)*(colA_UB - colA_LB + 1) + (k - colA_LB)] * 
				matrixB_local[(k - rowB_LB)*(colB_UB - colB_LB + 1) + (j - colB_LB)];
		}

	//Pass the blocks to its neighbors
	int* buffA = new int[size_blockA];
	int* buffB = new int[size_blockB];

	step = 1;
	while (step < sqrt_numP) {

		// pass A, block Aij shifts left 1 position
		prev = rank + 1;
		next = rank - 1;
		if (block_j == 0) next = rank + sqrt_numP - 1;
		if (block_j == (sqrt_numP - 1)) prev = rank - sqrt_numP + 1;

		MPI_Irecv(buffA, size_blockA, MPI_INT, prev, tagA, MPI_COMM_WORLD, &reqs[0]);
		MPI_Isend(matrixA_local, size_blockA, MPI_INT, next, tagA, MPI_COMM_WORLD, &reqs[1]);

		// pass B, block Bij shifts up 1 position
		prev = rank + sqrt_numP;
		next = rank - sqrt_numP;
		if (block_i == 0) next = rank + numP - sqrt_numP;
		if (block_i == (sqrt_numP - 1)) prev = rank - numP + sqrt_numP;

		MPI_Irecv(buffB, size_blockB, MPI_INT, prev, tagB, MPI_COMM_WORLD, &reqs[2]);
		MPI_Isend(matrixB_local, size_blockB, MPI_INT, next, tagB, MPI_COMM_WORLD, &reqs[3]);

		// retrive the blocks from buffers
		MPI_Waitall(4, reqs, stats);
		for (i = 0; i < size_blockA; i++)
			matrixA_local[i] = buffA[i];
		for (i = 0; i < size_blockB; i++)
			matrixB_local[i] = buffB[i];

		// New global indices of block A and B
		temp_blockIndex = (block_j + block_i + step) % sqrt_numP;
		colA_LB = temp_blockIndex * Ncol_A / sqrt_numP;
		colA_UB = (temp_blockIndex + 1) * Ncol_A / sqrt_numP - 1;
		rowB_LB = temp_blockIndex * Nrow_B / sqrt_numP;
		rowB_UB = (temp_blockIndex + 1) * Nrow_B / sqrt_numP - 1;

		// Add up new components to matrix C
		for (i = rowC_LB; i <= rowC_UB; i++)
			for (j = colC_LB; j <= colC_UB; j++) {
				index_local = (i - rowC_LB)*(colC_UB - colC_LB + 1) + (j - colC_LB);
				for (k = rowB_LB; k <= rowB_UB; k++)
					matrixC_local[index_local] += matrixA_local[(i - rowA_LB)*(colA_UB - colA_LB + 1) + (k - colA_LB)] *
					matrixB_local[(k - rowB_LB)*(colB_UB - colB_LB + 1) + (j - colB_LB)];
			}

		step++;
	}

	//Print elapsed time
	t_end = MPI_Wtime();
	if (rank == 0)
		std::cout << "Elapsed time = " << t_end - t_start << "s\n";

	// Output the calcualated matrix C 
	std::string filename = "LocalMatrix_rank" + std::to_string(rank) + ".txt";
	std::ofstream outfile(filename);
	outfile << "rank = " << rank << "\n\nlocal matrix C is: \n";
	for (i = rowC_LB; i <= rowC_UB; i++) {
		for (j = colC_LB; j <= colC_UB; j++) {
			index_local = (i - rowC_LB)*(colC_UB - colC_LB + 1) + (j - colC_LB);
			outfile << "C[" << i << "][" << j << "]=" << matrixC_local[index_local] << " ";
		}
		outfile << std::endl;
	}

	outfile << "local matrix A is: \n";
	for (i = rowA_LB; i <= rowA_UB; i++) {
		for (j = colA_LB; j <= colA_UB; j++) {
			index_local = (i - rowA_LB)*(colA_UB - colA_LB + 1) + (j - colA_LB);
			outfile << "A[" << i << "][" << j << "]=" << matrixA_local[index_local] << " ";
		}
		outfile << std::endl;
	}

	outfile << "local matrix B is: \n";
	for (i = rowB_LB; i <= rowB_UB; i++) {
		for (j = colB_LB; j <= colB_UB; j++) {
			index_local = (i - rowB_LB)*(colB_UB - colB_LB + 1) + (j - colB_LB);
			outfile << "B[" << i << "][" << j << "]=" << matrixB_local[index_local] << " ";
		}
		outfile << std::endl;
	}
	outfile.close();

	delete[] matrixA_local;
	delete[] matrixC_local;
	delete[] matrixB_local;
	delete[] buffA;
	delete[] buffB;

	MPI_Finalize();
	//std::cin.get();
}