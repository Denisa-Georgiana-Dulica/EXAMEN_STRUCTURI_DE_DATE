#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

struct Student
{
	int id;
	char* name;
	float avg;
};
typedef struct Student Student;
typedef struct Nod Nod;
typedef struct HashTable HashTable;
typedef struct NodStiva NodStiva;
struct Nod
{
	Nod* next;
	Student info;
};
Student initializare_implicit()
{
	Student s;
	s.id = 0;
	s.avg = 0;
	s.name = NULL;
	return s;
}
Student cautate_dupa_id(Nod* varf, int id)
{
	if (varf != NULL)
	{
		int gasit = 0;
		Student s;
		Nod* curent = varf;
		while (curent != NULL)
		{
			if (curent->info.id == id)
			{
				gasit = 1;
				s = curent->info;
			}
			curent = curent->next;
		}
		if (gasit == 0)
		{
			printf("nu exista student cu id-ul asta");
			Student s1=initializare_implicit();
			return s1;
		}
		return s;
	}
}

void put(Nod** varf,Student s)//la final
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = s;
	nou->next = NULL;
	Nod* curent = (*varf);
	while (curent->next!=NULL)
	{
		curent = curent->next;
	}
	curent->next = nou;
}
Student get(Nod** varf)// extragere de la inceput
{
	if ((*varf) != NULL)
	{
		Nod* curent = (*varf);
		Student s = (*varf)->info;
		(*varf) = (*varf)->next;
		free(curent);
		return s;
	}
	return initializare_implicit();
}
void citire_fisier(Nod** varf, const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	if (f == NULL)
	{
		printf("Fisierul nu exista");
		return;
	}

	char buffer[100];
	char delimitator[] = ",\n";
	while (fgets(buffer, 100, f) != NULL)
	{
		Student c;
		char* element = strtok(buffer, delimitator);
		c.id = atoi(element);

		element = strtok(NULL, delimitator);
		c.name = (char*)malloc(sizeof(char) * (strlen(element) + 1));
		strcpy(c.name, element);


		element = strtok(NULL, delimitator);
		c.avg = atof(element);


		put(&varf, c);
	}
}
void afisareStudent(Student c)
{
	printf("\nCod: %d Nume: %s Medie %.2f ", c.id, c.name, c.avg);
}
void afisare_coada(Nod* varf)
{
	if (varf != NULL)
	{
		while (varf != NULL)
		{
			afisareStudent(varf->info);
			varf = varf->next;
		}
	}
	else
		printf("Coada goala");
}
void dezalocare(Nod** varf)
{
	if ((*varf) != NULL)
	{
		while ((*varf) != NULL)
		{
			free((*varf)->info.name);
			get(&(*varf));
		}
		(*varf) = NULL;
	}
}
void main()
{
	Nod* varf = NULL;
	citire_fisier(&varf, "Student.txt");
	afisare_coada(varf);
	printf("\n\nStudent:\n");
	Student s = get(&varf);
	afisareStudent(s);
	printf("\n\nDupa get:\n");
	afisare_coada(varf);

	Student s1 = cautate_dupa_id(varf, 43);
	afisareStudent(s1);
	dezalocare(&varf);
	afisare_coada(varf);
}