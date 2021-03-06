// PP_Sorting.c : Defines the entry point for the console application.
//

#include <stdlib.h> 
#include <stdio.h>  
#include <malloc.h>  
#include <omp.h>
#include <time.h>

void SerialCount_sort(int a[], int n);
void ParallelCount_sort(int a[], int n, int thread_count);
#define numberCount 10000

int cmpfunc(const void * a, const void * b) {
	return (*(int*)a - *(int*)b);
}
//References:
//clock/timing found at: https://stackoverflow.com/questions/5248915/execution-time-of-c-program
//qsort: https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm

int main()
{
	int thread_count = 4;
	int numberArray[numberCount];
	//create random array with 10000 numbers anywhere from 0-15000
	for(int i = 0; i < numberCount; i++)
		numberArray[i] = rand() % 15001;
	//Call Serial Sort and track time it took
	clock_t serialBegin = clock();
	SerialCount_sort(numberArray, numberCount);
	clock_t serialEnd = clock();
	double serialTime_spent = (double)(serialEnd - serialBegin) / CLOCKS_PER_SEC;
	printf("Serial Sort took: %f seconds\n", serialTime_spent);
	//Randomize Array again
	for (int i = 0; i < numberCount; i++)
		numberArray[i] = rand() % 5001;
	//Call Parallel Sort and track time it took
	clock_t parallelSortbegin = clock();
	ParallelCount_sort(numberArray, numberCount, thread_count);
	clock_t parallelSortend = clock();
	double parallelSorttime_spent = (double)(parallelSortend - parallelSortbegin) / CLOCKS_PER_SEC;
	printf("Parallel Sort took: %f seconds\n", parallelSorttime_spent);
	//Randomize Array again
	for (int i = 0; i < numberCount; i++)
		numberArray[i] = rand() % 5001;
	//Call q sort and track time it took
	clock_t qsortbegin = clock();
	qsort(numberArray, numberCount, sizeof(int), cmpfunc);
	clock_t qsortend = clock();
	double qsorttime_spent = (double)(qsortend - qsortbegin) / CLOCKS_PER_SEC;
	printf("Quick Sort took: %f seconds\n", qsorttime_spent);

    return 0;
}

//Parallel Count algorithm modified slightly from the serial problem 5.3
void ParallelCount_sort(int a[], int n, int thread_count) {
	int i, j, count;
	int * temp = malloc(n * sizeof(int));
#pragma omp parallel for num_threads(thread_count) \
   default(none) private(i, j, count) shared(a, n, temp, thread_count)
	for (i = 0; i < n; i++) {
		count = 0;
		for (j = 0; j < n; j++)
			if (a[j] < a[i])
				count++;
			else if (a[j] == a[i] && j < i)
				count++;
		temp[count] = a[i];
	}
	memcpy(a, temp, n * sizeof(int));
	free(temp);
}

//Serial Count algorithm from problem 5.3
void SerialCount_sort(int a[], int n) {
	int i, j, count;
	int * temp = malloc(n * sizeof(int));
	for (i = 0; i < n; i++) {
		count = 0;
		for (j = 0; j < n; j++)
			if (a[j] < a[i])
				count++;
			else if (a[j] == a[i] && j < i)
				count++;
		temp[count] = a[i];
	}
	memcpy(a, temp, n * sizeof(int));
	free(temp);
}
