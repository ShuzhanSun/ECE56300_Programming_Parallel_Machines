#include  <stdio.h>
#include  <omp.h>

int main(int argc, char *argv[]) { 
 
   char* n = argv[1];
   int numThreads = 0;

   for (int i = 0; n[i] != 0; i++) {
      numThreads = numThreads*10 + n[i] - '0';
   }

   omp_set_num_threads(numThreads);

#pragma omp parallel
   printf("Hello world from thread %d\n",omp_get_thread_num( ));

   return 0;
}



      
   
