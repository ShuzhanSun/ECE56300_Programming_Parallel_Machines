#include <omp.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <ctime>
using namespace std;

int countNum = 0;

struct node {
   double val;
   struct node* l;
   struct node* r;

   int NodeLevel;
} nodeT;

#define MAXLEVEL 19
#define SEQ_LEVEL 5  //the num of nodes at this level is larger than the num of cores 16

struct node* build(int level) {

   if (level < MAXLEVEL) {
      struct node* p = (struct node*) malloc(sizeof(nodeT));
#pragma omp task untied if(level < SEQ_LEVEL)
      p->l = build(level+1);
#pragma omp task untied if(level < SEQ_LEVEL)
      p->r = build(level+1);
#pragma omp taskwait
	  p->val = (double)rand() / RAND_MAX;
	  p->NodeLevel = level;
      return p;
   } else {
      return NULL;
  }
}

void traverse(struct node* p) {
   if (p == NULL) return;
   if (p->l == NULL) return;
   else 
#pragma omp task untied if(p->NodeLevel < SEQ_LEVEL)
	   traverse(p->l);
   if (p->r == NULL) return;
   else 
#pragma omp task untied if(p->NodeLevel < SEQ_LEVEL)
	   traverse(p->r);
#pragma omp taskwait
	printf("%f\n", p->val);
	if (p->val < 0.5) 
		countNum++;
}

int main( ) {

	auto start = chrono::system_clock::now();
	
	int nt = omp_get_num_procs();
	omp_set_num_threads(nt);

	struct node* h;
#pragma omp parallel
#pragma omp single
	h = build(0);

#pragma omp parallel
#pragma omp single
	traverse(h);

	auto end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;
	cout << "Number of nodes whose value < 0.5 : " << countNum <<endl 
		<< "elapsed time: " << elapsed_seconds.count() << "s\n";
	//getchar();
	free(h);
}
