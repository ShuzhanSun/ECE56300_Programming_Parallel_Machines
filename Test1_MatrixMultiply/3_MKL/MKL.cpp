#include <iostream>
#include <fstream>
#include "mkl.h"
#include <chrono>
#include <ctime>

int main(int argc, char *argv[])
{
	const int Nrow_A = 3000, Ncol_A = 2000;  //3000*1200*2000 ~40s
	const int Nrow_B = Ncol_A, Ncol_B = 2000;
	const int Nrow_C = Nrow_A, Ncol_C = Ncol_B;

	int i, ntest, numP[5] = { 1, 2, 4, 8, 16};
	double *A, *B, *C;
	double alpha, beta;
	int max_numP = mkl_get_max_threads();

	for (ntest = 0; ntest < 5; ntest++) {

		// Set the number of process
		mkl_set_num_threads(numP[ntest]);
		if (numP[ntest] <= max_numP)
			std::cout << "used numP = " << numP[ntest] << std::endl;
		else {
			std::cout << "Insufficient number of threads \n";
			return 1;
		}

		auto start = std::chrono::system_clock::now();

		// Use MKL 
		alpha = 1.0, beta = 0.0;

		A = (double *)mkl_malloc(Nrow_A*Ncol_A * sizeof(double), 64);
		B = (double *)mkl_malloc(Ncol_A*Ncol_B * sizeof(double), 64);
		C = (double *)mkl_malloc(Nrow_A*Ncol_B * sizeof(double), 64);

		if (A == NULL || B == NULL || C == NULL) {
			std::cout << "\n ERROR: Can't allocate memory for matrices. Aborting... \n\n";
			mkl_free(A);
			mkl_free(B);
			mkl_free(C);
			return 1;
		}

		//Intializing matrix data
		for (i = 0; i < (Nrow_A*Ncol_A); i++) {
			A[i] = (double)((i % 5));
		}

		for (i = 0; i < (Ncol_A*Ncol_B); i++) {
			B[i] = (double)((i % 10));
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

		//Deallocating memory
		mkl_free(A);
		mkl_free(B);
		mkl_free(C);
	}

	//// Output the calcualated matrix C 
	//std::string filename = "OutputMatrix.txt";
	//std::ofstream outfile(filename);
	//for (i = 0; i < Nrow_C; i++) {
	//	for (j = 0; j < Ncol_C; j++) {
	//		outfile << "C[" << i << "][" << j << "]=" << C[i* Ncol_C + j] << " ";
	//	}
	//	outfile << std::endl;
	//}

	//outfile << "local matrix A is: \n";
	//for (i = 0; i < Nrow_A; i++) {
	//	for (j = 0; j < Ncol_A; j++) {
	//		outfile << "A[" << i << "][" << j << "]=" << A[i* Ncol_A + j] << " ";
	//	}
	//	outfile << std::endl;
	//}

	//outfile << "local matrix B is: \n";
	//for (i = 0; i < Nrow_B; i++) {
	//	for (j = 0; j < Ncol_B; j++) {
	//		outfile << "B[" << i << "][" << j << "]=" << B[i* Ncol_B + j] << " ";
	//	}
	//	outfile << std::endl;
	//}
	//outfile.close();
}