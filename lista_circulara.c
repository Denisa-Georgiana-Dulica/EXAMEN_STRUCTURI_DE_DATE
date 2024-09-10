#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Song Song;
struct Song
{
	char* title;
	char* artist;
	long int plays;
	int duration;
	char favourite;
};
typedef struct Nod Nod;
struct Nod
{
	Song info;
	Nod* next;
	Nod* prev;
};

typedef struct ListaDubla ListaDubla;
struct ListaDubla
{

	Nod* prim;
	Nod* ultim;
};

Song* adauga_vector(Song* vector_vechi, Song s, int* nr_songs)
{
	Song* vector_nou = (Song*)malloc(sizeof(Song) * ((*nr_songs) + 1));
	for (int i = 0; i < (*nr_songs); i++)
	{
		vector_nou[i] = vector_vechi[i];
	}
	vector_nou[(*nr_songs)] = s;
	(*nr_songs)++;

	if (vector_vechi != NULL)
	{
		free(vector_vechi);
	}
	return vector_nou;
}

Song* citireDinFisier(const char* nume_fisier, int* nr_songs)
{
	FILE* file = fopen(nume_fisier, "r");
	if (file == NULL)
	{
		printf("Fisierul nu s-a deschis");
	}

	Song* vector_song = NULL;
	(*nr_songs) = 0;

	char buffer[100];
	char delimitator[] = ",\n";
	while (fgets(buffer, 100, file) != NULL)
	{
		Song s;
		char* element = strtok(buffer, delimitator);
		s.title = (char*)malloc(sizeof(char) * (strlen(element) + 1));
		strcpy(s.title, element);

		element = strtok(NULL, delimitator);
		s.artist = (char*)malloc(sizeof(char) * (strlen(element) + 1));
		strcpy(s.artist, element);

		element = strtok(NULL, delimitator);
		s.plays = atoi(element);

		element = strtok(NULL, delimitator);
		s.duration = atoi(element);

		element = strtok(NULL, delimitator);
		s.favourite = element[0];


		vector_song = adauga_vector(vector_song, s, &(*nr_songs));
	}
	fclose(file);
	return vector_song;
}

void afisareSong(Song s)
{
	printf("\nTitle: %s Artist: %s plays: %d duration: %d favourite: %c", s.title, s.artist, s.plays, s.duration, s.favourite);
}

void inserare_inceput(ListaDubla* lista, Song s)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = s;
	if ((*lista).prim != NULL)
	{
		nou->next = (*lista).prim;
		nou->prev = (*lista).ultim;
		(*lista).prim->prev = nou;
		(*lista).ultim->next = nou;
		(*lista).prim = nou;
	}
	else
	{
		nou->next = nou;
		nou->prev = nou;
		(*lista).prim = nou;
		(*lista).ultim = nou;
	}
}
void afisare(ListaDubla lista)
{
	if (lista.prim != NULL)
	{
		Nod* curent = lista.prim;
		do
		{
			afisareSong(curent->info);
			curent = curent->next;
		} while (curent != lista.prim);
	}
}
void sterge(ListaDubla* lista, const char* nume)
{
	if ((*lista).prim != NULL)
	{
		Nod* curent = (*lista).prim;
		do
		{
			if (strcmp(curent->info.title, nume) == 0)
			{
				if (curent->prev == (*lista).ultim)
				{
					if (curent->next != lista->prim)
					{
						(*lista).prim = curent->next;
						curent->next->prev = (*lista).ultim;
					}
					else
					{
						(*lista).prim = NULL;
						(*lista).ultim = NULL;
					}

				}
				else
				{
					if (curent->next == lista->prim)
					{
						curent->prev->next = lista->prim;
						lista->ultim = curent->prev;
					}
					else
					{
						curent->prev->next = curent->next;
						curent->next->prev = curent->prev;
					}
				}
				Nod* temp = curent->next;
				free(curent->info.title);
				free(curent->info.artist);
				free(curent);
				curent = temp;
			}
			else
				curent = curent->next;
		} while (curent != lista->prim);
	}
}
void main()
{
	ListaDubla lista;
	lista.prim = NULL;
	lista.ultim = NULL;

	Song* vector_song = NULL;
	int nr_songs = 0;
	vector_song = citireDinFisier("Song.txt", &nr_songs);
	printf("\nLista dubla circulara inserare inceput\n\n");
	for (int i = 0; i < nr_songs; i++)
	{
		inserare_inceput(&lista, vector_song[i]);
	}
	afisare(lista);

	printf("\n\nStergere melodie din playlist si afisare final-inceput:\n");
	sterge(&lista, "Despacito");
	afisare(lista);
}