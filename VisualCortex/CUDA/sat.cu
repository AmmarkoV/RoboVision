#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int WIDTH = 1920;
int HEIGHT = 1080;
int THREADS_PER_BLOCK_1 = 256;
int THREADS_PER_BLOCK_2 = 128;
#define REPEAT_TIMES 1

char * device_inputArray;
unsigned int * device_outputArray;
__global__
void sumRow1(char *inputArray, unsigned int inputByteSize,
		unsigned int totalRows, unsigned int totalColumns,
		unsigned int *outputArray)
{
	int row = blockIdx.x * blockDim.x + threadIdx.x;
	if (row < totalRows)
	{
		int index = row * totalColumns;
		outputArray[index] = inputArray[index];

		int i;
		int idx;
		for (i = 1; i < totalColumns; i++)
		{
			idx = index + i;
			outputArray[idx] = outputArray[idx - 1] + inputArray[idx];
		}
	}
}

__global__
void sumColumn1(unsigned int *outputArray, unsigned int outputByteSize,
		unsigned int totalRows, unsigned int totalColumns)
{
	int column = blockIdx.x * blockDim.x + threadIdx.x;
	if (column < totalColumns)
	{
		int i;
		int offset;
		int prevOffset;
		for (i = 1; i < totalRows; i++)
		{
			prevOffset = (i - 1) * totalColumns;
			offset = prevOffset + totalColumns;
			outputArray[column + offset] = outputArray[column + prevOffset]
					+ outputArray[column + offset];
		}
	}
}
__global__
void sumRow2(char *inputArray, unsigned int inputByteSize,
		unsigned int totalRows, unsigned int totalColumns,
		unsigned int *outputArray)
{
	int row = blockIdx.x * blockDim.x + threadIdx.x;
	if (row < totalRows)
	{
		char * rowPos = inputArray + row * totalColumns;
		char * rowLimit = rowPos + totalColumns;

		unsigned int * outPrev = outputArray + row * totalColumns;
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
		unsigned int totalRows, unsigned int totalColumns)
{
	int column = blockIdx.x * blockDim.x + threadIdx.x;
	if (column < totalColumns)
	{
		unsigned int * columnPos = outputArray + column;
		unsigned int * columnLimit = columnPos
				+ (totalColumns * (totalRows - 1));

		unsigned int * outPrev = columnPos;
		unsigned int * out = columnPos + totalColumns;

		while (out <= columnLimit)
		{
			*out += *outPrev;
			out += totalColumns;
			outPrev += totalColumns;
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
	for (y = 0; y < height; y++)
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
		sumRow1<<<(HEIGHT - 1) / THREADS_PER_BLOCK_1 + 1, THREADS_PER_BLOCK_1>>>( device_inputArray, inputByteSize, HEIGHT, WIDTH, device_outputArray);

		Blocks = (WIDTH - 1) / THREADS_PER_BLOCK_2 + 1;
		printf("Gonna use %u blocks and %u threads for sumRow\n", Blocks,
				THREADS_PER_BLOCK_2);
		sumColumn1<<<(WIDTH - 1) / THREADS_PER_BLOCK_2 + 1, THREADS_PER_BLOCK_2>>>( device_outputArray, outputByteSize, HEIGHT, WIDTH);





		cudaMemcpy(outputArray, device_outputArray, outputByteSize,
				cudaMemcpyDeviceToHost);

	}

	//printOutput(outputArray, WIDTH, HEIGHT);

	closeCUDASAT();

	free(inputArray);
	free(outputArray);
}

