#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
typedef struct Garantie Garantie;
typedef struct HashTable HashTable;
typedef struct ListaDubla ListaDubla;
struct Garantie
{
	char* denumire_produs;
	unsigned int numar_luni;
	unsigned char esteElectronica;
	char* service;
};
typedef struct Nod Nod;
typedef struct NodLD NodLD;
struct Nod
{
	Nod* next;
	Garantie info;
};
struct HashTable
{
	Garantie** vector;
	int nr_elemente;
};
struct ListaDubla
{
	NodLD* ultim;
	NodLD* prim;
};
struct NodLD
{
	NodLD* next;
	NodLD* prev;
	Garantie info;
};
int functieHash(int dimensiune, const char* service)
{
	if (dimensiune > 0)
	{
		int functie = 0;
		for (int i = 0; i < strlen(service); i++)
		{
			functie += service[i];
		}
		return functie % dimensiune;
	}
	else
		return -1;
}
HashTable initializareHT(int dimensiune)
{
	HashTable ht;
	ht.nr_elemente = dimensiune;
	ht.vector = (Garantie**)malloc(sizeof(Garantie*) * ht.nr_elemente);
	for (int i = 0; i < ht.nr_elemente; i++)
	{
		ht.vector[i] = NULL;
	}
	return ht;
}
void inserareFinalLista(Nod** lista, Garantie g)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = g;
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
void inserare_in_ht(HashTable ht, Garantie g)
{
	if (ht.nr_elemente > 0)
	{
		int pozitie = functieHash(ht.nr_elemente, g.service);
		if (ht.vector[pozitie] != NULL)
		{
			inserareFinalLista(&ht.vector[pozitie], g);
		}
		else
		{
			inserareFinalLista(&ht.vector[pozitie], g);
		}
	}
}
void citire_fisier(const char* nume_fisier,HashTable ht)
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
		Garantie g;
		char* element = strtok(buffer, delimitator);

		g.denumire_produs = (char*)malloc(sizeof(char) * (strlen(element) + 1));
		strcpy(g.denumire_produs, element);

		element = strtok(NULL, delimitator);
		g.numar_luni = (unsigned int)atoi(element);

		element = strtok(NULL, delimitator);
		g.esteElectronica = element[0];

		element = strtok(NULL, delimitator);
		g.service= (char*)malloc(sizeof(char) * (strlen(element) + 1));
		strcpy(g.service, element);

		inserare_in_ht(ht, g);
	}
}
void afisareGarantie(Garantie g)
{
	printf("\ndenumire: %s, numar luni: %d, este electronica: %c, service: %s", g.denumire_produs, g.numar_luni, g.esteElectronica, g.service);
}
void afisareHT(HashTable ht)
{
	if (ht.nr_elemente > 0)
	{
		for (int i = 0; i < ht.nr_elemente; i++)
		{
			if (ht.vector[i] != NULL)
			{
				printf("\nCluster %d:\n", i + 1);
				Nod* curent = ht.vector[i];
				while (curent != NULL)
				{
					afisareGarantie(curent->info);
					curent = curent->next;
				}
			}
		}
	}
	else
	{
		printf("gol");
	}
}
int nr_garantii(HashTable ht, const char* service)
{
	if (ht.nr_elemente > 0)
	{
		int pozitie = functieHash(ht.nr_elemente, service);
		int count = 0;
		if (ht.vector[pozitie] != NULL)
		{
			Nod* curent = ht.vector[pozitie];
			while (curent != NULL)
			{
				if (strcmp(curent->info.service, service) == 0)
				{
					count++;
				}
				curent = curent->next;
			}
			return count;
		}
	}
}
void extinde_garantie(HashTable ht, unsigned int nr_luni)
{
	if (ht.nr_elemente > 0)
	{
		for (int i = 0; i < ht.nr_elemente; i++)
		{
			if (ht.vector[i] != NULL)
			{
				Nod* curent = ht.vector[i];
				while (curent != NULL)
				{
					if (curent->info.esteElectronica == '1')
					{
						curent->info.numar_luni += nr_luni;
					}
					curent = curent->next;
				}
			}
		}
	}
}
void modifica_service(HashTable ht, const char* denumire_produs,const char* service)
{
	if (ht.nr_elemente > 0)
	{
		for (int i = 0; i < ht.nr_elemente; i++)
		{
			if (ht.vector[i] != NULL)
			{
				Nod* curent = ht.vector[i];
				while (curent != NULL)
				{
					if (strcmp(curent->info.denumire_produs,denumire_produs)==0)
					{
						curent->info.service=service;
					}
					curent = curent->next;
				}
			}
		}
	}
}
Garantie deepCopy(Garantie gr)
{
	Garantie g;
	g.denumire_produs = (char*)malloc(sizeof(char) * (strlen(gr.denumire_produs) + 1));
	strcpy(g.denumire_produs, gr.denumire_produs);

	g.numar_luni = gr.numar_luni;

	g.esteElectronica = gr.esteElectronica;

	g.service = (char*)malloc(sizeof(char) * (strlen(gr.service) + 1));
	strcpy(g.service, gr.service);

	return g;
}
void inserare_la_inceput(ListaDubla* lista,Garantie g)
{
	NodLD* nou = (NodLD*)malloc(sizeof(NodLD));
	nou->info = g;
	nou->next = lista->prim;
	if (lista->prim == NULL)
	{
		lista->prim = nou;
		lista->ultim = nou;
	}
	else
	{
		lista->prim->prev = nou;
		lista->prim = nou;
	}

}
void adauga_in_ld(HashTable ht,ListaDubla* lista,int nr_luni)
{
	if (ht.nr_elemente > 0)
	{
		for (int i = 0; i < ht.nr_elemente; i++)
		{
			if (ht.vector[i] != NULL)
			{
				Nod* curent = ht.vector[i];
				while (curent != NULL)
				{
					if (curent->info.numar_luni<nr_luni)
					{
						inserare_la_inceput(lista, curent->info);
					}
					curent = curent->next;
				}
			}
		}
	}
}
void afisare_ld(ListaDubla ld)
{
	if (ld.prim != NULL)
	{
		for (NodLD* i = ld.prim; i != NULL; i = i->next)
		{
			afisareGarantie(i->info);
		}
	}
}
void main()
{
	HashTable ht = initializareHT(3);
	citire_fisier("Garantie.txt", ht);
	afisareHT(ht);
	printf("\nNumar garantii: %d", nr_garantii(ht, "Service1"));
	extinde_garantie(ht, 100);
	afisareHT(ht);
	modifica_service(ht, "Produs3", "hdjbsws");
	afisareHT(ht);
	printf("\nNumar garantii: %d", nr_garantii(ht, "Service3"));
	printf("\nLista dubla:\n");
	ListaDubla ld;
	ld.prim = NULL;
	ld.ultim = NULL;
	adauga_in_ld(ht, &ld, 5);
	afisare_ld(ld);

}