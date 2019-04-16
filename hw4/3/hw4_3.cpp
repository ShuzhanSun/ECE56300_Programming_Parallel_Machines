#include <stdio.h>
#include <omp.h>
#include <vector>

using namespace std;

vector<vector <double> > MatrixMultiply(vector <vector<double> > mat1, vector<vector <double> > mat2)
{
	int r_mat1 = mat1.size();
	int c_mat1 = mat1[0].size();
	int r_mat2 = mat2.size();
	int c_mat2 = mat2[0].size();

	if (c_mat1 != r_mat2)
	{
		printf("Dimensions do not match! Wrong multiplication! \n");
	}
		
	int i, j, k;
	vector<vector <double> > res (r_mat1, vector<double> (c_mat2, 0));

	for (i = 0; i < r_mat1; i++)
		for (j = 0; j < c_mat2; j++)
		{
			for (k = 0; k < c_mat1; k++)
				res[i][j] += mat1[i][k] * mat2[k][j];
		}

	return res;
}

int PrintMatrix(vector <vector<double> > mat) 
{
	for (int i = 0; i < mat.size(); i++)
	{
		for (int j = 0; j < mat[0].size(); j++)
			printf("%f    ", mat[i][j]);
		printf("\n");
	}
	return 0;
}

int main()
{
	int tid1, tid2;
	vector< vector<double> > mat1, mat2, mat3, multi1, multi2;
	mat1 = {{1,2,3},
			{4,5,6}
			};
	mat2 = { {1,1},{1,1},{1,1} };
	mat3 = { {1},{1},{1} };

#pragma omp parallel sections
	{
#pragma omp section
		{
			multi1 = MatrixMultiply(mat1, mat2);
			tid1 = omp_get_thread_num();
		}
#pragma omp section
		{
			multi2 = MatrixMultiply(mat1, mat3);
			tid2 = omp_get_thread_num();
		}
	}
	printf("matrix1 = \n");
	PrintMatrix(mat1);
	printf("matrix2 = \n");
	PrintMatrix(mat2);
	printf("matrix3 = \n");
	PrintMatrix(mat3);
	printf("Thread id = %d, matrix1 * matrix 2= \n",tid1);
	PrintMatrix(multi1);
	printf("Thread id = %d, matrix1 * matrix 3= \n",tid2);
	PrintMatrix(multi2);
}