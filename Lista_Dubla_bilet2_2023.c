#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include<stdio.h>
typedef struct Proiect Proiect;
typedef struct ListaDubla ListaDubla;
typedef struct Nod Nod;
struct Proiect
{
	unsigned int cod;
	char* proiect;
	char* beneficiar;
	unsigned char numar_executanti;
	float buget;
};
struct Nod
{
	Nod* next;
	Nod* prev;
	Proiect info;
};
struct ListaDubla
{
	Nod* prim;
	Nod* ultim;
};
typedef struct Heap Heap;
struct Heap
{
	Proiect* vector;
	int size;
};
void inserare_inceput(ListaDubla* lista,Proiect p)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = p;
	nou->next = lista->prim;
	nou->prev = NULL;
	if (lista->prim != NULL)
	{
		lista->prim->prev = nou;
		lista->prim = nou;
	}
	else
	{
		lista->prim = nou;
		lista->ultim = nou;
	}
}
void citire_fisier(const char* nume_fisier,ListaDubla* lista)
{
	FILE* f = fopen(nume_fisier, "r");
	if (f == NULL)
	{
		return;
	}
	
	char buffer[100];
	char delimitator[] = ",\n";
	while (fgets(buffer, 100, f) != NULL)
	{
		Proiect p;

		char* element = strtok(buffer, delimitator);

		p.cod = (unsigned int)atoi(element);

		element = strtok(NULL, delimitator);
		p.proiect = (char*)malloc(sizeof(char) * (strlen(element) + 1));
		strcpy(p.proiect, element);

		element = strtok(NULL, delimitator);
		p.beneficiar = (char*)malloc(sizeof(char) * (strlen(element) + 1));
		strcpy(p.beneficiar, element);

		element = strtok(NULL, delimitator);
		p.numar_executanti = (unsigned char)atoi(element);

		element = strtok(NULL, delimitator);
		p.buget = atof(element);

		inserare_inceput(lista, p);

	}
}
void afisareProiect(Proiect p)
{
	printf("\nid: %d, proiect: %s, beneficiar: %s, numar executanti: %d, buget:%.2f", p.cod, p.proiect, p.beneficiar, p.numar_executanti, p.buget);
}
void afisare_inceput_final(ListaDubla lista)
{
	if (lista.prim != NULL)
	{
		for (Nod* i = lista.prim; i != NULL; i = i->next)
		{
			afisareProiect(i->info);
		}
	}
}
void afisare_final_inceput(ListaDubla lista)
{
	if (lista.ultim != NULL)
	{
		for (Nod* i = lista.ultim; i != NULL; i = i->prev)
		{
			afisareProiect(i->info);
		}
	}
}
int nr_proiecte(ListaDubla lista, int nr_executanti)
{
	if (lista.prim != NULL)
	{
		int nr = 0;
		for (Nod* i = lista.prim; i != NULL; i = i->next)
		{
			if (i->info.numar_executanti > nr_executanti)
			{
				nr++;
			}
		}
		return nr;
	}
	else
		return 0;
}
void modificare_buget(ListaDubla lista, const char* beneficiar,float buget)
{
	if (lista.prim != NULL)
	{
		for (Nod* i = lista.prim; i != NULL; i = i->next)
		{
			if (strcmp(i->info.beneficiar,beneficiar)==0)
			{
				i->info.buget = buget;
			}
		}
	}
}
void sortare(ListaDubla lista)
{
	if (lista.prim != NULL)
	{
		for (Nod* i = lista.prim; i != NULL; i = i->next)
		{
			for (Nod* j = i->next; j != NULL; j = j->next)
			{
				if (i->info.buget > j->info.buget)
				{
					Proiect p = i->info;
					i->info = j->info;
					j->info = p;
				}
			}
		}
	}
}
Proiect deepCopy(Proiect pr)
{
	Proiect p;
	p.cod = pr.cod;

	p.proiect = (char*)malloc(sizeof(char) * (strlen(pr.proiect) + 1));
	strcpy(p.proiect, pr.proiect);

	p.beneficiar = (char*)malloc(sizeof(char) * (strlen(pr.beneficiar) + 1));
	strcpy(p.beneficiar, pr.beneficiar);

	p.numar_executanti = pr.numar_executanti;

	p.buget = pr.buget;
	
	return p;
}
void dezalocareLista(ListaDubla* lista)
{
	if (lista->prim != NULL)
	{
		Nod* curent = lista->prim;
		while (curent != NULL)
		{
			Nod* temp = curent->next;
			free(curent->info.beneficiar);
			free(curent->info.proiect);
			free(curent);
			curent = NULL;
			curent = temp;
		}
		lista->prim = NULL;
	}
}
void interschimb(Proiect* p1, Proiect* p2)
{
	Proiect p = *p1;
	*p1 = *p2;
	*p2 = p;
}
void inserare_in_heap(Heap* heap,Proiect p)
{
	heap->size++;
	int pozitie = heap->size - 1;
	heap->vector = (Proiect*)realloc(heap->vector, sizeof(Proiect) * heap->size);

	heap->vector[pozitie] = p;

	int pozitie_parinte = (pozitie - 1) / 2;

	while (heap->vector[pozitie].cod > heap->vector[pozitie_parinte].cod)
	{
		interschimb(&heap->vector[pozitie], &heap->vector[pozitie_parinte]);
		pozitie = pozitie_parinte;
		pozitie_parinte = (pozitie - 1) / 2;
	}
	
}
Proiect* introduce_heap(Heap* heap, ListaDubla lista, const char* beneficiar)
{
	if (lista.prim != NULL)
	{
		for (Nod* i = lista.prim; i != NULL; i = i->next)
		{
			if (strcmp(i->info.beneficiar, beneficiar) == 0)
			{
				Proiect p = deepCopy(i->info);
				inserare_in_heap(heap, p);
			}
		}
		return heap->vector;
	}
	
}

