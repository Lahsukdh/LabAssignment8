#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
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

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void merge(int pData[], int l, int mid, int r){
    int size1 = mid-l+1;
    int size2 = r-mid;

    int t1 = 0;
    int t2 = 0;
    int t3 = l;
    
    //left and right arrays
    int* lArr = (int*) Alloc(sizeof(int) * size1);
    int* rArr = (int*) Alloc(sizeof(int) * size2);

    //copy data from pData to the left and right arrays
    for(int i = 0; i < size1; i++){
        lArr[i] = pData[l+i];
    }
    for(int i = 0; i < size2; i++){
        rArr[i] = pData[mid+1+i];
    }

    //merge arrays into one
    while(t1 < size1 && t2 < size2){
        if(lArr[t1] <= rArr[t2]){
            pData[t3] = lArr[t1];
            t1++;
        }
        else{
            pData[t3] = rArr[t2];
            t2++;
        }
        t3++;
    }

    //if elements remain in either left or right, dump them into pData
    while(t1 < size1){
        pData[t3] = lArr[t1];
        t1++;
        t3++;
    }
    DeAlloc(lArr);
    while(t2 < size2){
        pData[t3] = rArr[t2];
        t2++;
        t3++;
    }
    DeAlloc(rArr);
}

void mergeSort(int pData[], int l, int r)
{
    //checks if there are more than one element
    if(l < r){  
        int mid = l+(r-l)/2;
        mergeSort(pData, l, mid);
        mergeSort(pData, mid+1, r);

        merge(pData, l, mid, r);

    }
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
    //I changed sz to equal half of the data size to traverse both halves of the array
	int i, sz = dataSz/2;
	printf("\tData:\n\t");

    //print first half
	for (i=0;i<sz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
    //print second half
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}