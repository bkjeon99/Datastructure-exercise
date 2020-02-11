#include <stdio.h>
#include <stdlib.h> // malloc, rand
#include <time.h> // time
#include <assert.h>

#define MAX_ELEM	20

typedef struct
{
	int *heapArr;
	int	last;
	int	capacity;
} HEAP;

//재귀함수 쓰기위해서 함수 미리 선언
static void _reheapUp(HEAP *heap, int index);
static void _reheapDown(HEAP *heap, int index);

HEAP *heapCreate(int capacity)
{
	HEAP * heap = (HEAP *)malloc(sizeof(HEAP));
	heap->heapArr = (int *)malloc(sizeof(int)*capacity);
	heap->last = 1;
	return heap;
};

void heapDestroy(HEAP *heap)
{
	free(heap->heapArr);
	free(heap);
}

int heapInsert(HEAP *heap, int data)
{
	heap->heapArr[heap->last] = data;
	_reheapUp(heap, heap->last);
	heap->last = heap->last + 1;
	return 1;
};

static void _reheapUp(HEAP *heap, int index)
{
	int temp;
	if (index != 1) {
		if (heap->heapArr[index / 2] < heap->heapArr[index])
		{
			temp = heap->heapArr[index];
			heap->heapArr[index] = heap->heapArr[index / 2];
			heap->heapArr[index / 2] = temp;
			_reheapUp(heap, index / 2);
		}
	}
}

int heapDelete(HEAP *heap, int* data)
{
	heap->heapArr[0] = heap->heapArr[1];
	*data = heap->heapArr[0];
	heap->heapArr[1] = heap->heapArr[heap->last-1];
	heap->last = heap->last - 1;
	_reheapDown(heap, 1);
	if (heap->last == 1) heap->last = heap->last -2;
	return 1;
};

static void _reheapDown(HEAP *heap, int index)
{
	int next_index;
	int temp;
	if (index <= heap->last / 2) {
		if (&(heap->heapArr[index * 2 + 1]) != NULL) {
			if (heap->heapArr[index * 2] >= heap->heapArr[index * 2 + 1])
			{
				next_index = index * 2;
			}
			else
			{
				next_index = index * 2 + 1;
			}
		}
		else next_index = index * 2;
	
		if (heap->heapArr[index] < heap->heapArr[next_index])
		{
			temp = heap->heapArr[next_index];
			heap->heapArr[next_index] = heap->heapArr[index];
			heap->heapArr[index] = temp;
			_reheapDown(heap, next_index);
		}
	}
}

void heapPrint(HEAP *heap)
{
	int i = 1;
	for (i = 1; i < heap->last; i++)
	{
		printf("%6d", heap->heapArr[i]);
	}
	printf("\n");
};


int main(void)
{
	HEAP *heap;
	int data;

	heap = heapCreate(MAX_ELEM);

	srand(time(NULL));

	for (int i = 1; i < MAX_ELEM; i++)
	{
		data = rand() % MAX_ELEM * 3 + 1; // 1 ~ MAX_ELEM*3 random number

		fprintf(stdout, "Inserting %d: ", data);

		// insert function call
		heapInsert( heap, data);

		heapPrint( heap);
	}

	while (heap->last >= 0)
	{
		// delete function call
		heapDelete( heap, &data);

		fprintf( stdout, "Deleting %d: ", data);

		heapPrint( heap);
	}

	heapDestroy( heap);

	return 0;
}