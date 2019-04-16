#include <iostream>
#include <vector>

class DataDistr
{
	// First array index = 0, first process id = 0

public:
	enum DistrType	{ block, cyclic, block_cyclic };
	DistrType dtype;

	int numP, N, ChunkSize;	//# of processors, array size

	int LowerBound(int pid)
	{
		switch(dtype) 
		{
			case block: 
				return pid * N / numP;
			case cyclic:
				return pid;
			default:
				return NULL;
		}
	}

	int UpperBound(int pid)
	{
		switch (dtype)
		{
		case block:
			return (pid+1) * N / numP - 1;
		case cyclic:
		{
			if (pid < N % numP)
				return pid + (N / numP) * numP;
			else
				return pid + (N / numP - 1) * numP;
		}
		default:
			return NULL;
		}
	}

	int ElemDistance(int pid)
	{
		switch (dtype)
		{
		case block:
			return 1;
		case cyclic:
			return numP;
		default:
			return NULL;
		}
	}

	std::vector<int> allIndices(int pid)
	{
		std::vector<int> arr;
		if (dtype != block_cyclic)
			return arr;

		int num1cyc = numP * ChunkSize;
		int i, j;

		for (i = 0; i < N / num1cyc; i++)
			for (j = 0; j < ChunkSize; j++)
				arr.push_back(pid * ChunkSize + j + i * num1cyc);

		int res = N % num1cyc;

		if (pid < res/ChunkSize)
			for (j = 0; j < ChunkSize; j++)
				arr.push_back(pid * ChunkSize + j + (N / num1cyc) * num1cyc);
		else if (pid == res / ChunkSize)
			for (j = 0; j < res % ChunkSize; j++)
				arr.push_back(pid * ChunkSize + j + (N / num1cyc) * num1cyc);

		return arr;
	}
};

int main()
{
	DataDistr DD;
	DD.numP = 7;
	DD.N = 101;
	DD.ChunkSize = 3;

	int pid, i;
	std::vector<int> arr;

	DD.dtype = DataDistr::DistrType::block;
	std::cout << "block: \n";
	for (pid = 0; pid < DD.numP; pid++)
		std::cout << pid << ": [" << DD.LowerBound(pid) << " : " << DD.UpperBound(pid)
		<< " : " << DD.ElemDistance(pid) << "]\n";

	DD.dtype = DataDistr::DistrType::cyclic;
	std::cout << "cyclic: \n";
	for (pid = 0; pid < DD.numP; pid++)
		std::cout << pid << ": [" << DD.LowerBound(pid) << " : " << DD.UpperBound(pid)
		<< " : " << DD.ElemDistance(pid) << "]\n";

	DD.dtype = DataDistr::DistrType::block_cyclic;
	std::cout << "block_cyclic: \n";
	for (pid = 0; pid < DD.numP; pid++)
	{
		arr = DD.allIndices(pid);
		std::cout << pid << ": [";
		for (i = 0; i < arr.size(); i++)
			std::cout << arr[i] << " ";
		std::cout << "]\n";
	}

	std::cin.get();
}