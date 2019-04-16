#include <mpi.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <ctime>
using namespace std;

int RootLevel_Subtree = 4;  // 16 processors
int MaxLevel = 19;
int MAXLEVEL = MaxLevel;
int countNum = 0;

struct node {
   double val;
   struct node* l;
   struct node* r;

   int NodeLevel;
} nodeT;

struct node* build(int level) {

   if (level < MAXLEVEL) {
      struct node* p = (struct node*) malloc(sizeof(nodeT));
	  p->val = (double)rand() / RAND_MAX;
	  p->NodeLevel = level;
      p->l = build(level+1);
      p->r = build(level+1);
      return p;
   } else {
      return NULL;
  }
}

void traverse(struct node* p) {
   if (p == NULL) return;
   if (p->l == NULL) return;
   else traverse(p->l);
   if (p->r == NULL) return;
   else traverse(p->r);
   printf("%f\n", p->val);
   if (p->val < 0.5) countNum++;
}

int main(int argc, char *argv[]) {
	//auto start = chrono::system_clock::now();

	int numP, rank, recvbuf;
	struct node *h, *h_subtree;

	MPI_Init(&argc, &argv);

	double t_start, t_end;
	MPI_Barrier(MPI_COMM_WORLD);
	t_start = MPI_Wtime();

	MPI_Comm_size(MPI_COMM_WORLD, &numP);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	srand(rank);

	//Process 0 deals with the accendents of the subtrees
	if (rank == 0)
	{
		MAXLEVEL = RootLevel_Subtree + 1;
		h = build(0);
		traverse(h);

		MAXLEVEL = MaxLevel;
	}

	//Each process initializes its subtree locally and counts the number
	h_subtree = build(RootLevel_Subtree);
	traverse(h_subtree);

	//Collective communication
	MPI_Reduce(&countNum, &recvbuf, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	//auto end = chrono::system_clock::now();
	//chrono::duration<double> elapsed_seconds = end - start;
	t_end = MPI_Wtime();
	if (rank == 0)
	{
		cout << "Number of nodes whose value < 0.5 : " << recvbuf << endl
			<< "elapsed time: " << t_end - t_start << "s\n";
	}

	free(h);
	free(h_subtree);
	MPI_Finalize();
}
