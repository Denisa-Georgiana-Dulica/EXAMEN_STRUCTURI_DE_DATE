#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

struct Reteta
{
	unsigned int nrReteta;
	unsigned char nrMedicamente;
	char** listaMedicamente;
	unsigned char* cantitateMedicamente;
	float* preturiMedicamente;
	unsigned char* procente;
	char* numeMedic;
};
typedef struct Reteta Reteta;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;
struct Nod
{
	Nod* next;
	Nod* prev;
	Reteta info;
};
struct ListaDubla
{
	Nod* prim;
	Nod* ultim;
};

void inserare_la_inceput(ListaDubla* lista, Reteta r)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = r;
	nou->prev = NULL;
	nou->next = lista->prim;
	if (lista->prim== NULL)
	{
		lista->ultim = nou;
	}
	else
	{
		lista->prim->prev = nou;
	}
	lista->prim = nou;
}
void citire_fisier(ListaDubla* lista, const char* nume_fisier)
{
	FILE* f = fopen(nume_fisier, "r");
	if (f == NULL)
	{
		printf("Fisierul nu exista");
		return;
	}

	char buffer[100];
	char delimitator[] = ",;\n";
	while (fgets(buffer,100,f) != NULL)
	{
		char* element = strtok(buffer, delimitator);
		Reteta r;

		r.nrReteta = atoi(element);
		element = strtok(NULL, delimitator);
		r.nrMedicamente = (unsigned char)atoi(element);
		element = strtok(NULL, delimitator);
		r.listaMedicamente = NULL;
		int count = 0;
		for (int i = 0; i < r.nrMedicamente; i++)
		{
			r.listaMedicamente = (char**)realloc(r.listaMedicamente, sizeof(char*) * (count + 1));
			r.listaMedicamente[count] = (char*)malloc(sizeof(char) * (strlen(element) + 1));
			strcpy(r.listaMedicamente[count], element);
			count++;
			element = strtok(NULL, delimitator);
		}
		int count1 = 0;
		r.cantitateMedicamente = NULL;
		for (int i = 0; i < r.nrMedicamente; i++)
		{
			r.cantitateMedicamente = (unsigned char*)realloc(r.cantitateMedicamente, sizeof(unsigned char) * (count1 + 1));
			r.cantitateMedicamente[count1] = (unsigned char)atoi(element);
			count1++;
			element = strtok(NULL, delimitator);
		}
		int count2 = 0;
		r.preturiMedicamente = NULL;
		for (int i = 0; i < r.nrMedicamente; i++)
		{
			r.preturiMedicamente = (float*)realloc(r.preturiMedicamente, sizeof(float) * (count2 + 1));
			r.preturiMedicamente[count2] = atof(element);
			count2++;
			element = strtok(NULL, delimitator);
		}
		int count3 = 0;
		r.procente = NULL;
		for (int i = 0; i < r.nrMedicamente; i++)
		{
			r.procente = (unsigned char*)realloc(r.procente, sizeof(unsigned char) * (count3 + 1));
			r.procente[count3] = (unsigned char)atoi(element);
			count3++;
			element = strtok(NULL, delimitator);
		}
		r.numeMedic = (char*)malloc(sizeof(char) * (strlen(element) + 1));
		strcpy(r.numeMedic, element);

		inserare_la_inceput(&(*lista), r);//ATENTIE
	}
}
void afisareReteta(Reteta r)
{
	printf("Numar reteta: %d Numar medicamente: %d Medic: %s \n", r.nrReteta, r.nrMedicamente,r.numeMedic);
	for (int i = 0; i < r.nrMedicamente; i++)
	{
		printf("\nMedicament: %s Cantitate: %d Pret: %.2f Procent: %d", r.listaMedicamente[i], r.cantitateMedicamente[i], r.preturiMedicamente[i], r.procente[i]);
	}
	printf("\n\n");
}
Reteta initializare_reteta(unsigned int nrReteta, unsigned char nrMedicamente, char** listaMedicamente, unsigned char* cantitateMedicamente, float* preturiMedicamente, unsigned char* procente, char* numeMedic)
{
	Reteta r;
	r.nrReteta = nrReteta;
	r.nrMedicamente = nrMedicamente;
	r.listaMedicamente = NULL;
	r.cantitateMedicamente = NULL;
	r.preturiMedicamente = NULL;
	r.procente = NULL;
	int count = 0;
	for (int i = 0; i < r.nrMedicamente; i++)
	{
		r.listaMedicamente = (char**)realloc(r.listaMedicamente, sizeof(char*) * (count + 1));
		r.listaMedicamente[count] = (char*)malloc(sizeof(char) * (strlen(listaMedicamente[i]) + 1));
		strcpy(r.listaMedicamente[count], listaMedicamente[i]);

		r.cantitateMedicamente = (unsigned char*)realloc(r.cantitateMedicamente, sizeof(unsigned char) * (count + 1));
		r.cantitateMedicamente[count] = cantitateMedicamente[i];

		r.preturiMedicamente = (float*)realloc(r.preturiMedicamente, sizeof(float) * (count + 1));
		r.preturiMedicamente[count] = preturiMedicamente[i];

		r.procente = (unsigned char*)realloc(r.procente, sizeof(unsigned char) * (count + 1));
		r.procente[count] =procente[i];
		count++;
	}
	r.numeMedic = (char*)malloc(sizeof(char) * (strlen(numeMedic) + 1));
	strcpy(r.numeMedic, numeMedic);
	return r;

}
Reteta initializare_reteta_dupa_alta(Reteta reteta)
{
	Reteta r;
	r.nrReteta = reteta.nrReteta;
	r.nrMedicamente = reteta.nrMedicamente;
	r.listaMedicamente = NULL;
	r.cantitateMedicamente = NULL;
	r.preturiMedicamente = NULL;
	r.procente = NULL;
	int count = 0;
	for (int i = 0; i < r.nrMedicamente; i++)
	{
		r.listaMedicamente = (char**)realloc(r.listaMedicamente, sizeof(char*) * (count + 1));
		r.listaMedicamente[count] = (char*)malloc(sizeof(char) * (strlen(reteta.listaMedicamente[i]) + 1));
		strcpy(r.listaMedicamente[count], reteta.listaMedicamente[i]);

		r.cantitateMedicamente = (unsigned char*)realloc(r.cantitateMedicamente, sizeof(unsigned char) * (count + 1));
		r.cantitateMedicamente[count] = reteta.cantitateMedicamente[i];

		r.preturiMedicamente = (float*)realloc(r.preturiMedicamente, sizeof(float) * (count + 1));
		r.preturiMedicamente[count] = reteta.preturiMedicamente[i];

		r.procente = (unsigned char*)realloc(r.procente, sizeof(unsigned char) * (count + 1));
		r.procente[count] = reteta.procente[i];
		count++;
	}
	r.numeMedic = (char*)malloc(sizeof(char) * (strlen(reteta.numeMedic) + 1));
	strcpy(r.numeMedic, reteta.numeMedic);
	return r;

}
void afisareListaDublaInceputFinal (ListaDubla lista)
{
	if (lista.prim != NULL)
	{
		for (Nod* i = lista.prim; i != NULL; i = i->next)
		{
			afisareReteta(i->info);
		}
	}
	else
	{
		printf("Lista goala");
	}
}
void inserare_la_final(ListaDubla* lista, Reteta r)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = r;
	nou->next = NULL;
	nou->prev = lista->ultim;
	if (lista->ultim == NULL)
	{
		lista->prim = nou;
	}
	else
	{
		lista->ultim->next = nou;
	}
	lista->ultim = nou;
}
void afisareListaDublaFinalInceput(ListaDubla lista)
{
	if (lista.ultim != NULL)
	{
		for (Nod* i = lista.ultim; i != NULL; i = i->prev)
		{
			afisareReteta(i->info);
		}
	}
	else
	{
		printf("Lista goala");
	}
}
void stergere_inceput(ListaDubla* lista)
{
	if (lista->prim != NULL)
	{
		Nod* primul_nod = lista->prim;
		lista->prim = lista->prim->next;

		if (lista->prim == NULL)
		{
			lista->ultim = NULL;
		}
		else
		{
			lista->prim->prev = NULL;
		}
		for (int i = 0; i < primul_nod->info.nrMedicamente; i++)
		{
			free(primul_nod->info.listaMedicamente[i]);
		}
		free(primul_nod->info.listaMedicamente);
		free(primul_nod->info.cantitateMedicamente);
		free(primul_nod->info.procente);
		free(primul_nod->info.preturiMedicamente);
		free(primul_nod->info.numeMedic);
		free(primul_nod);

	}
}
void stergere_final(ListaDubla* lista)
{
	if (lista->ultim != NULL)
	{
		Nod* ultimul_nod = lista->ultim;
		lista->ultim = lista->ultim->prev;
		if (lista->ultim != NULL)
		{
			lista->ultim->next = NULL;
		}
		else
		{
			lista->prim = NULL;
		}
		for (int i = 0; i < ultimul_nod->info.nrMedicamente; i++)
		{
			free(ultimul_nod->info.listaMedicamente[i]);
		}
		free(ultimul_nod->info.listaMedicamente);
		free(ultimul_nod->info.cantitateMedicamente);
		free(ultimul_nod->info.procente);
		free(ultimul_nod->info.preturiMedicamente);
		free(ultimul_nod->info.numeMedic);
		free(ultimul_nod);
	}
}
void sortare(ListaDubla lista)
{
	if (lista.prim != NULL)
	{
		Reteta r;
		for (Nod* curent = lista.prim; curent != NULL; curent = curent->next)
		{
			for (Nod* curent1 = curent->next; curent1 != NULL; curent1 = curent1->next)
			{
				if (curent->info.nrReteta > curent1->info.nrReteta)
				{
					r = curent->info;
					curent->info = curent1->info;
					curent1->info = r;
				}
			}
		}
	}
}
void stergere_mijloc(ListaDubla* lista, const char* nume_medic)
{
	if (lista->prim != NULL)
	{
		if (lista->prim->next == NULL && strcmp(lista->prim->info.numeMedic, nume_medic) == 0)
		{
			for (int i = 0; i < lista->prim->info.nrMedicamente; i++)
			{
				free(lista->prim->info.listaMedicamente[i]);
			}
			free(lista->prim->info.listaMedicamente);
			free(lista->prim->info.cantitateMedicamente);
			free(lista->prim->info.preturiMedicamente);
			free(lista->prim->info.numeMedic);
			free(lista->prim);
			lista->prim = NULL;
			lista->ultim = NULL;
		}
		else
		{
			Nod* curent = lista->prim;
			while(curent!=NULL)
			{
				if (strcmp(curent->info.numeMedic, nume_medic) == 0)
				{
					if (curent->prev == NULL)
					{
						curent->next->prev = NULL;
						lista->prim = curent->next;
					}
					else
					{
						if (curent->next == NULL)
						{
							curent->prev->next = NULL;
							lista->ultim = curent->prev;
						}
						else
						{
							curent->next->prev = curent->prev;
							curent->prev->next = curent->next;
						}
					}
					Nod* temp = curent->next;
					for (int i = 0; i < curent->info.nrMedicamente; i++)
					{
						free(curent->info.listaMedicamente[i]);
					}
					free(curent->info.listaMedicamente);
					free(curent->info.cantitateMedicamente);
					free(curent->info.preturiMedicamente);
					free(curent->info.numeMedic);
					free(curent);
					curent = temp;
				}
				else
				{
					curent = curent->next;
				}
			}
		}
	}
}


