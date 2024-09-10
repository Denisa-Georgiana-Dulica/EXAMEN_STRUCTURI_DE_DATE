#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>
struct Task
{
	char* nume;
	int prioritate;
};
typedef struct Task Task;
typedef struct Heap Heap;
struct Heap
{
	Task* vector;
	int size;
};
void citire_fisier(const char* nume_fisier,Task** vector,int* nr_task)
{
	FILE* f = fopen(nume_fisier, "r");
	if (f == NULL)
	{
		return;
	}

	(*nr_task) = 0;
	(*vector) = (Task*)malloc(sizeof(Task) * (*nr_task));


	char buffer[100];
	char delimitator[] = ",\n";
	while (fgets(buffer, 100, f) != NULL)
	{
		char* element = strtok(buffer, delimitator);
		Task t;
		t.nume = (char*)malloc(sizeof(char) * (strlen(element) + 1));
		strcpy(t.nume, element);
		element = strtok(NULL, delimitator);
		t.prioritate = atoi(element);

		(*nr_task)++;
		(*vector) = (Task*)realloc((*vector), sizeof(Task) * (*nr_task));
		(*vector)[(*nr_task) - 1] = t;
	}

}
void interschimbare(Task* t1, Task* t2)
{
	Task aux = *t1;
	*t1 = *t2;
	*t2 = aux;
}
void inserare_in_heap(Heap* heap, Task task)
{
	heap->size++;
	heap->vector = (Task*)realloc(heap->vector, sizeof(Task) * heap->size);

	int pozitie = heap->size - 1;
	heap->vector[pozitie] = task;

	int pozitie_parinte = (pozitie - 1) / 2;

	while (heap->vector[pozitie].prioritate > heap->vector[pozitie_parinte].prioritate)
	{
		interschimbare(&heap->vector[pozitie], &heap->vector[pozitie_parinte]);
		pozitie = pozitie_parinte;
		pozitie_parinte = (pozitie - 1) / 2;
	}

}
void afiseaza_task(Task task)
{
	printf("\n%s - %d", task.nume, task.prioritate);
}
void afisare_heap(Heap heap)
{
	if (heap.size>0)
	{
		printf("\n");
		for (int i = 0; i < heap.size; i++)
		{
			printf("%d ", heap.vector[i].prioritate);
		}
	}
}
void maxim(Heap heap, int index)
{
	int max = index;
	int stanga = 2 * index + 1;
	int dreapta = 2 * index + 2;

	if (stanga<heap.size && heap.vector[stanga].prioritate>heap.vector[max].prioritate)
	{
		max = stanga;
	}
	if (dreapta<heap.size && heap.vector[dreapta].prioritate>heap.vector[max].prioritate)
	{
		max = dreapta;
	}

	if (max != index)
	{
		interschimbare(&heap.vector[max], &heap.vector[index]);
		maxim(heap, max);
	}
}
Task stergere(Heap* heap)
{
	Task task = heap->vector[0];
	interschimbare(&heap->vector[0], &heap->vector[heap->size - 1]);

	heap->size--;

	heap->vector = (Task*)realloc(heap->vector, sizeof(Task) * heap->size);

	int startIndex = heap->size / 2 - 1;
	for (int i = startIndex; i >= 0; i--)
	{
		maxim((*heap), i);
	}

	return task;
}
void main()
{
	//CITESC DATELE DIN FISIER
	int nr_taskuri;
	Task* vector = NULL;
	citire_fisier("Heap.txt", &vector, &nr_taskuri);
	for (int i = 0; i < nr_taskuri; i++)
	{
		afiseaza_task(vector[i]);
	}

	//INITIALIZEZ HEAP
	Heap heap;
	heap.size = 0;
	heap.vector = (Task*)malloc(sizeof(Task) * heap.size);

	for (int i = 0; i < nr_taskuri; i++)
	{
		inserare_in_heap(&heap, vector[i]);
	}
	afisare_heap(heap);

	Task t = stergere(&heap);
	afiseaza_task(t);
	free(t.nume);
	afisare_heap(heap);

	free(vector);
}
