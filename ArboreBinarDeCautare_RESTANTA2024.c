#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

struct Proiect
{
	unsigned int id;
	char* titlu;
	char* contractant;
	unsigned char durata;
	float buget;
	float executie_procent;
};
typedef struct Proiect Proiect;
typedef struct Nod Nod;
typedef struct Vector Vector;
struct Nod
{
	Nod* stanga;
	Nod* dreapta;
	Proiect info;
};
struct Vector
{
	Proiect* vector;
	int nrElem;
};
Nod* inserare_in_arbore(Nod* radacina, Proiect p)
{
	if (radacina != NULL)
	{
		if (radacina->info.id > p.id)
		{
			radacina->stanga = inserare_in_arbore(radacina->stanga, p);
		}
		else
		{
			radacina->dreapta = inserare_in_arbore(radacina->dreapta, p);
		}
		return radacina;
	}
	else
	{
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->dreapta = NULL;
		nou->stanga = NULL;
		nou->info = p;
		return nou;
	}
}
void citire_fisier(const char* nume_fisier, Nod** radacina)
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
		p.id = (unsigned int)atoi(element);

		element = strtok(NULL, delimitator);
		p.titlu = (char*)malloc(sizeof(char) * (strlen(element) + 1));
		strcpy(p.titlu, element);

		element = strtok(NULL, delimitator);
		p.contractant= (char*)malloc(sizeof(char) * (strlen(element) + 1));
		strcpy(p.contractant, element);

		element = strtok(NULL, delimitator);
		p.durata = (unsigned char)atoi(element);

		element = strtok(NULL, delimitator);
		p.buget = atof(element);
		element = strtok(NULL, delimitator);
		p.executie_procent = atof(element);

		(*radacina) = inserare_in_arbore((*radacina), p);
	}
	fclose(f);
}
void afisareProiect(Proiect p)
{
	printf("\nid: %d, titlu: %s, contractant: %s, durata: %d, buget: %.2f, procent executat: %.2f", p.id, p.titlu, p.contractant, p.durata, p.buget, p.executie_procent);
}
Proiect initializare(unsigned int id,const char* titlu,const char* contractant, unsigned char durata, float buget, float executie_procent)
{
	Proiect p;
	p.id = id;
	p.titlu = (char*)malloc(sizeof(char) * (strlen(titlu) + 1));
	strcpy(p.titlu, titlu);

	p.contractant = (char*)malloc(sizeof(char) * (strlen(contractant) + 1));
	strcpy(p.contractant, contractant);
	p.durata = durata;
	p.buget = buget;
	p.executie_procent = executie_procent;
	return p;
}
void afisare_inordine(Nod* radacina)
{
	if (radacina != NULL)
	{
		afisare_inordine(radacina->stanga);
		afisareProiect(radacina->info);
		afisare_inordine(radacina->dreapta);
	}
}
void afisare_preordine(Nod* radacina)
{
	if (radacina != NULL)
	{
		afisareProiect(radacina->info);
		afisare_preordine(radacina->stanga);
		afisare_preordine(radacina->dreapta);
	}
}
//nu caut dupa cheie
int numar_proiecte_dupa_contractant(Nod* radacina, const char* contractant_cautat)
{
	if (radacina != NULL)
	{
		int nrproiecte = 0;
		if (strcmp(radacina->info.contractant, contractant_cautat) == 0)
		{
			nrproiecte++;
		}
		else
		{
			nrproiecte += numar_proiecte_dupa_contractant(radacina->dreapta, contractant_cautat);
			nrproiecte += numar_proiecte_dupa_contractant(radacina->stanga, contractant_cautat);
		}
		return nrproiecte;
	}
	return 0;
}
//cautare dupa cheie
void modifica_statiu(Nod* radacina, int id,float stadiu)
{
	if (radacina!=NULL)
	{
		if (radacina->info.id == id)
		{
			radacina->info.executie_procent = stadiu;
		}
		else
		{
			if (radacina->info.id > id)
			{
				modifica_statiu(radacina->stanga, id, stadiu);
			}
			else
			{
				modifica_statiu(radacina->dreapta, id, stadiu);
			}
		}
	}
}
float bugetTotal(Nod* radacina, const char* contractant_cautat)
{
	if (radacina != NULL)
	{
		float buget=0;
		if (strcmp(radacina->info.contractant, contractant_cautat) == 0)
		{
			buget += radacina->info.buget;
		}
		else
		{
			buget += bugetTotal(radacina->dreapta, contractant_cautat);
			buget += bugetTotal(radacina->stanga, contractant_cautat);
		}
		return buget;
	}
	return 0;
}
void sterge_noduri_frunza(Nod** radacina)
{
	if ((*radacina) != NULL)
	{
		//daca radacina nu are niciun fie
		if ((*radacina)->dreapta == NULL && (*radacina)->stanga == NULL)
		{
			free((*radacina)->info.titlu);
			free((*radacina)->info.contractant);
			free((*radacina));
			(*radacina)=NULL;
		}
		else
		{
			sterge_noduri_frunza(&((*radacina)->dreapta));
			sterge_noduri_frunza(&((*radacina)->stanga));
		}
	}
	else
	return;
}
void dezalocareArbore(Nod** radacina)
{
	if ((*radacina) != NULL)
	{
		dezalocareArbore(&((*radacina)->dreapta));
		dezalocareArbore(&((*radacina)->stanga));
		free((*radacina)->info.titlu);
		free((*radacina)->info.contractant);
		free((*radacina));
		(*radacina) = NULL;
	}
}
void afiseaza_vector(Vector* v, Nod* radacina, float buget)
{
	if (radacina != NULL)
	{
		afiseaza_vector(v, radacina->stanga, buget);
		if (radacina->info.buget > buget)
		{
			(*v).nrElem++;
			(*v).vector = (Proiect*)realloc((*v).vector, sizeof(Proiect) * (*v).nrElem);
			(*v).vector[(*v).nrElem - 1] = radacina->info;
		}
		afiseaza_vector(v, radacina->dreapta, buget);
		
	}

}
void main()
{
	Nod* radacina = NULL;
	citire_fisier("Proiect.txt", &radacina);
	afisare_inordine(radacina);
	printf("\n\n");
	Proiect p = initializare(6, "Proiect 6", "Persoana 6", 100, 200, 0.90);
	radacina = inserare_in_arbore(radacina, p);
	afisare_preordine(radacina);

	printf("\nNumar proiecte: %d", numar_proiecte_dupa_contractant(radacina, "OpenAI"));
	modifica_statiu(radacina, 4, 99999);
	afisare_preordine(radacina);

	printf("\nBuget total: %.2f", bugetTotal(radacina, "OpenAI"));

	sterge_noduri_frunza(&radacina);
	afisare_preordine(radacina);

	afisare_preordine(radacina);
	Vector v;
	v.nrElem = 0;
	v.vector = (Proiect*)malloc(sizeof(Proiect) * v.nrElem);

	printf("\n\n");
	afiseaza_vector(&v, radacina, 200);
	for (int i = 0; i < v.nrElem; i++)
	{
		afisareProiect(v.vector[i]);
	}

	dezalocareArbore(&radacina);
	
}
