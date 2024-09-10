#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>
//Definiți structura Candidat ce conține : cod candidat(int), nume candidat(char*),
//facultate absolvită(char*), medie absolvire facultate(float), salariu solicitat(int).

//Creați o structură tabelă de dispersie cu cel puțin 5 candidați ale căror date sunt
//preluate dintr - un fișier text. Crearea structurii se realizează prin apel repetat al unei
//funcții de inserare a unui candidat. Mecanismul de evitare a coliziunilor este chaining,
//iar cheia de inserare este facultate absolvită(2 p)

//2. Implementați funcția care determină cea mai mare medie de absolvire a candidaților
//pentru o anumită facultate specificată ca parametru. Funcția implementată se
//apelează în funcția main(), iar rezultatul apelului se afișează în consola de execuție a
//aplicației. (1 p)

//3. Implementați funcția care actualizează salariul solicitat de un candidat specificat ca
//parametru al funcției prin cod candidat.Noul salariu este specificat, de asemenea, ca
//parametru.Funcția implementată se apelează în funcția main(), iar rezultatul apelului
//se afișează în consola de execuție a aplicației. (1 p)

//CHAINING - DACA AM COLIZIUNI --> LISTE SIMPLE
struct Candidat
{
	int cod_candidat;
	char* nume_candidat;
	char* facultate_absolvita;
	float medie_absolvire;
	int salariu_solicitat;
};
//ESTE O LISTA DE LISTE SIMPLE = HASHTABLE
typedef struct Candidat Candidat;
typedef struct Nod Nod;
typedef struct HashTable HashTable;
struct Nod
{
	Nod* next;
	Candidat info;
};
struct HashTable
{
	Nod** vector_liste;
	int nr_elemente;
};
typedef struct Heap Heap;

struct Heap
{
	Candidat* vector;
	int size;
};

