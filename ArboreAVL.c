#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

struct Fotografie
{
	unsigned int idFotografie;
	char* oras;
	char* data;
	float rezolutie;
}; 
typedef struct Fotografie  Fotografie;
typedef struct Nod Nod;
struct Nod
{
	Nod* stanga;
	Nod* dreapta;
	Fotografie info;
};
typedef struct Vector Vector;
struct Vector
{
	Fotografie* vector;
	int nr_elemente;
};
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
		return 1+maxim;
	}
	else
		return 0;
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
Nod* inserare_arbore(Nod* radacina, Fotografie f)
{
	if (radacina != NULL)
	{
		if (radacina->info.idFotografie > f.idFotografie)
		{
			radacina->stanga = inserare_arbore(radacina->stanga, f);
		}
		else
		{
			radacina->dreapta= inserare_arbore(radacina->dreapta, f);
		}

		if (grad_de_echilibruSTG_DR(radacina) == 2)//dez pe stanga
		{
			if (grad_de_echilibruSTG_DR(radacina->stanga)==1)
			{
				radacina = rotire_dreapta(radacina);
			}
			else
			{
				//grad radacina->stanga==-1
				radacina->stanga = rotire_stanga(radacina->stanga);
				radacina = rotire_dreapta(radacina);
			}
		}
		if (grad_de_echilibruSTG_DR(radacina) == -2)//dez pe stanga
		{
			if (grad_de_echilibruSTG_DR(radacina->stanga) == -1)
			{
				radacina = rotire_stanga(radacina);
			}
			else
			{
				//grad radacina->stanga==1
				radacina->dreapta = rotire_dreapta(radacina->dreapta);
				radacina = rotire_stanga(radacina);
			}
		}
		return radacina;
	}
	else
	{
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = f;
		nou->dreapta = NULL;
		nou->stanga = NULL;
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
		Fotografie foto;
		char* element = strtok(buffer, delimitator);

		foto.idFotografie = (unsigned int)atoi(element);

		element = strtok(NULL, delimitator);
		foto.oras = (char*)malloc(sizeof(char) * (strlen(element) + 1));
		strcpy(foto.oras, element);

		element = strtok(NULL, delimitator);
		foto.data = (char*)malloc(sizeof(char) * (strlen(element) + 1));
		strcpy(foto.data, element);

		element = strtok(NULL, delimitator);
		foto.rezolutie = atof(element);

		*radacina = inserare_arbore((*radacina), foto);
	}
	fclose(f);
}
void afisareFotografie(Fotografie f)
{
	printf("\nid: %d , oras: %s, data: %s, rezolutie: %.2f", f.idFotografie, f.oras, f.data, f.rezolutie);
}
void parcurgere_preordine(Nod* radacina)
{
	if (radacina != NULL)
	{
		afisareFotografie(radacina->info);
		parcurgere_preordine(radacina->stanga);
		parcurgere_preordine(radacina->dreapta);
	}
}
void parcurgere_inordine(Nod* radacina)
{
	if (radacina != NULL)
	{
		parcurgere_preordine(radacina->stanga);
		afisareFotografie(radacina->info);
		parcurgere_preordine(radacina->dreapta);
	}
}
int nr_fotografii_acelasi_oras(Nod* radacina, const char* oras)
{
	if (radacina != NULL)
	{
		int nr = 0;
		if (strcmp(radacina->info.oras, oras) == 0) {
			nr++;
		}
		
		nr += nr_fotografii_acelasi_oras(radacina->stanga, oras);
		nr += nr_fotografii_acelasi_oras(radacina->dreapta, oras);
		
		return nr;
	}
	else
		return 0;
}
void modifica_data(Nod* radacina, const char* data,unsigned int id)
{
	if (radacina != NULL)
	{
		if (radacina->info.idFotografie == id)
		{
			free(radacina->info.data);
			radacina->info.data = (char*)malloc(sizeof(char) * (strlen(data) + 1));
			strcpy(radacina->info.data, data);
		}
		else
		{
			if (radacina->info.idFotografie > id)
			{
				modifica_data(radacina->stanga, data, id);
			}
			else
			{
				modifica_data(radacina->dreapta, data, id);
			}
		}
	}
}
int nr_noduri_frunza(Nod* radacina)
{
	if (radacina != NULL)
	{
		int nr = 0;
		if (radacina->dreapta == NULL && radacina->stanga == NULL)
		{
			nr++;
		}
		nr += nr_noduri_frunza(radacina->dreapta);
		nr += nr_noduri_frunza(radacina->stanga);
		return nr;
	}
	else
		return 0;
}
void sterge_noduri_frunza(Nod** radacina)
{
	if ((*radacina) != NULL)
	{
		if ((*radacina)->dreapta == NULL && (*radacina)->stanga == NULL)
		{
			free((*radacina)->info.oras);
			free((*radacina)->info.data);
			free((*radacina));
			(*radacina) = NULL;
		}
		else
		{
			sterge_noduri_frunza(&((*radacina)->dreapta));
			sterge_noduri_frunza(&((*radacina)->stanga));
		}
		
	}

}
void sterge_radacina(Nod** radacina)
{
	if ((*radacina) != NULL)
	{
		Nod* stanga = (*radacina)->stanga;
		Nod* dreapta = (*radacina)->dreapta;
		dreapta->stanga->stanga= stanga;
		free((*radacina)->info.oras);
		free((*radacina)->info.data);
		free((*radacina));
		(*radacina) = NULL;
		(*radacina) = dreapta;
	}
}
void dezalocare_arbore(Nod** radacina)
{
	if ((*radacina) != NULL)
	{
		dezalocare_arbore(&((*radacina)->dreapta));
		dezalocare_arbore(&((*radacina)->stanga));
		free((*radacina)->info.data);
		free((*radacina)->info.oras);
		free((*radacina));
		(*radacina) = NULL;
	}
}
Fotografie deepCopy(Fotografie f)
{
	Fotografie foto;

	foto.idFotografie = f.idFotografie;

	
	foto.oras = (char*)malloc(sizeof(char) * (strlen(f.oras) + 1));
	strcpy(foto.oras, f.oras);

	
	foto.data = (char*)malloc(sizeof(char) * (strlen(f.data) + 1));
	strcpy(foto.data, f.data);

	foto.rezolutie = f.rezolutie;
	return foto;
}
void inserare_vector(Vector* vector, Nod* radacina, float rezolutie)
{
	if (radacina != NULL)
	{
		if (radacina->info.rezolutie == rezolutie)
		{
			(*vector).nr_elemente++;
			(*vector).vector = (Fotografie*)realloc((*vector).vector, sizeof(Fotografie) * (*vector).nr_elemente);
			(*vector).vector[(*vector).nr_elemente - 1] = radacina->info;
		}
		inserare_vector(&(*vector), radacina->dreapta, rezolutie);
		inserare_vector(&(*vector), radacina->stanga, rezolutie);
	}
}
void main()
{
	Nod* radacina = NULL;
	citire_fisier("Fotografie.txt", &radacina);
	parcurgere_preordine(radacina);
	printf("\n inaltime arbore: %d", inaltime(radacina));
	printf("\nNumar fotografii : %d", nr_fotografii_acelasi_oras(radacina, "Rome"));
	printf("\nModifica:\n");
	modifica_data(radacina, "2022-02-02", 3);
	parcurgere_inordine(radacina);
	printf("\nNr noduri frunza: %d", nr_noduri_frunza(radacina));

	Vector vector;
	vector.nr_elemente = 0;
	vector.vector = (Fotografie*)malloc(sizeof(Fotografie) * vector.nr_elemente);
	printf("\nVector:\n");
	inserare_vector(&vector, radacina, 22.1);
	for (int i = 0; i < vector.nr_elemente; i++)
	{
		afisareFotografie(vector.vector[i]);
	}

	/*printf("\nNoduri frunza sterse\n");
	sterge_noduri_frunza(&radacina);
	parcurgere_inordine(radacina);*/
	printf("\nSterge radacina\n");
	sterge_radacina(&radacina);
	parcurgere_preordine(radacina);

	dezalocare_arbore(&radacina);
	parcurgere_inordine(radacina);
}