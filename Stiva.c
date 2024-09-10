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

struct Nod
{
	Nod* next;
	Student info;
};
struct Vector
{
	Student* vector;
	int nrElemente;
};

void push(Student s, Nod** varf)//inserare la inceput
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = s;
	nou->next = (*varf);
	(*varf) = nou;
}
Student initializare_implicit()
{
	Student s;
	s.id = 0;
	s.avg = 0;
	s.name = NULL;
}
Student pop(Nod** varf)//extrag de la inceput (NU STERG INFO)
{
	if ((*varf) != NULL)
	{
		Student temp = (*varf)->info;
		Nod* curent = (*varf);
		(*varf) = (*varf)->next;
		free(curent);

		return temp;
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


		push(c, &(*varf));
	}
}
void afisareStudent(Student c)
{
	printf("\nCod: %d Nume: %s Medie %.2f ", c.id, c.name, c.avg);
}
void parcurge_stiva(Nod* varf)//in sens invers
{
	if (varf != NULL)
	{
		while (varf != NULL)
		{
			afisareStudent(varf->info);
			varf = varf->next;
		}
	}
}
void parcurgere_recursiv(Nod* varf)//in sens invers
{
	if (varf != NULL)
	{
		parcurgere_recursiv(varf->next);
		afisareStudent(varf->info);
	}
}
void parcurgere_iterativ(Nod* varf)
{
	Nod* aux = NULL;
	Nod* curent = varf;
	while (curent != NULL)
	{
		push(curent->info, &aux);
		curent = curent->next;
	}
	while (aux != NULL)
	{
		Student s = pop(&aux);
		afisareStudent(s);
	}
}
void cautare_dupa_id(int id,Nod* varf)
{
	if (varf != NULL)
	{
		int gasit = 0;
		Nod* curent = varf;
		while (curent != NULL)
		{
			if (curent->info.id == id)
			{
				gasit = 1;
				afisareStudent(curent->info);
			}
			curent = curent->next;
		}
		if (gasit == 0)
		{
			printf("Studentul nu a fost gasit");
		}
	}
}
void stergere_ultimul_stud(Nod** varf)
{
	if ((*varf) != NULL)
	{
		Nod* curent = (*varf);
		if (curent->next == NULL)
		{
			free(curent->info.name);
			free(curent);
			(*varf) = NULL;
		}
		else
		{
			while (curent->next->next != NULL)
			{
				curent = curent->next;
			}
			free(curent->next->info.name);
			free(curent->next);
			curent->next = NULL;
		}

	}
}
Student deepCopy(Student s)
{
	Student st;
	st.id = s.id;
	st.avg = s.avg;
	st.name = (char*)malloc(sizeof(char*) * (strlen(s.name) + 1));
	strcpy(st.name, s.name);
	return st;
}
typedef struct Vector Vector;
void convertire_la_vector(Vector* v, Nod* varf)//pun stiva in vector
{
	if (varf != NULL)
	{
		Nod* curent = varf;
		int nr = 0;
		while (curent != NULL)
		{
			nr++;
			curent=curent->next;
		}
		v->vector = (Student*)malloc(sizeof(Student) * nr);
		v->nrElemente = nr;
		curent = varf;
		int count = 0;
		while (curent != NULL)
		{
			v->vector[count] = deepCopy(curent->info);
			count++;
			curent = curent->next;
		}
	}
	
}
void dezalocare(Nod** varf)
{
	if ((*varf) != NULL)
	{
		while ((*varf) != NULL)
		{
			Nod* curent = (*varf)->next;
			free((*varf)->info.name);
			pop(&(*varf));
			(*varf) = curent;
		}
	}
}
void main()
{
	Nod* varf_stiva = NULL;
	citire_fisier(&varf_stiva, "Student.txt");
	printf("\n\nSTIVA:\n");
	//parcurge_stiva(varf_stiva);
	//parcurgere_recursiv(varf_stiva); //le afiseaza invers
	parcurgere_iterativ(varf_stiva);
	printf("\n\nPOP:\n");
	Student extras = pop(&varf_stiva);
	afisareStudent(extras);

	printf("\n\nCAUTA:\n");
	cautare_dupa_id(43, varf_stiva);

	printf("\n\nsterge ultimul nod:\n");
	stergere_ultimul_stud(&varf_stiva);
	parcurge_stiva(varf_stiva);

	printf("\n\nVECTOR:\n");
	Vector v;
	convertire_la_vector(&v, varf_stiva);
	for (int i = 0; i < v.nrElemente; i++)
	{
		afisareStudent(v.vector[i]);
	}

	dezalocare(&varf_stiva);
}