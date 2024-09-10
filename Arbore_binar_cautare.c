#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>
typedef struct CursaFeroviara CursaFeroviara;
struct CursaFeroviara
{
	unsigned int nr;
	char* data_cursa;
	unsigned char nr_vagoane;
	unsigned short int nr_bilete_cumparate[2];//clasa 1/2
	float pret[2]; //clasa 1/2


};
typedef struct Nod Nod;
struct Nod
{
	Nod* stanga;
	Nod* dreapta;
	CursaFeroviara info;
};
Nod* rotire_dreapta(Nod* radacina)
{
	if (radacina != NULL)
	{
		Nod* aux = radacina->stanga;
		radacina->stanga = aux->dreapta;
		aux->dreapta = radacina;
		return aux;
	}
	else
		return radacina;
}
Nod* rotire_stanga(Nod* radacina)
{
	if (radacina != NULL)
	{
		Nod* aux = radacina->dreapta;
		radacina->dreapta = aux->stanga;
		aux->dreapta = radacina;
		return aux;
	}
	else
		return radacina;
}
int grad_de_echilibruSTG_DR(Nod* radacina)
{
	if (radacina != NULL)
	{
		int inaltimeStg = inaltime(radacina->stanga);
		int inaltimeDr = inaltime(radacina->dreapta);
		return inaltimeStg - inaltimeDr;
	}
	return 0;
}
int inaltime(Nod* radacina)
{
	if (radacina != NULL)
	{
		int inaltimeStg = inaltime(radacina->stanga);
		int inaltimeDr = inaltime(radacina->dreapta);
		int maxim = 0;
		if (inaltimeDr > inaltimeStg)
		{
			maxim = inaltimeDr;
		}
		else
			maxim = inaltimeStg;
		return 1 + maxim;
	}
	else
		return 0;
}
Nod* inserare_in_arbore(Nod* radacina, CursaFeroviara c)
{
	if (radacina != NULL)
	{
		if (radacina->info.nr > c.nr)
		{
			radacina->stanga = inserare_in_arbore(radacina->stanga, c);
		}
		else
		{
			radacina->dreapta = inserare_in_arbore(radacina->dreapta, c);
		}
		if (grad_de_echilibruSTG_DR(radacina) == 2)
		{
			if (grad_de_echilibruSTG_DR(radacina->stanga) == 1)
			{
				radacina = rotire_dreapta(radacina);
			}
			else
			{

			}
		}
		return radacina;
	}
	else
	{
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->dreapta = NULL;
		nou->stanga = NULL;
		nou->info = c;
		return nou;
	}
}
void citire_fisier(const char* nume_fisier, Nod** radacina)
{
	FILE* f = fopen(nume_fisier, "r");
	if (f == NULL)
	{
		//exit(EXIT_FAILURE);
	}

	char buffer[100];
	char delimitator[] = ",\n";
	while (fgets(buffer, 100, f) != NULL)
	{
		CursaFeroviara cursa;
		char* element = strtok(buffer, delimitator);

		cursa.nr = atoi(element);

		element = strtok(NULL, delimitator);
		cursa.data_cursa = (char*)malloc(strlen(element) + 1);
		strcpy(cursa.data_cursa, element);

		element = strtok(NULL, delimitator);
		cursa.nr_vagoane = (unsigned char)atoi(element);

		element = strtok(NULL, delimitator);
		cursa.nr_bilete_cumparate[0] = (unsigned short int)atoi(element);

		element = strtok(NULL, delimitator);
		cursa.nr_bilete_cumparate[1] = (unsigned short int)atoi(element);

		element = strtok(NULL, delimitator);
		cursa.pret[0] = atof(element);

		element = strtok(NULL, delimitator);
		cursa.pret[1] = atof(element);
		/*	char* temp1 = strtok(element, "/");
			cursa.pret[0] = atof(temp1);
			temp1 = strtok(NULL, "/");
			cursa.pret[1] = atof(temp1);*/

		(*radacina) = inserare_in_arbore((*radacina), cursa);

	}
	fclose(f);
}
void afisare_cursa_feroviara(CursaFeroviara cursa) {
	printf("Numar cursa: %d, Data cursa: %s, Numar vagoane: %d\n", cursa.nr, cursa.data_cursa, cursa.nr_vagoane);
	printf("Nr. bilete clasa 1: %d, Nr. bilete clasa 2: %d\n", cursa.nr_bilete_cumparate[0], cursa.nr_bilete_cumparate[1]);
	printf("Pret bilet clasa 1: %.2f, Pret bilet clasa 2: %.2f\n", cursa.pret[0], cursa.pret[1]);
	printf("\n");
}
void afisarePreordina(Nod* radacina)
{
	if (radacina != NULL)
	{
		afisare_cursa_feroviara(radacina->info);
		afisarePreordina(radacina->stanga);
		afisarePreordina(radacina->dreapta);
	}
}
void main()
{
	Nod* radacina = NULL;
	citire_fisier("Cursa.txt", &radacina);
	afisarePreordina(radacina);
}

