#include <iostream>
#include <fstream>
#include "mkl.h"
#include <chrono>
#include <ctime>

int main(int argc, char *argv[])
{
	const int Nrow_A = 5, Ncol_A = 6;  //3000*1200*2000 ~40s
	const int Nrow_B = Ncol_A, Ncol_B = 3;
	const int Nrow_C = Nrow_A, Ncol_C = Ncol_B;

	int i, j;

	// Print the number of process
	std::cout << "max numP = " << mkl_get_max_threads() << std::endl;
	i = 4;
	mkl_set_num_threads(i);
	std::cout << "used numP = " << i << std::endl;

	auto start = std::chrono::system_clock::now();

	// Use MKL 
	double alpha = 1.0, beta = 0.0;

	double* A = (double *)mkl_malloc(Nrow_A*Ncol_A * sizeof(double), 64);
	double* B = (double *)mkl_malloc(Ncol_A*Ncol_B * sizeof(double), 64);
	double* C = (double *)mkl_malloc(Nrow_A*Ncol_B * sizeof(double), 64);

	if (A == NULL || B == NULL || C == NULL) {
		std::cout << "\n ERROR: Can't allocate memory for matrices. Aborting... \n\n";
		mkl_free(A);
		mkl_free(B);
		mkl_free(C);
		return 1;
	}

	//Intializing matrix data
	for (i = 0; i < (Nrow_A*Ncol_A); i++) {
		A[i] = (double)(i);
	}

	for (i = 0; i < (Ncol_A*Ncol_B); i++) {
		B[i] = (double)(i);
	}

	for (i = 0; i < (Nrow_A*Ncol_B); i++) {
		C[i] = 0.0;
	}

	//Computing matrix product using Intel(R) MKL dgemm function via CBLAS interface
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
		Nrow_A, Ncol_B, Ncol_A, alpha, A, Ncol_A, B, Ncol_B, beta, C, Ncol_B);

	//Print elapsed time
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "Elapsed time = " << elapsed_seconds.count() << "s\n";

	// Output the calcualated matrix C 
	std::string filename = "OutputMatrix.txt";
	std::ofstream outfile(filename);
	for (i = 0; i < Nrow_C; i++) {
		for (j = 0; j < Ncol_C; j++) {
			outfile << "C[" << i << "][" << j << "]=" << C[i* Ncol_C + j] << " ";
		}
		outfile << std::endl;
	}

	outfile << "local matrix A is: \n";
	for (i = 0; i < Nrow_A; i++) {
		for (j = 0; j < Ncol_A; j++) {
			outfile << "A[" << i << "][" << j << "]=" << A[i* Ncol_A + j] << " ";
		}
		outfile << std::endl;
	}

	outfile << "local matrix B is: \n";
	for (i = 0; i < Nrow_B; i++) {
		for (j = 0; j < Ncol_B; j++) {
			outfile << "B[" << i << "][" << j << "]=" << B[i* Ncol_B + j] << " ";
		}
		outfile << std::endl;
	}
	outfile.close();

	//Deallocating memory
	mkl_free(A);
	mkl_free(B);
	mkl_free(C);

	//std::cin.get();
}