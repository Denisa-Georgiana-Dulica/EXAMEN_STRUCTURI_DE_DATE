#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
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
int inaltime_arbore(Nod* radacina)
{
	if (radacina != NULL)
	{
		int inaltime_stg = inaltime_arbore(radacina->stanga);
		int inaltime_dr = inaltime_arbore(radacina->dreapta);
		int maxim;
		if (inaltime_dr > inaltime_stg)
		{
			maxim = inaltime_dr;
		}
		else
		{
			maxim = inaltime_stg;
		}

		return 1 + maxim;
	}
	else
		return 0;
}
//AVEM CLAR CEVA DEZECHILIBRU IN PARTEA STANGA (DACA FAC ROTIRE LA DREAPTA -> DEZ PARTEA STG)
Nod* rotire_dreapta(Nod* radacina)//returneaza noua radacina
{
	if (radacina != NULL) {
		//NE SALVAM NODUL CARE SE VA DUCE IN SUS (VA DEVENI RADACINA)
		Nod* aux = radacina->stanga; //Y
		radacina->stanga = aux->dreapta; //SALVEZ IN STG LUI X PE R
		aux->dreapta = radacina; //in dr lui Y este X
		return aux;//noua radacina Y
	}
	return radacina;// daca radacina mea e NULL, returnez NULL inapoi
}
Nod* rotire_stanga(Nod* radacina)//returneaza noua radacina
{
	if (radacina != NULL)
	{
		Nod* aux = radacina->dreapta;
		radacina->dreapta = aux->stanga;
		aux->stanga = radacina;
		return aux;
	}
	return radacina;// daca radacina mea e NULL, returnez NULL inapoi
}
int grad_de_echilibru(Nod* radacina)
{
	if (radacina != NULL)
	{
		int inaltime_stanga = inaltime_arbore(radacina->stanga);
		int inaltime_dreapta = inaltime_arbore(radacina->dreapta);
		return inaltime_stanga - inaltime_dreapta;
	}
	else
		return 0;
}
Nod* inserare_in_arbore(Nod* radacina, Rezervare r)
{
	if (radacina != NULL)
	{
		if (radacina->info.id > r.id)
		{
			radacina->stanga = inserare_in_arbore(radacina->stanga, r);
		}
		else
		{
			radacina->dreapta = inserare_in_arbore(radacina->dreapta, r);
		}
		//inainte sa returnez radacina trebuie sa vad daca aceasta mai este echilibrata
		
		if (grad_de_echilibru(radacina) == 2)//DEZECHILIBRU PE PARTEA STG
		{
			//trebuie sa vad daca Y=1 (O ROTIRE PE PARTEA DR) sau Y=-1 (DUBLA ROTIRE STG-DR)
			if (grad_de_echilibru(radacina->stanga)==1)
			{
				radacina=rotire_dreapta(radacina);//modific radacina
			}
			else
			{
				radacina->stanga = rotire_stanga(radacina->stanga);//rotire la stg in Y
				radacina = rotire_dreapta(radacina);//rotesc la dreapta pe X
			}
		}
		if (grad_de_echilibru(radacina) == -2)//DEZECHILIBRU PE DREAPTA
		{
			if (grad_de_echilibru(radacina->dreapta) == -1)
			{
				radacina = rotire_stanga(radacina);
			}
			else
			{
				radacina->dreapta = rotire_dreapta(radacina->dreapta);//rotire la stg in Y
				radacina = rotire_dreapta(radacina);//rotesc la dreapta pe X
			}
		}
		return radacina;//returnez noua radacina modificata daca am avut dezechilibru
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
void citireFisier(const char* nume_fisier, Nod** radacina)
{
	int nr_rezervari = 0;
	FILE* f = fopen(nume_fisier, "r");
	fscanf(f, "%d", &nr_rezervari);
	for (int i = 0; i < nr_rezervari; i++)
	{
		Rezervare r;
		r = citireRezervare(f);
		*radacina = inserare_in_arbore(*radacina, r);
	}
}
void afisare_rezervare(Rezervare r)
{
	printf("Rezervarea cu id-ul %d a fost facuta de %s pentru %d persoane.\n", r.id, r.numeClient, r.nrPersoane);
}
void afisare_arbore_preordine(Nod* arbore)
{
	if (arbore != NULL)
	{
		afisare_rezervare(arbore->info);
		afisare_arbore_preordine(arbore->stanga);
		afisare_arbore_preordine(arbore->dreapta);
	}
}
//sortare binara
void afisare_arbore_inordine(Nod* arbore)
{
	if (arbore != NULL)
	{
		afisare_arbore_inordine(arbore->stanga); //procesare stg
		afisare_rezervare(arbore->info); //procesare rad
		afisare_arbore_inordine(arbore->dreapta); //procesare dreapta
	}
}
void afisare_arbore_postordine(Nod* arbore)
{
	if (arbore != NULL)
	{
		afisare_arbore_inordine(arbore->stanga); //procesare stg
		afisare_arbore_inordine(arbore->dreapta); //procesare dreapta
		afisare_rezervare(arbore->info); //procesare rad

	}
}

void main()
{
	Nod* radacina = NULL;
	citireFisier("Rezervare.txt", &radacina);
	afisare_arbore_preordine(radacina);

	printf("\nInaltime %d", inaltime_arbore(radacina));
}