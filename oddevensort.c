
/*

//compilation
$  gcc -g -Wall -fopenmp -o main main.c

//4 threads , 1 = two_parallel_for_directives, 2 = two_for_directives
$  ./main 4 1
$  ./main 4 2

//8 threads , 1 = two_parallel_for_directives, 2 = two_for_directives
$  ./main 8 1
$  ./main 8 2

//16 threads , 1 = two_parallel_for_directives, 2 = two_for_directives
$  ./main 16 1
$  ./main 16 2




*/


#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int a[20020];
int n;

void two_parallel_for_directives(int thread_count)
{
	int i, tmp, phase;

	for (phase = 0; phase < n; phase++) {
		if (phase % 2 == 0)
			#pragma omp parallel for num_threads(thread_count) \
			default(none) shared(a, n) private(i, tmp)
			for (i = 1; i < n; i += 2) {
				if (a[i-1] > a[i]) {
					tmp = a[i-1];	
					a[i-1] = a[i];
					a[i] = tmp;
				}
			}
		else
			#pragma omp parallel for num_threads(thread_count) \
			default(none) shared(a, n) private(i, tmp)
			for (i = 1; i < n-1; i += 2) {
				if (a[i] > a[i+1]) {
					tmp = a[i+1];
					a[i+1] = a[i];
					a[i] = tmp;
				}
			}
	}

}

void two_for_directives(int thread_count)
{
	int i, tmp, phase;

	#pragma omp parallel for num_threads(thread_count) \
			default(none) shared(a, n) private(i, tmp)
	for (phase = 0; phase < n; phase++) {
		if (phase % 2 == 0)
			#pragma omp parallel for 
			for (i = 1; i < n; i += 2) {
				if (a[i-1] > a[i]) {
					tmp = a[i-1];	
					a[i-1] = a[i];
					a[i] = tmp;
				}
			}
		else
			#pragma omp parallel for 
			for (i = 1; i < n-1; i += 2) {
				if (a[i] > a[i+1]) {
					tmp = a[i+1];
					a[i+1] = a[i];
					a[i] = tmp;
				}
			}
	}
}


int main(int argc, char *argv[])
{

	int thread_count = strtol(argv[1], NULL, 10);
	int implementation = strtol(argv[2], NULL, 10);

	n = 20000;
	for(int i=0; i<n; ++i)
	{
		a[i] = rand()%n;
	}


	float start, finish;

	//-------------------------------------------
	if(implementation == 1){
		start = omp_get_wtime();
		two_parallel_for_directives(thread_count);
		finish = omp_get_wtime();
		printf("Two parallel for directives with %d threads: %f seconds.\n", thread_count, finish-start);
	}
	//-------------------------------------------
	if(implementation == 2){
		start = omp_get_wtime();
		two_for_directives(thread_count);
		finish = omp_get_wtime();
		printf("Two for directives with %d threads: %f seconds.\n", thread_count, finish-start);
	}

	return 0;
}