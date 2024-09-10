#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
//arbore binar de cautare dupa id
typedef struct Rezervare Rezervare;
struct Rezervare
{
	int id;
	char* numeClient;
	int nrPersoane;
};

typedef struct Nod Nod;
struct Nod
{
	Nod* stanga;
	Nod* dreapta;
	Rezervare info;
};

Rezervare citireRezervare(FILE* f)
{
	Rezervare r;
	fscanf(f, "%d", &r.id);
	char buffer[100];
	fscanf(f, "%s", &buffer);
	r.numeClient = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(r.numeClient, buffer);
	fscanf(f, "%d", &r.nrPersoane);
	return r;

}
Nod* inserare_in_arbore(Nod* radacina,Rezervare r)//cand insererz in arbore, obligatoriu noul nou inserat va fi nod frunza
{
	if (radacina != NULL)//conditie de oprire obligatoriu (am arbore?)
	{
		if (radacina->info.id > r.id)//daca valoarea e mai mica decat radacina, o sa inserez valoarea in subarborele stang
		{
			radacina->stanga = inserare_in_arbore(radacina->stanga,r);//recursivitate pentru nodul din stanga radacinii
		}
		else
		{
			radacina->dreapta = inserare_in_arbore(radacina->dreapta, r);
		}
		return radacina;//refacem toate legaturile prin care am trecut la acea inserare
	}
	else
	{
		//creez un nou nod
		Nod* nod = (Nod*)malloc(sizeof(Nod));
		nod->info = r;
		//nod frunza
		nod->dreapta = NULL;
		nod->stanga = NULL;

		return nod;
	}
}
void citireFisier(const char* nume_fisier, Nod** radacina)
{
	int nr_rezervari = 0;
	FILE* f = fopen(nume_fisier, "r");
	fscanf(f, "%d", &nr_rezervari);
	for (int i = 0; i < nr_rezervari; i++)
	{
		Rezervare r;
		r = citireRezervare(f);
		(*radacina)=inserare_in_arbore((*radacina), r);
	}
	fclose(f);
}
void afisare_rezervare(Rezervare r)
{
	printf("Rezervarea cu id-ul %d a fost facuta de %s pentru %d persoane.\n", r.id, r.numeClient, r.nrPersoane);
}
void afisare_inordine(Nod* radacina)//elemente afisare crescator
{
	if (radacina != NULL)
	{
		//divide et impera (afisez stg, rad, dr)
		afisare_inordine(radacina->stanga);
		afisare_rezervare(radacina->info);//radacina e un nod
		afisare_inordine(radacina->dreapta);
	}
}
void afisare_preordine(Nod* radacina)
{
	if (radacina != NULL)
	{
		afisare_rezervare(radacina->info);
		afisare_preordine(radacina->stanga);
		afisare_preordine(radacina->dreapta);
	}
}
void afisare_postordine(Nod* radacina)
{
	if (radacina != NULL)
	{
		
		afisare_postordine(radacina->stanga);
		afisare_postordine(radacina->dreapta);
		afisare_rezervare(radacina->info);
	}
}
Rezervare initializareRezervare(Rezervare rez)
{
	Rezervare r;
	r.id = rez.id;
	r.nrPersoane = rez.nrPersoane;
	r.numeClient = (char*)malloc(sizeof(char) * (strlen(rez.numeClient) + 1));
	strcpy(r.numeClient, rez.numeClient);
	return r;
}
Rezervare initializareRezervareDefault()
{
	Rezervare r;
	r.id = -1;
	r.nrPersoane = -1;
	r.numeClient = "";
	return r;
}
Rezervare cautare_dupa_id(Nod* radacina, int id)//nu extrage, doar afiseaza
{
	if (radacina != NULL)
	{
		if (radacina->info.id == id)
		{
			//deep copy
			Rezervare r = initializareRezervare(radacina->info);
			return r;
		}
		else
		{
			if (radacina->info.id>id)
			{
				return cautare_dupa_id(radacina->stanga, id);// pentru ca trebuie sa returnez
			}
			else
			{
				return cautare_dupa_id(radacina->dreapta, id);
			}
		}
	}
	else
	{
		return initializareRezervareDefault();
	}
}
int calculTotalPersoane(Nod* radacina)
{
	if (radacina != NULL)
	{
		int rNrPersoane = radacina->info.nrPersoane;
		int stgSumaNrPers = calculTotalPersoane(radacina->stanga);
		int drSumaNrPers = calculTotalPersoane(radacina->dreapta);
		int suma = rNrPersoane + stgSumaNrPers + drSumaNrPers;
		return suma;
	}
	else
		return 0;
}
int inaltimeArbore(Nod* radacina)
{
	if (radacina != NULL)
	{
		int inaltimeStg = inaltimeArbore(radacina->stanga);
		int inaltimeDr = inaltimeArbore(radacina->dreapta);
		if (inaltimeDr > inaltimeStg)
		{
			return 1 + inaltimeDr;
		}
		else
			return 1 + inaltimeStg;
	}
	else
		return 0;
}
void main()
{
	Nod* radacina = NULL;
	citireFisier("Rezervare.txt", &radacina);
	afisare_preordine(radacina);

	Rezervare r = cautare_dupa_id(radacina,3);
	afisare_rezervare(r);
	free(r.numeClient);//pentru ca am facut deep copy
	
	printf("NR TOTAL PERS: %d", calculTotalPersoane(radacina));
	printf("\n%d",inaltimeArbore(radacina));
}