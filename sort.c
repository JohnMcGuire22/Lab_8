#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

int extraMemoryAllocated;

void* Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

void merge(int pData[], int l, int mid, int r) {
	int bot = mid - l + 1; //idk why i named them bot and top tbh
	int top = r - mid;

	int* temp1 = (int*)Alloc(sizeof(int)*bot);
	int* temp2 = (int*)Alloc(sizeof(int)*top);

	for (int i = 0; i < bot; i++) //fill temp arrays
		temp1[i] = pData[l + i];
	for (int j = 0; j < top; j++)
		temp2[j] = pData[mid + 1 + j];

	int i = 0, j = 0, k = l;
	while (i < bot && j < top) {
		if (temp1[i] <= temp2[j]) {
			pData[k] = temp1[i]; //lower number goes into original array, i increments
			i++;
		}
		else {
			pData[k] = temp2[j]; //lower number goes into original array, j increments
			j++;
		}
		k++; //next element of original array
	}

	while (i < bot) { //remaining elements pasted on the back end
		pData[k] = temp1[i];
		k++;
		i++;
	}
	while (j < top) { // ^above comment
		pData[k] = temp2[j];
		k++;
		j++;
	}

	DeAlloc(temp1);
	DeAlloc(temp2);
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if (l < r) {
		int mid = (l + r) / 2;
		mergeSort(pData, l, mid);
		mergeSort(pData, mid + 1, r);
		merge(pData, l, mid, r);
	}
}

// parses input file to an integer array
int parseData(char* inputFileName, int** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n, * data;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (int*)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		if (dataSz > 100) {
			for (i = 0; i < dataSz; ++i)
			{
				fscanf(inFile, "%d ", &n);
				data = *ppData + i;
				*data = n;
			}
		}
		fclose(inFile);
	}

	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	int firstHundred = (dataSz < 100 ? dataSz : 100);
	printf("\tData:\n\t");
	for (i = 0; i < firstHundred; ++i)
	{
		printf("%d ", pData[i]);
	}
	printf("\n\t");

	for (i = sz; i < dataSz; ++i)
	{
		printf("%d ", pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
	double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };

	for (i = 0; i < 4; ++i)
	{
		int* pDataSrc, * pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);

		if (dataSz <= 0)
			continue;

		pDataCopy = (int*)malloc(sizeof(int) * dataSz);

		printf("---------------------------\n");
		printf("Dataset Size : %d\n", dataSz);
		printf("---------------------------\n");

		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		free(pDataCopy);
		free(pDataSrc);
	}

}