#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

//ASEMANATOR SUBIECT 14
typedef struct Programare Programare;
struct Programare
{
	char* dataProgramare;
	char* numeClient;
	float durataOre;
	char* serviciiEfectuate;
	int costProgramare;
};
typedef struct NodLSecundara NodLSecundara;
struct NodLSecundara
{
	NodLSecundara* next;
	Programare info;
};
typedef struct NodLPrincipala NodLPrincipala;
struct NodLPrincipala
{
	NodLSecundara* lista;
	NodLPrincipala* nextLP;
};
typedef struct Stiva Stiva;
struct Stiva
{
	Programare info;
	Stiva* next;
};
NodLSecundara* cauta_in_LP(NodLPrincipala* listaP, const char* data_programare)
{
	if (listaP != NULL)
	{
		NodLPrincipala* curent = listaP;
		while (curent != NULL)
		{
			if (curent->lista != NULL)
			{
				if (strcmp(curent->lista->info.dataProgramare,data_programare)==0)
				{
					return curent->lista;
				}
				
			}
			curent = curent->nextLP;
		}
		return NULL;
	}
}

NodLPrincipala* creareNodpp()
{
	NodLPrincipala* nou = (NodLPrincipala*)malloc(sizeof(NodLPrincipala));
	nou->lista = NULL;
	nou->nextLP = NULL;
	return nou;
}
void inserareLS(NodLSecundara** lista, Programare p)
{
	NodLSecundara* nou = (NodLSecundara*)malloc(sizeof(NodLSecundara));
	nou->next = NULL;
	nou->info = p;
	if ((*lista) == NULL)
	{
		(*lista) = nou;
	}
	else
	{
		NodLSecundara* curent = (*lista);
		while (curent->next != NULL)
		{
			curent = curent->next;
		}
		curent->next = nou;
	}
}

void inserareLP(NodLPrincipala** lista, Programare p)
{
	if ((*lista) == NULL)
	{
		(*lista) = creareNodpp();
		inserareLS(&((*lista)->lista), p);
	}
	else
	{
		NodLSecundara* lista_gasita = cauta_in_LP((*lista), p.dataProgramare);
		if (lista_gasita == NULL)
		{
			NodLPrincipala* curent = (*lista);
			while (curent->nextLP != NULL)
			{
				curent = curent->nextLP;
			}
			curent->nextLP = creareNodpp();
			inserareLS(&curent->nextLP->lista, p);

		}
		else
		{
			inserareLS(&lista_gasita, p);
		}
	}
}

void citire_din_fisier(const char* nume_fisier,NodLPrincipala** lista)
{
	FILE* f = fopen(nume_fisier, "r");
	if (f == NULL)
	{
		printf("Fisierul nu exista");
	}
	char buffer[100];
	char delimitator[] = ",\n";
	while (fgets(buffer, 100, f) != NULL)
	{
		Programare p;
		char* element = strtok(buffer, delimitator);
		p.dataProgramare = (char*)malloc(sizeof(char) * (strlen(element) + 1));
		strcpy(p.dataProgramare, element);

		element = strtok(NULL, delimitator);
		p.numeClient = (char*)malloc(sizeof(char) * (strlen(element) + 1));
		strcpy(p.numeClient, element);

		element = strtok(NULL, delimitator);
		p.durataOre = atof(element);

		element = strtok(NULL, delimitator);
		p.serviciiEfectuate = (char*)malloc(sizeof(char) * (strlen(element) + 1));
		strcpy(p.serviciiEfectuate, element);

		element = strtok(NULL, delimitator);
		p.costProgramare = atoi(element);

		inserareLP(&(*lista), p);
	}
}
void afisareLS(NodLSecundara* lista)
{
	NodLSecundara * temp = lista;
	if (temp != NULL)
	{
		while (temp)
		{
			printf("Data: %s\nNume: %s\nDurata ore: %.2f\nServiciu: %s\nPret: %d RON\n\n", temp->info.dataProgramare, temp->info.numeClient, temp->info.durataOre, temp->info.serviciiEfectuate, temp->info.costProgramare);
			temp = temp->next;
		}
	}
}
void afisareLP(NodLPrincipala* lista)
{
	if (lista != NULL)
	{
		NodLPrincipala* lista_pp = lista;
		int count = 0;
		while (lista_pp != NULL)
		{
			printf("\nProgramare %d:\n", count++);
			afisareLS(lista_pp->lista);
			lista_pp = lista_pp->nextLP;
		}
	}
	else
		printf("Lista goala");

}
float valoare_totala_data(NodLPrincipala* lista, const char* data)
{
	if (lista != NULL)
	{
		float valoare = 0;
		NodLSecundara* gasit = cauta_in_LP(lista, data);
		if (gasit == NULL)
		{
			printf("data nu exista");
			return 0;
		}
		else
		{
			while (gasit != NULL)
			{
				valoare += gasit->info.costProgramare;
				gasit = gasit->next;
			}
			return valoare;
		}
	}
}