HashTable initializareHashTable(int dimensiune)
{
	HashTable ht;
	ht.nr_elemente = dimensiune;
	ht.vector_liste = (Nod**)malloc(sizeof(Nod*) * ht.nr_elemente);
	for (int i = 0; i < ht.nr_elemente; i++)
	{
		ht.vector_liste[i] = NULL;
	}
	return ht;
}
int functia_hash(int dimeniune, const char* facultate)
{
	int functie = 0;
	if (dimeniune > 0)
	{
		for (int i = 0; i < strlen(facultate); i++)
		{
			functie += facultate[i];
		}
		return functie % dimeniune;
	}
	return -1;
}
void inserareLaFinal(Nod** lista, Candidat c)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = c;
	nou->next = NULL;
	if ((*lista) != NULL)
	{
		Nod* curent = (*lista);
		while (curent->next != NULL)
		{
			curent = curent->next;
		}
		curent->next = nou;
	}
	else
	{
		(*lista) = nou;
	}

}
void inserare_in_hashtable(HashTable ht, Candidat c)
{
	if (ht.nr_elemente > 0)
	{
		int pozitie = functia_hash(ht.nr_elemente, c.facultate_absolvita);
		if (ht.vector_liste[pozitie] != NULL)
		{
			inserareLaFinal(&ht.vector_liste[pozitie], c);
		}
		else
		{
			inserareLaFinal(&ht.vector_liste[pozitie], c);
		}
	}
}
void citire_fisier(HashTable ht, const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	if (f == NULL)
	{
		printf("Fisierul nu exista");
		return;
	}

	char buffer[100];
	char delimitator[] = ",\n";
	while (fgets(buffer, 100, f) != NULL)
	{
		Candidat c;
		char* element = strtok(buffer, delimitator);
		c.cod_candidat = atoi(element);

		element = strtok(NULL, delimitator);
		c.nume_candidat = (char*)malloc(sizeof(char) * (strlen(element) + 1));
		strcpy(c.nume_candidat, element);

		element = strtok(NULL, delimitator);
		c.facultate_absolvita = (char*)malloc(sizeof(char) * (strlen(element) + 1));
		strcpy(c.facultate_absolvita, element);

		element = strtok(NULL, delimitator);
		c.medie_absolvire = atof(element);

		element = strtok(NULL, delimitator);
		c.salariu_solicitat = atoi(element);

		inserare_in_hashtable(ht, c);
	}
}
void afisareCandidat(Candidat c)
{
	printf("\nCod: %d Nume: %s Facultate: %s Medie %.2f Salariu solicitat: %d", c.cod_candidat, c.nume_candidat, c.facultate_absolvita, c.medie_absolvire, c.salariu_solicitat);
}
void afisareHashTable(HashTable ht)
{
	if (ht.nr_elemente > 0)
	{
		for (int i = 0; i < ht.nr_elemente; i++)
		{
			Nod* curent = ht.vector_liste[i];
			printf("\nCluster %d\n", i + 1);
			while (curent != NULL)
			{
				afisareCandidat(curent->info);
				curent = curent->next;
			}
		}
	}
}
float cea_mai_mare_medie(HashTable ht, const char* facultate)
{
	if (ht.nr_elemente > 0)
	{
		float max = 0;
		int pozitie = functia_hash(ht.nr_elemente, facultate);
		if (ht.vector_liste[pozitie] != NULL)
		{
			Nod* curent = ht.vector_liste[pozitie];
			max = curent->info.medie_absolvire;
			while (curent != NULL)
			{
				if (max < curent->info.medie_absolvire)
				{
					max = curent->info.medie_absolvire;
				}
				curent = curent->next;
			}
		}
		else
		{
			printf("Nu exista facultatea");
		}
		return max;
	}
}
void actualizeaza_salariu(HashTable ht, int cod_candidat, float salariu_nou)
{
	if (ht.nr_elemente > 0)
	{
		for (int i = 0; i < ht.nr_elemente; i++)
		{
			if (ht.vector_liste[i] != NULL)
			{
				Nod* curent = ht.vector_liste[i];
				while (curent != NULL)
				{
					if (curent->info.cod_candidat == cod_candidat)
					{
						curent->info.salariu_solicitat = salariu_nou;
					}
					curent = curent->next;
				}
			}
		}
	}
}
//ATENTIE
void stergere_dupa_cheie(HashTable ht, const char* facultate)
{
	if (ht.nr_elemente > 0)
	{
		int pozitie = functia_hash(ht.nr_elemente, facultate);
		Nod* curent = ht.vector_liste[pozitie];
		Nod* prev = NULL;
		while (curent != NULL)
		{
			if (strcmp(curent->info.facultate_absolvita, facultate) == 0)
			{
				if (prev == NULL)
				{
					ht.vector_liste[pozitie] = curent->next;
					free(curent->info.nume_candidat);
					free(curent->info.facultate_absolvita);
					free(curent);
					curent = NULL;

					prev = NULL;
					curent = ht.vector_liste[pozitie];
				}
				else
				{
					if (curent->next == NULL)
					{
						prev->next = NULL;
						free(curent->info.nume_candidat);
						free(curent->info.facultate_absolvita);
						free(curent);
						curent = NULL;
					}
					else
					{
						prev->next = curent->next;
						Nod* temp = curent->next;
						free(curent->info.nume_candidat);
						free(curent->info.facultate_absolvita);
						free(curent);
						curent = NULL;
						curent = temp;
					}
				}

			}
			else
			{
				prev = curent;
				curent = curent->next;
			}
		}
	}
}
void dezalocare_lista(Nod** lista)
{
	if ((*lista) != NULL)
	{
		while ((*lista) != NULL)
		{
			free((*lista)->info.nume_candidat);
			free((*lista)->info.facultate_absolvita);
			Nod* curent = (*lista)->next;
			free((*lista));
			(*lista) = NULL;
			(*lista) = curent;
		}
	}
}
void dezalocare(HashTable* ht)
{
	if ((*ht).nr_elemente > 0)
	{
		for (int i = 0; i < (*ht).nr_elemente;i++)
		{
			dezalocare_lista(&(ht->vector_liste[i]));
		}
	}
}
//SA TRANSFORM ASTA IN HEAP

