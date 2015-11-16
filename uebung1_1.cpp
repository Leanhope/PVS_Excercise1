#include <stdio.h> //included input and output
#include <omp.h> //included openMP

int main(int argc, char* argv[])
{
	int numThreads; //declared integer for storage of count of Threads
	int threadID; //declared integer for display of the current thread
	float start, end; //starttime and endtime

	start = omp_get_wtime(); //saves elapsed clock time in seconds. Measured from "some time in the past".

	/*starts the parallelization. Pragma signals that the compiler
	executes the following command, in this case from the omp library*/
	#pragma omp parallel num_threads(5000)
	{
		threadID = omp_get_thread_num();
		printf("Hello from thread %d\n", threadID);

		/*If the Thread with the ID 0  is executing, print the total number of threads*/
		if(threadID == 0)
		{
			numThreads = omp_get_num_threads(); //omp function getting the total number of threads
			printf("Number of threads: %d\n", numThreads);
		}
	}

	end = omp_get_wtime();
	printf("This task took %f seconds\n", end-start);
}

//1.3)
/*Increasing the parameter num_threads increases the execution time. 
If the amount is increased to about 20000, the thread creation failes,
with the reason that the resource is temporarily unavailable.
Increasing to about 100000 causes a segmentation fault. Interesting is
aswell, that the threads are printed in random order.*/