void modifica_cost(NodLPrincipala* lista, const char* nume, float nou_pret)
{
	if (lista != NULL)
	{
		int gasit = 0;
		NodLPrincipala* curent = lista;
		while (curent != NULL)
		{
			NodLSecundara* curent1 = curent->lista;
			while (curent1 != NULL)
			{
				if (strcmp(curent1->info.numeClient, nume) == 0)
				{
					gasit = 1;
					curent1->info.costProgramare = nou_pret;
				}
				curent1 = curent1->next;
			}
			curent = curent->nextLP;
		}
		if (gasit == 0)
		{
			printf("Nu exista numele");
		}
	}
}

void sterge_programari(NodLPrincipala** lista, int durata)
{
	if ((*lista) != NULL)
	{
		NodLPrincipala* curent = (*lista);
		while (curent != NULL)
		{
			NodLSecundara* curent1 = curent->lista;
			NodLSecundara* prev = NULL;
			while (curent1 != NULL)
			{
				if (curent1->info.durataOre < durata)
				{
					if (curent1 == curent->lista)
					{
						curent->lista = curent1->next;
					}
					else
					{
						prev->next = curent1->next;
					}
					NodLSecundara* temp = curent1->next;
					free(curent1->info.dataProgramare);
					free(curent1->info.numeClient);
					free(curent1->info.serviciiEfectuate);
					free(curent1);
					curent1 = temp;
				}
				else
				{
					prev = curent1;
					curent1 = curent1->next;
				}
			}
			curent = curent->nextLP;
		}
	}
}

Programare deepCopy(Programare prog)
{
	Programare p;
	p.numeClient = (char*)malloc(sizeof(char) * (strlen(prog.numeClient) + 1));
	strcpy(p.numeClient, prog.numeClient);

	p.dataProgramare= (char*)malloc(sizeof(char) * (strlen(prog.dataProgramare) + 1));
	strcpy(p.dataProgramare, prog.dataProgramare);

	p.durataOre = prog.durataOre;
	p.costProgramare = prog.costProgramare;

	p.serviciiEfectuate = (char*)malloc(sizeof(char) * (strlen(prog.serviciiEfectuate) + 1));
	strcpy(p.serviciiEfectuate, prog.serviciiEfectuate);

	return p;
}
void lista_stiva(NodLSecundara** lista,Programare p)
{
	NodLSecundara* nou = (NodLSecundara*)malloc(sizeof(NodLSecundara));
	nou->info = deepCopy(p);
	nou->next = (*lista);
	(*lista) = nou;
}
void push(Stiva** s,Programare p)
{
	Stiva* nou = (Stiva*)malloc(sizeof(Stiva));
	nou->info = deepCopy(p);
	nou->next = ((*s));
	(*s) = nou;
}
void stiva_inserare(NodLPrincipala* lista, Stiva** stiva, const char* servicii)
{
	if (lista != NULL)
	{
		NodLPrincipala* curent = lista;
		while (curent != NULL)
		{
			NodLSecundara* curent1 = curent->lista;
			while (curent1 != NULL)
			{
				if (strcmp(curent1->info.serviciiEfectuate, servicii) == 0)
				{
					push(&(*stiva), curent1->info);
				}
				curent1 = curent1->next;
			}
			curent = curent->nextLP;
		}
	}
}
void afisare_stiva(Stiva* stiva)
{
	if (stiva != NULL)
	{
		Stiva* s = stiva;
		while (s != NULL)
		{
			printf("Data: %s\nNume: %s\nDurata ore: %.2f\nServiciu: %s\nPret: %d RON\n\n", s->info.dataProgramare, s->info.numeClient, s->info.durataOre, s->info.serviciiEfectuate, s->info.costProgramare);
			s = s->next;
		}
	}
}
//void push(NodLSecundara** lista,Programare p)
//{
//	NodLSecundara* nou = (NodLSecundara*)malloc(sizeof(NodLSecundara));
//	nou->info = deepCopy(p);
//	nou->next = (*lista);
//	(*lista) = nou;
//}
//void stiva_inserare(NodLPrincipala* lista, NodLSecundara** stiva, const char* servicii)
//{
//	if (lista != NULL)
//	{
//		NodLPrincipala* curent = lista;
//		while (curent != NULL)
//		{
//			NodLSecundara* curent1 = curent->lista;
//			while (curent1 != NULL)
//			{
//				if (strcmp(curent1->info.serviciiEfectuate,servicii)==0)
//				{
//					push(&(*stiva), curent1->info);
//				}
//				curent1 = curent1->next;
//			}
//			curent = curent->nextLP;
//		}
//	}
//}
//void afisare_stiva(NodLSecundara* stiva)
//{
//	if (stiva != NULL)
//	{
//		afisareLS(stiva);
//	}
//}
void main()
{
	NodLPrincipala* lprincipala =NULL;
	
	citire_din_fisier("Programare.txt", &lprincipala);
	afisareLP(lprincipala);

	printf("\n\nValoare totala %.2f", valoare_totala_data(lprincipala, "06-06-2024"));
	printf("\n\nMODIFICARE\n");
	modifica_cost(lprincipala, "John", 123);
	afisareLP(lprincipala);

	sterge_programari(&lprincipala, 1);
	afisareLP(lprincipala);

	printf("\n\nStiva:\n");
	NodLSecundara* stiva = NULL;
	stiva_inserare(lprincipala, &stiva, "Teeth Whitening");
	afisare_stiva(stiva);
}