Candidat deepCopy(Candidat ca)
{
	Candidat c;
	c.cod_candidat = ca.cod_candidat;

	c.nume_candidat = (char*)malloc(sizeof(char) * (strlen(ca.nume_candidat) + 1));
	strcpy(c.nume_candidat, ca.nume_candidat);

	c.facultate_absolvita = (char*)malloc(sizeof(char) * (strlen(ca.facultate_absolvita) + 1));
	strcpy(c.facultate_absolvita, ca.facultate_absolvita);

	c.medie_absolvire = ca.medie_absolvire;

	c.salariu_solicitat = ca.salariu_solicitat;
	return c;
}
void interschimbare(Candidat* c1, Candidat* c2)
{
	Candidat c = *c1;
	*c1 = *c2;
	*c2 = c;
}
void inserare_in_heap(Heap* heap, HashTable ht)
{
	if (ht.nr_elemente > 0)
	{
		for (int i = 0; i < ht.nr_elemente; i++)
		{
			if (ht.vector_liste[i] != NULL)
			{
				Nod* curent = ht.vector_liste[i];
				while (curent != NULL)
				{
					(*heap).size++;
					(*heap).vector = (Candidat*)realloc((*heap).vector,sizeof(Candidat) * (*heap).size);
					int pozitie = (*heap).size - 1;
					(*heap).vector[pozitie] = deepCopy(curent->info);

					int poz_parinte = (pozitie - 1) / 2;

					while (heap->vector[pozitie].medie_absolvire > heap->vector[poz_parinte].medie_absolvire)
					{
						interschimbare(&heap->vector[pozitie], &heap->vector[poz_parinte]);
						pozitie = poz_parinte;
						poz_parinte=(pozitie - 1) / 2;
					}
					curent = curent->next;

				}
			}
		}
	}
}
void afisare_heap(Heap heap)
{
	if (heap.size > 0)
	{
		for (int i = 0; i < heap.size; i++)
		{
			printf("\n %s - %.2f", heap.vector[i].nume_candidat, heap.vector[i].medie_absolvire);
		}
	}
}
void maxim(Heap heap,int index)
{
	int max = index;
	int stanga = 2 * index + 1;
	int dreapta = 2 * index + 2;

	if (stanga<heap.size && heap.vector[stanga].medie_absolvire>heap.vector[max].medie_absolvire)
	{
		max = stanga;
	}
	if (dreapta<heap.size && heap.vector[dreapta].medie_absolvire>heap.vector[max].medie_absolvire)
	{
		max = dreapta;
	}
	if (max != index)
	{
		interschimbare(&heap.vector[max], &heap.vector[index]);
		maxim(heap, max);
	}

}
Candidat stergere_max(Heap* heap)//EXTRAGERE MAXIM
{
	Candidat c = heap->vector[0];
	heap->vector[0] = heap->vector[heap->size - 1];
	heap->size--;

	heap->vector = (Candidat*)realloc(heap->vector,sizeof(Candidat) * heap->size);

	int startIndex = heap->size / 2 - 1;
	for (int i = startIndex; i >= 0; i--)
	{
		maxim((*heap), i);
	}
	return c;
}
void dezalocareHeap(Heap* heap)
{
	if (heap->size > 0)
	{
		for (int i = 0; i < heap->size; i++)
		{
			free(heap->vector[i].facultate_absolvita);
			free(heap->vector[i].nume_candidat);
		}
		free(heap->vector);
		heap->size = 0;
		heap->vector = NULL;
	}
}
void main()
{
	HashTable ht = initializareHashTable(4);
	citire_fisier(ht, "Candidat.txt");
	afisareHashTable(ht);
	
	printf("\n\nCea mai mare medie: %.2f", cea_mai_mare_medie(ht, "CSIE"));

	printf("\n\nActualizare salariu:\n");
	actualizeaza_salariu(ht, 2, 1234);
	afisareHashTable(ht);
	
	printf("\n\nStergere dupa cheie:\n");
	stergere_dupa_cheie(ht, "Sport");
	afisareHashTable(ht);

	Heap heap;
	heap.size = 0;
	heap.vector = (Candidat*)malloc(sizeof(Candidat)*heap.size);
	printf("\n\nAfisare heap:\n");
	inserare_in_heap(&heap, ht);
	afisare_heap(heap);

	printf("\n\nStergere max:\n");
	Candidat c = stergere_max(&heap);
	afisareCandidat(c);
	free(c.nume_candidat);
	afisare_heap(heap);

	dezalocareHeap(&heap);

	printf("\n\nDezalocare:\n");
	dezalocare(&ht);
	afisareHashTable(ht);



}