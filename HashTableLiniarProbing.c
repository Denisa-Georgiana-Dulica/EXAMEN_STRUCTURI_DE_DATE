#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

#define SIZE 100
struct Student {
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
struct HashTable
{
	Nod** vector_liste;
	int nr_elemente;
};
HashTable initializare(int dim)
{
	HashTable ht;
	ht.nr_elemente = dim;
	ht.vector_liste = (Nod**)malloc(sizeof(Nod*) * ht.nr_elemente);
	for (int i = 0; i < ht.nr_elemente; i++)
	{
		ht.vector_liste[i] = NULL;
	}
	return ht;
}

int functieHash(int dimensiune, const char* nume_student)
{
	if (dimensiune > 0)
	{
		int functie = 0;
		for(int i=0;i<strlen(nume_student);i++)
		{ 
			functie += nume_student[i];
		}
		return functie % dimensiune;
		
	}
	else
		return -1;
}
Student deepCopy(Student s)
{
	Student s1;
	s1.avg = s.avg;
	s1.id = s.id;
	s1.name = (char*)malloc(sizeof(char) * (strlen(s.name) + 1));
	strcpy(s1.name, s.name);
	return s1;
}
void inserare_nod(Nod** lista,Student s)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = deepCopy(s);
	nou->next = NULL;
	(*lista) = nou;
}
void dezalocare_lista(Nod** lista)
{
	if ((*lista) != NULL)
	{
		while ((*lista) != NULL)
		{
			Nod* curent = (*lista)->next;
			free((*lista)->info.name);
			free((*lista));
			(*lista) = curent;
		}
		(*lista) = NULL;
	}
}
void dezalocare(HashTable* ht)
{
	if (ht->nr_elemente > 0)
	{
		for (int i = 0; i < ht->nr_elemente; i++)
		{
			dezalocare_lista(&ht->vector_liste[i]);
		}
	}
}
void inserare_in_hashtable(HashTable* ht, Student s)
{
	if (ht->nr_elemente > 0)
	{
		int pozitie = functieHash(ht->nr_elemente, s.name);
		char inserat = 0;
		for(int i=pozitie;i<ht->nr_elemente && inserat!=1 ;i++)//NU UITA DE && inserat!=1
		{
			if (ht->vector_liste[i] == NULL)
			{
				inserare_nod(&ht->vector_liste[i], s);
				inserat = 1;
			}
		}
		if (inserat == 0)//nu mai are loc de inserat
		{
			while (inserat!=1)
			{
				int noua_dim = ht->nr_elemente + SIZE;
				HashTable ht1 = initializare(noua_dim);
				for (int i = 0; i < ht->nr_elemente; i++)
				{
					if (ht->vector_liste[i] != NULL)
					{
						inserare_nod(&ht1.vector_liste[i], ht->vector_liste[i]->info);//fiecare lista are cate un student
					}
				}
					dezalocare(&(*ht));
					(*ht)= ht1;
					ht->nr_elemente = ht1.nr_elemente;
					pozitie = functieHash(ht->nr_elemente, s.name);//NU UITA SA MAI CALCULEZI O DATA FUNCTIA HASH
					for (int i = pozitie; i < ht->nr_elemente && inserat != 1; i++)//NU UITA DE && inserat!=1
					{
						if (ht->vector_liste[i] == NULL)
						{
							inserare_nod(&ht->vector_liste[i], s);
							inserat = 1;
						}
					}
					
				
				
			}
		}
		
	}
}
void citire_fisier(HashTable* ht, const char* numeFisier)
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
		c.avg= atof(element);

	
		inserare_in_hashtable(&(*ht), c);
	}
}
void afisareStudent(Student c)
{
	printf("\nCod: %d Nume: %s Medie %.2f ", c.id, c.name, c.avg);
}
void afisareHashTable(HashTable ht)
{
	if (ht.nr_elemente > 0)
	{
		for (int i = 0; i < ht.nr_elemente; i++)
		{
			if (ht.vector_liste[i] != NULL)
			{
				Nod* curent = ht.vector_liste[i];
				printf("\nCluster %d\n", i + 1);
				while (curent != NULL)
				{

					afisareStudent(curent->info);

					curent = curent->next;
				}
			}
		}
	}
}
void cautare_student(HashTable ht, const char* nume_stud)
{
	int gasit = 0;
	int pozitie = functieHash(ht.nr_elemente, nume_stud);
	for (int i = pozitie; i < ht.nr_elemente && gasit == 0; i++)
	{
		Nod* curent = ht.vector_liste[i];
		while (curent != NULL)
		{
			if (strcmp(curent->info.name, nume_stud) == 0)
			{
				gasit = 1;
				afisareStudent(curent->info);
				break; 
			}
			curent = curent->next;
		}
	}
	if (gasit == 0)
	{
		printf("Nu s-a gasit studentul");
	}
}
void main()
{
	HashTable ht = initializare(4);
	citire_fisier(&ht, "Student.txt");
	afisareHashTable(ht);
	cautare_student(ht, "Ionescu George");
}