void maxim_f(Heap heap,int index)
{
	int maxim = index;
	int stanga = 2 * index + 1;
	int dreapta = 2 * index + 2;

	if (stanga<heap.size && heap.vector[maxim].cod < heap.vector[stanga].cod)
	{
		maxim = stanga;
	}

	if (dreapta < heap.size && heap.vector[maxim].cod < heap.vector[dreapta].cod)
	{
		maxim = dreapta;
	}

	if (maxim != index)
	{
		interschimb(&heap.vector[maxim], &heap.vector[index]);
		maxim_f(heap, maxim);
	}
}

void stergere_max(Heap* heap)//pot sa returnez proiectul inainte sa il sterg
{
	heap->vector[0] = heap->vector[heap->size - 1];
	heap->size--;
	heap->vector = (Proiect*)realloc(heap->vector, sizeof(Proiect) * heap->size);
	int startIndex = heap->size / 2 - 1;
	for (int i = startIndex; i < heap->size; i++)
	{
		maxim_f(*heap, i);
	}
}

void main()
{
	ListaDubla lista;
	lista.prim = NULL;
	lista.ultim = NULL;
	citire_fisier("Proiect2.txt", &lista);
	afisare_final_inceput(lista);

	printf("\nNumar de proiecte cu un numar de executanti: %d", nr_proiecte(lista, 10));
	printf("\n\nModificare\n");
	modificare_buget(lista, "Primaria Bucuresti", 1234);
	afisare_inceput_final(lista);
	printf("\n\nModificare\n");
	afisare_final_inceput(lista);

	printf("\n\nSortare\n");
	sortare(lista);
	afisare_inceput_final(lista);
	printf("\n\nSortare\n");
	afisare_final_inceput(lista);

	printf("\n\nHeap\n");
	Heap heap;
	heap.size = 0;
	heap.vector = (Proiect*)malloc(sizeof(Proiect) * heap.size);

	Proiect* vector_P = introduce_heap(&heap, lista, "Primaria Bucuresti");

	dezalocareLista(&lista);
	afisare_inceput_final(lista);

	for (int i = 0; i < heap.size; i++)
	{
		afisareProiect(vector_P[i]);
	}

	printf("\n\nDupa stergere\n");
	stergere_max(&heap);
	for (int i = 0; i < heap.size; i++)
	{
		afisareProiect(heap.vector[i]);
	}

	free(vector_P);
}