//CERINTA 3
float incasari_medic(char* nume_medic,ListaDubla lista)
{
	//determinarea incasarilor
	float incasare = 0;
	int ok = 1;
	if (lista.prim != NULL)
	{
		
		for(Nod* curent = lista.prim;curent!=NULL;curent=curent->next)
		{
			if (strcmp(curent->info.numeMedic, nume_medic) == 0)
			{
				ok = 0;
				for (int i = 0; i < curent->info.nrMedicamente; i++)
				{
					incasare +=(float) (curent->info.cantitateMedicamente[i]) * (float)(curent->info.preturiMedicamente[i])*(float)curent->info.procente[i];
				}
			}
		}
		if (ok == 1)
		{
			printf("Nu exista medicul");
		}
		return incasare;
	}
}

//CERINTA 4
Reteta* vector(ListaDubla lista,const char* nume_medic, int* nr_retete)
{
	if (lista.prim != NULL)
	{
		(*nr_retete) = 0;
		
		Nod* curent = NULL;
		for (curent = lista.prim; curent != NULL; curent = curent->next)
		{
			if (strcmp(curent->info.numeMedic, nume_medic)==0)
			{
				(*nr_retete)++;
			}
			
		}
		Reteta* vector = (Reteta*)malloc(sizeof(Reteta) * (*nr_retete));
		int index = 0;
		for (curent = lista.prim; curent != NULL; curent = curent->next)
		{
			if (strcmp(curent->info.numeMedic, nume_medic)==0)
			{
				Reteta r = initializare_reteta_dupa_alta(curent->info);
				vector[index] = r;
				index++;
			}
		}
		return vector;
	}

}

