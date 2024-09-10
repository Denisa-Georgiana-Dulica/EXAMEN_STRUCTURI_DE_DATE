#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

typedef struct Banca Banca;
typedef struct Nod Nod;
struct Banca
{
	int cod;
	char* denumire;
	float profit;
	int nrClienti;
	char* strada;
	char exista;
};

struct Nod
{
	Nod* next;
	Banca info;
};

Banca initializare_Banca(int cod, const char* denumire, float profit, int nrClienti, const char* strada, char exista)
{
	Banca b;
	b.cod = cod;
	b.denumire = (char*)malloc(sizeof(char) * (strlen(denumire) + 1));
	strcpy(b.denumire, denumire);
	b.profit = profit;
	b.nrClienti = nrClienti;
	b.strada = (char*)malloc(sizeof(char) * (strlen(strada) + 1));
	strcpy(b.strada, strada);
	b.exista = exista;
	return b;
}
Banca deepCopy_initializare(Banca banca)
{
	Banca b;
	b.cod = banca.cod;
	b.denumire = (char*)malloc(sizeof(char) * (strlen(banca.denumire) + 1));
	strcpy(b.denumire, banca.denumire);
	b.profit = banca.profit;
	b.nrClienti = banca.nrClienti;
	b.strada = (char*)malloc(sizeof(char) * (strlen(banca.strada) + 1));
	strcpy(b.strada, banca.strada);
	b.exista = banca.exista;
	return b;
}
void inserare_inceput(Nod** lista, Banca b)
{
	Nod* nod_nou = (Nod*)malloc(sizeof(Nod));
	nod_nou->info = deepCopy_initializare(b);//nod_nou->info =b shallow copy
	nod_nou->next = (*lista);
	(*lista) = nod_nou; 
}
void citire_fisier(Nod** lista,const char* nume_fisier)
{
	FILE* f = fopen(nume_fisier, "r");
	if (f == NULL)
	{
		printf("Eroare la deschiderea fisierului");
		return ;
	}
	
	char buffer[100];
	char delimitator[] = ",\n";
	while(fgets(buffer,100,f)!=NULL)
	{
		Banca b;
		char* element = strtok(buffer, delimitator);

		b.cod = atoi(element);

		element = strtok(NULL, delimitator);
		b.denumire = (char*)malloc(sizeof(char) * (strlen(element) + 1));
		strcpy(b.denumire, element);

		element = strtok(NULL, delimitator);
		b.profit = atof(element);

		element = strtok(NULL, delimitator);
		b.nrClienti = atoi(element);

		element = strtok(NULL, delimitator);
		b.strada = (char*)malloc((strlen(element) + 1));
		strcpy(b.strada, element);

		element = strtok(NULL, delimitator);
		b.exista = element[0];

		inserare_inceput(&(*lista), b);
	}
	
}
void afisareBanca(Banca b)
{
	printf("\n%d Denumire: %s Profit: %.2f NrClienti: %d Strada: %s litera: %c", b.cod, b.denumire, b.profit, b.nrClienti, b.strada,b.exista);
}
void parcurgereLista(Nod* lista)
{
	if (lista != NULL)
	{
		while (lista != NULL)
		{
			afisareBanca(lista->info);
			lista = lista->next;
		}
	}
	else
		printf("Lista goala");


}
void inserareLaFinal(Nod** lista, Banca b)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = b;
	Nod* curent = (*lista);
	while (curent->next != NULL)
	{
		curent = curent->next;
	}
	curent->next = nou;
	nou->next = NULL;
}
void sortareDupaProfit(Nod* lista)
{
	Nod* i;
	Nod* j;
	for (i = lista; i != NULL; i=i->next)
	{
		for (j = i->next; j != NULL; j = j->next)
		{
			if (i->info.profit > j->info.profit)
			{
				Banca temp;
				temp = i->info;
				i->info = j->info;
				j->info = temp;
			}
		}
	}
}
void inserareLaMijloc(Nod** lista,Banca b)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = b;
	if ((*lista) == NULL || (*lista)->info.profit>b.profit)
	{
		nou->next = (*lista);
		(*lista) = nou;
	}
	else
	{
		Nod* curent = (*lista);
		while (curent->next != NULL && curent->next->info.profit < b.profit)
		{
			curent = curent->next;
		}
		nou->next = curent->next;
		curent->next = nou;
	}
}
void dezalocare(Nod** lista)
{
	if ((*lista) != NULL)
	{
		Nod* curent = (*lista);
		while ((*lista) != NULL)
		{
			free((*lista)->info.denumire);
			free((*lista)->info.strada);
			curent = (*lista)->next;
			free((*lista));
			(*lista) = NULL;
		}
	}
}
void stergere_inceput(Nod** lista)
{
	Nod* curent = (*lista)->next;
	free((*lista)->info.denumire);
	free((*lista)->info.strada);
	free((*lista));
	(*lista) = NULL;
	(*lista) = curent;
}
void sterge_final(Nod** lista)
{
	Nod* curent = (*lista);
	while (curent->next->next != NULL)
	{
		curent = curent->next;
	}
	free(curent->next->info.denumire);
	free(curent->next->info.strada);
	free(curent->next);
	curent->next = NULL;
}
void stergere_mijloc_nume(Nod** lista, const char* nume_sters)
{
	if (lista != NULL)
	{
		if (strcmp((*lista)->info.denumire, nume_sters) == 0)
		{
			stergere_inceput(&(*lista));
		}
		else
		{
			Nod* curent = (*lista);
			while (curent->next != NULL && (strcmp(curent->next->info.denumire, nume_sters) != 0))
			{
				curent = curent->next;
			}
			if (curent->next == NULL)//CURENT A AJUNS LA FINAL
			{
				printf("nu exista acest nume");
			}
			else//CURENT ESTE PENULTIMUL SAU IN MIJLOC
			{
				Nod* nod_sters = curent->next;
				curent->next = nod_sters->next;
				free(nod_sters->info.denumire);
				free(nod_sters->info.strada);
				free(nod_sters);
			}
				

		}
	}
}

void main()
{
	Nod* lista = NULL;
	printf("\n\nLISTA:\n");
	citire_fisier(&lista, "Banca.txt");
	parcurgereLista(lista);

	printf("\n\nLISTA INSERARE FINAL:\n");
	Banca b1 = initializare_Banca(4, "Banca1", 67.8, 300, "Strada1", 'n'); //char-> intre ' '
	inserareLaFinal(&lista, b1);
	parcurgereLista(lista);

	printf("\n\nSORTARE DUPA PROFIT:\n");
	sortareDupaProfit(lista);
	parcurgereLista(lista);

	printf("\n\nINSERARE LA MIJLOC:\n");
	Banca b2 = initializare_Banca(5, "Banca2", 100000000, 100, "Strada2", 'm');
	inserareLaMijloc(&lista, b2);
	parcurgereLista(lista);

	printf("\n\nSTERGE INCEPUTUL:\n");
	stergere_inceput(&lista);
	parcurgereLista(lista);

	printf("\n\nSTERGE FINAL:\n");
	sterge_final(&lista);
	parcurgereLista(lista);

	printf("\n\nSTERGE MIJLOC:\n");
	stergere_mijloc_nume(&lista,"Transilvania");
	parcurgereLista(lista);

	printf("\n\nDEZALOCARE:\n");
	dezalocare(&lista);
	parcurgereLista(lista);


}