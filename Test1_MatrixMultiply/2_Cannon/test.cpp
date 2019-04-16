#include <cmath>
#include <iostream>

int main(int argc, char *argv[])
{
	const int Nrow_A = 3, Ncol_A = 3;  //3000*1200*2000 ~40s
	const int Nrow_B = Ncol_A, Ncol_B = 2;
	const int Nrow_C = Ncol_A, Ncol_C = Ncol_B;

	int rowA_LB, rowA_UB, colA_LB, colA_UB;
	int rowB_LB, rowB_UB, colB_LB, colB_UB;
	int rowC_LB, rowC_UB, colC_LB, colC_UB;

	int i, j, k, index_local;
	int rank, numP, sqrt_numP;
	double t_start, t_end;

	numP = 4;
	sqrt_numP = (int)sqrt(numP);
	for (rank = 0; rank < numP; rank++) {
		std::cout << std::endl << "rank = " << rank << std::endl;

		// Matrix A: the rows and columns are distributed among processes using block distribution
		rowA_LB = (rank / sqrt_numP) * Nrow_A / sqrt_numP;
		rowA_UB = ((rank / sqrt_numP) + 1) * Nrow_A / sqrt_numP - 1;
		colA_LB = (rank % sqrt_numP) * Ncol_A / sqrt_numP;
		colA_UB = ((rank % sqrt_numP) + 1) * Ncol_A / sqrt_numP - 1;
		std::cout << "A: " << rowA_LB << rowA_UB << colA_LB << colA_UB << std::endl;

		// Matrix B: the columns are block distributed, rows are determined by A
		rowB_LB = (rank / sqrt_numP) * Nrow_B / sqrt_numP;
		rowB_UB = ((rank / sqrt_numP) + 1) * Nrow_B / sqrt_numP - 1;
		colB_LB = (rank % sqrt_numP) * Ncol_B / sqrt_numP;
		colB_UB = ((rank % sqrt_numP) + 1) * Ncol_B / sqrt_numP - 1;
		std::cout << "B: " << rowB_LB << rowB_UB << colB_LB << colB_UB << std::endl;

		// Rearrange the blocks, block Aij shifts left i positions, block Bij shifts up j positions
		int j_orig = rank % sqrt_numP + rank / sqrt_numP;
		j_orig = j_orig % sqrt_numP;
		colA_LB = j_orig * Ncol_A / sqrt_numP;
		colA_UB = (j_orig + 1) * Ncol_A / sqrt_numP - 1;
		int i_orig = rank % sqrt_numP + rank / sqrt_numP;
		i_orig = i_orig % sqrt_numP;
		rowB_LB = i_orig * Nrow_B / sqrt_numP;
		rowB_UB = (i_orig + 1) * Nrow_B / sqrt_numP - 1;
		std::cout << "Rearranged A: " << rowA_LB << rowA_UB << colA_LB << colA_UB << std::endl;
		std::cout << "Rearranged B: " << rowB_LB << rowB_UB << colB_LB << colB_UB << std::endl;

		// Matrix C = A*B: columns are determinded by B, rows are determined by A
		rowC_LB = rowA_LB;
		rowC_UB = rowA_UB;
		colC_LB = colB_LB;
		colC_UB = colB_UB;
	}
	std::cin.get();
}