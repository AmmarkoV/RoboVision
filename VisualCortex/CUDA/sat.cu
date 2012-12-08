#include <stdio.h>
#include <string.h>
#include <stdlib.h>

 * main.c

int WIDTH = 1920;
int HEIGHT = 1080;
int THREADS_PER_BLOCK_1 = 256;
int THREADS_PER_BLOCK_2 = 128;
#define REPEAT_TIMES 177

char * device_inputArray;
unsigned int * device_outputArray;

__global__
void sumRow1(char *inputArray, unsigned int inputByteSize, unsigned int rowSize,
		unsigned int *outputArray)
{
	int row = blockIdx.x * blockDim.x + threadIdx.x;
	int index = row * rowSize;

	char * rowPos = inputArray + index;

	if (rowPos + rowSize < inputArray + inputByteSize)
	{
		outputArray[index] = inputArray[index];

		int i;
		int idx;
		for (i = 1; i < rowSize; i++)
		{
			idx = index + i;
			outputArray[idx] = outputArray[idx - 1] + inputArray[idx];
		}
	}
}

__global__
void sumColumn1(unsigned int *outputArray, unsigned int outputByteSize,
		unsigned int rowSize, unsigned int columnSize)
{
	int column = blockIdx.x * blockDim.x + threadIdx.x;
	int index = column;

	unsigned int * columnPos = outputArray + index;
	unsigned int * columnLimit = columnPos + (rowSize * (columnSize - 1));

	if (columnLimit < outputArray + outputByteSize)
	{
		int i;
		int idx;
		int offset;
		for (i = 1; i < columnSize; i++)
		{
			offset = i * rowSize;
			idx = index + offset;
			outputArray[idx] = outputArray[idx - offset] + outputArray[idx];
		}
	}
}

__global__
void sumRow2(char *inputArray, unsigned int inputByteSize, unsigned int rowSize,
		unsigned int columnSize, unsigned int *outputArray)
{
	//         BLOCKID        32         THREADID
	int row = blockIdx.x * blockDim.x + threadIdx.x;
	if (row < columnSize)
	{
		char * rowPos = inputArray + row * rowSize;
		char * rowLimit = rowPos + rowSize;

		unsigned int * outPrev = outputArray + row * rowSize;
		unsigned int * out = outPrev + 1;

		*outPrev = *rowPos;
		++rowPos;

		while (rowPos < rowLimit)
		{
			*out = *outPrev + *rowPos;
			++out;
			++outPrev;
			++rowPos;
		}
	}
}

__global__
void sumColumn2(unsigned int *outputArray, unsigned int outputByteSize,
		unsigned int rowSize, unsigned int columnSize)
{
	int column = blockIdx.x * blockDim.x + threadIdx.x;
	if (column < rowSize)
	{
		unsigned int * columnPos = outputArray + column;
		unsigned int * columnLimit = columnPos + (rowSize * (columnSize - 1));

		//rowSizeCONST
		unsigned int * outPrev = columnPos;
		unsigned int * out = columnPos + rowSize;

		while (out < columnLimit)
		{
			*out += *outPrev;
			out += rowSize;
			outPrev += rowSize;
		}
	}
}

void Preconditions_checkMemoryAllocation(void * array)
{
	if (!array)
	{
		printf("Memory allocation was not successful\n");
		exit(1);
	}
}


void printOutput(unsigned int * output, unsigned int width, unsigned int height)
{
	printf("Output \n");
	int x, y;
	for (y = 0; y < 7; y++)
	{
		for (x = 0; x < width; x++)
		{
			printf("%u ", output[y * width + x]);
		}
		printf("\n");
	}
}

int InitializeCUDASAT(int height, int width, int threads1, int threads2)
{
	WIDTH = width;
	HEIGHT = height;
	THREADS_PER_BLOCK_1 = threads1;
	THREADS_PER_BLOCK_2 = threads2;

	unsigned int inputByteSize = WIDTH * HEIGHT * sizeof(char);
	cudaMalloc((void**) &device_inputArray, inputByteSize);

	unsigned int outputByteSize = WIDTH * HEIGHT * sizeof(unsigned int);
	cudaMalloc((void**) &device_outputArray, outputByteSize);

	return 1;
}

int closeCUDASAT()
{
	cudaFree(device_inputArray);
	cudaFree(device_outputArray);

	return 1;
}

int main()
{
	InitializeCUDASAT(WIDTH, HEIGHT, THREADS_PER_BLOCK_1, THREADS_PER_BLOCK_2);

	unsigned int inputByteSize = WIDTH * HEIGHT * sizeof(char);
	char * inputArray = (char *) malloc(inputByteSize);
	Preconditions_checkMemoryAllocation((void*) inputArray);

	unsigned int outputByteSize = WIDTH * HEIGHT * sizeof(unsigned int);
	unsigned int * outputArray = (unsigned int *) malloc(outputByteSize);
	Preconditions_checkMemoryAllocation((void*) outputArray);

	int i = 0;

	for (i = 0; i < REPEAT_TIMES; i++)
	{

		memset(inputArray, 1, inputByteSize);
		cudaMemcpy(device_inputArray, inputArray, inputByteSize,
				cudaMemcpyHostToDevice);

		int Blocks = (HEIGHT - 1) / THREADS_PER_BLOCK_1 + 1;
		printf("Gonna use %u blocks and %u threads for sumRow\n", Blocks,
				THREADS_PER_BLOCK_1);
		sumRow2<<<Blocks, THREADS_PER_BLOCK_1>>>(device_inputArray,
				inputByteSize, WIDTH, HEIGHT, device_outputArray);

		Blocks = (WIDTH - 1) / THREADS_PER_BLOCK_2 + 1;
		printf("Gonna use %u blocks and %u threads for sumRow\n", Blocks,
				THREADS_PER_BLOCK_2);
		sumColumn2<<<Blocks, THREADS_PER_BLOCK_2>>>(device_outputArray,
				outputByteSize, WIDTH, HEIGHT);

		cudaMemcpy(outputArray, device_outputArray, outputByteSize,
				cudaMemcpyDeviceToHost);

	}

	printOutput(outputArray, WIDTH, HEIGHT);

	closeCUDASAT();

	free(inputArray);
	free(outputArray);
}