//CERINTA 5
float compensatie(Reteta* vector, int nrRetete)
{
	float compensatieTotala = 0;
	for (int i = 0; i < nrRetete; i++)
	{
		for (int j = 0; j < vector[i].nrMedicamente; j++)
		{
			compensatieTotala += vector[i].preturiMedicamente[j] * ((float)vector[i].procente[j]/100);
		}
	}
	return compensatieTotala;
}

//CERINTA 6
int cantitate_medicament(ListaDubla lista, const char* denumire_medicament)
{
	if (lista.prim != NULL)
	{
		int cantitate = 0;
		for (Nod* curent = lista.prim; curent != NULL; curent = curent->next)
		{
			for (int i = 0; i < curent->info.nrMedicamente; i++)
			{
				if (strcmp(curent->info.listaMedicamente[i], denumire_medicament) == 0)
				{
					cantitate += curent->info.cantitateMedicamente[i];
				}
			}
		}
		return cantitate;
	}
}

//CERINTA 7
void dezalocare(ListaDubla* lista)
{
	if (lista->prim != NULL)
	{
		while (lista->prim != NULL)
		{
			Nod* curent = lista->prim->next;
			for (int i = 0; i < lista->prim->info.nrMedicamente; i++)
			{
				free(lista->prim->info.listaMedicamente[i]);	
			}
			free(lista->prim->info.listaMedicamente);
			free(lista->prim->info.cantitateMedicamente);
			free(lista->prim->info.procente);
			free(lista->prim->info.preturiMedicamente);
			free(lista->prim->info.numeMedic);
			free(lista->prim);
			lista->prim = NULL;
			lista->prim = curent;
		}
	}
}
//ATENTIE
void dezalocare_vector(Reteta** vector, int nrRetete)
{
	for (int i = 0; i < nrRetete; i++)
	{
		if ((*vector)[i].listaMedicamente != NULL) {
			for (int j = 0; j < (*vector)[i].nrMedicamente; j++)
			{
				free((*vector)[i].listaMedicamente[j]);
			}
			free((*vector)[i].listaMedicamente);
		}

		free((*vector)[i].cantitateMedicamente);
		free((*vector)[i].procente);
		free((*vector)[i].preturiMedicamente);
		free((*vector)[i].numeMedic);
	}

	free(*vector);
	*vector = NULL;
}
void main()
{
	ListaDubla listaDubla;
	listaDubla.prim = NULL;
	listaDubla.ultim = NULL;
	citire_fisier(&listaDubla, "Reteta.txt");
	afisareListaDublaInceputFinal(listaDubla);
	char* listaMedicamente[2] = {"Decasept", "Medicament2"};
	unsigned char cantitateMedicamente[2] = { '2','3' };
	float preturiMedicamente[2] = { 10.33,45.00 };
	unsigned char procente[2] = { 20.3,3 };

	printf("\n\nLISTA DUBLA INSERARE LA FINAL:\n");
	Reteta r1 = initializare_reteta(3, 2, listaMedicamente, cantitateMedicamente, preturiMedicamente, procente, "Nicolae");
	inserare_la_final(&listaDubla, r1);
	afisareListaDublaFinalInceput(listaDubla);

	printf("\n\nSORTARE:\n");
	sortare(listaDubla);
	afisareListaDublaInceputFinal(listaDubla);

	printf("\n\nSTERGERE MIJLOC:\n");
	stergere_mijloc(&listaDubla, "Popa Mihai");
	afisareListaDublaInceputFinal(listaDubla);

	float incasare = incasari_medic("Popa Mihai", listaDubla);
	printf("\n\nIncasari: %.2f", incasare);

	printf("\n\nVECTOR:\n");
	Reteta* vector_r = NULL;
	int nr_retete = 0;
	vector_r = vector(listaDubla, "Popa Mihai",&nr_retete);
	for (int i = 0; i < nr_retete; i++)
	{
		afisareReteta(vector_r[i]);
	}

	float compensatie_t = compensatie(vector_r, nr_retete);
	printf("\n\nCOMPENSATIE TOTALA: %.2f\n",compensatie_t);

	int cantitate_t = cantitate_medicament(listaDubla, "Decasept");
	printf("\n\nCANTITATE TOTALA: %d\n", cantitate_t);

	printf("\n\nSTERGE INCEPUT/FINAL:\n");
	//stergere_inceput(&listaDubla);
	stergere_final(&listaDubla);
	afisareListaDublaInceputFinal(listaDubla);

	printf("\n\nDEZALOCARE LISTA:\n");
	dezalocare(&listaDubla);
	afisareListaDublaInceputFinal(listaDubla);

	printf("\n\nDEZALOCARE VECTOR:\n");
	dezalocare_vector(&vector_r, nr_retete);
	

}
