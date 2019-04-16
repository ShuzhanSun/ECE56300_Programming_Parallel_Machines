#include <iostream>
#include <chrono>
#include <ctime>

int main(int argc, char *argv[])
{
	const int Nrow_A = 3000, Ncol_A = 2000; 
	const int Nrow_B = Ncol_A, Ncol_B = 2000;
	const int Nrow_C = Nrow_A, Ncol_C = Ncol_B;

	int i, j, k, indexC_local;

	auto start = std::chrono::system_clock::now();

	// Generate the same matrix B at each process, B[i][j] = matrixB[i*Ncol_B+j]
	int* matrixB = new int[Nrow_B * Ncol_B];
	for (i = 0; i < Nrow_B * Ncol_B; i++)
		matrixB[i] = i % 10;

	//Each process generates its own rows of matrix A
	int* matrixA = new int[Nrow_A * Ncol_A];
	for (i = 0; i < Nrow_A * Ncol_A; i++)
		matrixA[i] = i % 5;

	//Each process calculates local rows of matrix C = A*B
	long int* matrixC = new long int[Nrow_C * Ncol_C];
	for (i = 0; i < Nrow_C; i++)
		for (j = 0; j < Ncol_C; j++) {
			indexC_local = i * Ncol_C + j;
			matrixC[indexC_local] = 0;
			for (k = 0; k < Nrow_B; k++)
				matrixC[indexC_local] += matrixA[i*Ncol_A + k] * matrixB[k*Ncol_B + j];
		}

	//Print elapsed time
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "Elapsed time = " << elapsed_seconds.count() << "s\n";

	delete[] matrixA;
	delete[] matrixC;
	delete[] matrixB;
}