#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

struct Consultatie
{
	char* data;
	char* nume_medic;
	char* specialitate;
	char* diagnostic;
	int pret_consultatie;
};
typedef struct NodP NodP;
typedef struct NodS NodS;
typedef struct Consultatie Consultatie;
struct NodP
{
	NodP* next;
	NodS* lista;
};
struct NodS
{
	Consultatie info;
	NodS* next;
};
NodS* cauta_dupa_cheie(NodP* lista, const char* specialitate)
{
	if (lista != NULL)
	{
		NodP* curent = lista;
		while (curent != NULL)
		{
			if (strcmp(curent->lista->info.specialitate, specialitate) == 0)
			{
				return curent->lista;
			}
			curent = curent->next;
		}
		return NULL;
	}
}
void inserare_in_LS_final (NodS** lista, Consultatie c)
{
	NodS* nou = (NodS*)malloc(sizeof(NodS));
	nou->next = NULL;
	nou->info = c;
	if ((*lista) != NULL)
	{
		NodS* curent = (*lista);
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
void inserare_in_LP_final(NodP** lista,Consultatie c)
{
	NodP* nou = (NodP*)malloc(sizeof(NodP));
	nou->lista = NULL;
	nou->next = NULL;
	if ((*lista) == NULL)
	{
		(*lista) = nou;
		inserare_in_LS_final(&(*lista)->lista, c);
	}
	else
	{
		NodS* gasita = cauta_dupa_cheie(*lista, c.specialitate);
		if (gasita == NULL)
		{
			NodP* curent = (*lista);
			while (curent->next != NULL)
			{
				curent = curent->next;
			}
			curent->next = nou;
			inserare_in_LS_final(&nou->lista, c);
		}
		else
		{
			inserare_in_LS_final(&gasita, c);
		}
	}
}
void citire_fisier(const char* nume_fisier,NodP** lista)
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
		Consultatie cons;
		char* element = strtok(buffer, delimitator);

		cons.data = (char*)malloc(sizeof(char) * (strlen(element) + 1));
		strcpy(cons.data, element);

		element = strtok(NULL, delimitator);
		cons.nume_medic = (char*)malloc(sizeof(char) * (strlen(element) + 1));
		strcpy(cons.nume_medic, element);

		element = strtok(NULL, delimitator);
		cons.specialitate = (char*)malloc(sizeof(char) * (strlen(element) + 1));
		strcpy(cons.specialitate, element);

		element = strtok(NULL, delimitator);
		cons.diagnostic = (char*)malloc(sizeof(char) * (strlen(element) + 1));
		strcpy(cons.diagnostic, element);

		element = strtok(NULL, delimitator);
		cons.pret_consultatie = atoi(element);

		inserare_in_LP_final(&(*lista), cons);
		
	}
	fclose(f);
}
int valoare_totala(NodP* lista,const char* specialitate)
{
	if (lista != NULL)
	{
		int valoare = 0;
		NodS* lista_sp = cauta_dupa_cheie(lista, specialitate);
		while (lista_sp != NULL)
		{
			valoare += lista_sp->info.pret_consultatie;
			lista_sp = lista_sp->next;
		}
		return valoare;
	}
}
void afisare_consultatie(Consultatie consultatie) {
	printf("Data: %s\n", consultatie.data);
	printf("Nume medic: %s\n", consultatie.nume_medic);
	printf("Specialitate: %s\n", consultatie.specialitate);
	printf("Diagnostic: %s\n", consultatie.diagnostic);
	printf("Pret consultatie: %d\n", consultatie.pret_consultatie);
}
void afisare_ls(NodS* lista)
{
	if (lista != NULL)
	{
		NodS* curent = lista;
		while (curent != NULL)
		{
			afisare_consultatie(curent->info);
			printf("\n");
			curent = curent->next;
		}
	}
}
void afisare_lp(NodP* lista)
{
	if (lista != NULL)
	{
		NodP* curent = lista;
		int i = 0;
		while (curent != NULL)
		{
			printf("\nLista %d\n",i++);
			afisare_ls(curent->lista);
			curent = curent->next;
		}
	}
}
void modifica_pret(NodP* lista, const char* data, int pret)
{
	if (lista != NULL)
	{
		NodP* lista_sp = lista;
		while (lista_sp != NULL)
		{
			NodS* curent = lista_sp->lista;
			while (curent != NULL)
			{
				if (strcmp(curent->info.data, data) == 0)
				{
					curent->info.pret_consultatie = pret;
				}
				curent = curent->next;
			}
			lista_sp = lista_sp->next;
		}
		
	}
}
void sterge_consultatie(NodP** lista, const char* diagnostic)
{
	if ((*lista) != NULL)
	{
		NodP* lista_sp = (*lista);
		while (lista_sp != NULL)
		{
			NodS* curent = lista_sp->lista;
			NodS* prev = NULL;
			while (curent != NULL)
			{
				if (strcmp(curent->info.diagnostic, diagnostic) == 0)
				{
					if (prev == NULL)
					{
						if (curent->next == NULL)
						{
							lista_sp->lista = NULL;
						}
						else
							lista_sp->lista = curent->next;
					}
					else
					{
						if (curent->next = NULL)
						{
							prev->next = NULL;
						}
						else
						{
							prev->next = curent->next;
						}
					}
					NodS* temp = curent->next;
					free(curent->info.data);
					free(curent->info.diagnostic);
					free(curent->info.nume_medic);
					free(curent->info.specialitate);
					free(curent);
					curent = NULL;
					curent = temp;

				}
				else
				{
					prev = curent;
					curent = curent->next;
				}
			}
			lista_sp = lista_sp->next;
		}

	}
}
void main()
{
	NodP* lista = NULL;
	citire_fisier("Consultatie.txt", &lista);
	afisare_lp(lista);
	printf("\nValoare totala: %d", valoare_totala(lista, "Chirurgie"));
	modifica_pret(lista, "2024-06-13", 1234);
	afisare_lp(lista);
	printf("\nSTERGERE\n");
	sterge_consultatie(&lista, "Entorsa");
	afisare_lp(lista);
}