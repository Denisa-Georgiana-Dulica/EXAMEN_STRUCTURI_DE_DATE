//Implementaţi o aplicaţie în limbajul C care implementează soluţii la probleme de localizare geografică.
//1. Scrieţi secvenţa de cod sursă pentru crearea unei structuri de tip Graf implementată prin Listă de Adiacenţă.
// Inserarea unei muchii în graf se implementează în funcţia principală pentru un număr de minim 6 localităţi citite dintr - un fişier de intrare. (3p)
//Structura Localitate se va defini astfel încât să conţină minim 5 câmpuri din care minim două câmpuri sunt definite ca variabile pointer.
//2. Scrieţi şi apelaţi funcţia pentru determinarea localităţilor din Graf cu număr maxim de conexiuni cu alte localităţi.Funcţia returnează un vector care conţine denumirea localităţilor cu maximul de conexiuni. (2p)
//3. Scrieţi secvenţa de cod care copiază datele localităţilor din structura Graf creată la punctul 1) într - o structură Arbore Binar de Căutare.Sunt considerate localităţile a căror denumire încep cu o literă specificată.Cele două structuri de date NU partajează zone de memorie heap. (2p)
//4. Scrieţi şi apelaţi funcţia pentru extragerea într - un vector a localităţilor plasate în nodurile frunză în Arborele Binar de Căutare creat la punctul 3).Nodurile frunză sunt şterse din structura arborescentă. (2p)
//5. Scrieţi secvenţa de cod care dezalocă structurile Listă de Adiacenţă, Vectori, Arbore Binar de Căutare şi toate structurile auxiliare utilizate în implementarea cerinţelor(dacă este necesar). (1p)

////sub 16 arb
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

struct Localitate
{
	int numar_oras;
	char* denumire;

};
typedef struct NodP NodP;
typedef struct NodS NodS;
typedef struct Localitate Localitate;

struct NodP
{
	NodP* next;
	NodS* lista;
	Localitate info;
};
struct NodS
{
	NodS* next;
	NodP* nod;
};

void inserare_final_LP(NodP** graf, Localitate l)
{
    NodP* nou = (NodP*)malloc(sizeof(NodP));
    nou->next = NULL;
    nou->info = l;
    nou->lista = NULL;
    if ((*graf) != NULL)
    {
        NodP* curent = (*graf);
        while (curent->next != NULL)
        {
            curent = curent->next;
        }
        curent->next = nou;
    }
    else
    {
        (*graf) = nou;
    }
}
void inserare_final_LS(NodS** lista, NodP* nod)
{
    NodS* nou = (NodS*)malloc(sizeof(NodS));
    nou->next = NULL;
    nou->nod = nod;
    if ((*lista) != NULL)
    {
        NodP* curent = (*lista);
        while (curent->next != NULL)
        {
            curent = curent->next;
        }
        curent->next = nou;
    }
    else
    {
        (*lista) = nou;
    }
}
NodP* cautare_nod(NodP* graf, int cheie)
{
    if (graf != NULL)
    {
        NodP* curent = graf;
        while (curent!= NULL)
        {
            if (curent->info.numar_oras == cheie)
            {
                return curent;
            }
            curent = curent->next;
        }
        return curent;
    }
}
void inserare_muchii(NodP* graf, int nod1, int nod2)
{
    NodP* nodStart = cautare_nod(graf, nod1);
    NodP* nodStop = cautare_nod(graf, nod2);
    inserare_final_LS(&nodStart->lista, nodStop);
    inserare_final_LS(&nodStop->lista, nodStart);
}
void citire_fisier(const char* nume_fisier,NodP** graf)
{
    FILE* f = fopen(nume_fisier, "r");
    if (f == NULL)
    {
        printf("Nu s-a putut deschide fișierul %s\n", nume_fisier);
        return;
    }

    char buffer[100];
    char delimitator[] = ",\n";
    while (fgets(buffer, 100, f) != NULL)
    {
        Localitate loc;
        char* element = strtok(buffer, delimitator);

        loc.numar_oras = atoi(element);

        element = strtok(NULL, delimitator);
        loc.denumire = (char*)malloc(sizeof(char) * (strlen(element) + 1));
        strcpy(loc.denumire, element);

        inserare_final_LP(&(*graf), loc);
    }
    fclose(f);
}
void afisare_localitate(Localitate loc)
{
	printf("Numar oras: %d, Denumire: %s\n", loc.numar_oras, loc.denumire);
}
typedef struct Coada Coada;
struct Coada
{
    Coada* next;
    int numar;

};

void put(Coada** coada, int nod)//INSERARE LA FINAL
{
    Coada* nou = (Coada*)malloc(sizeof(Coada));
    nou->next = NULL;
    nou->numar = nod;
    if ((*coada) != NULL)
    {
        Coada* curent = (*coada);
        while (curent->next != NULL)
        {
            curent = curent->next;
        }
        curent->next = nou;
    }
    else
    {
        (*coada) = nou;
    }
}

int get(Coada** coada)
{
    if ((*coada) != NULL)
    {
        Coada* temp = (*coada);
        int rez = (*coada)->numar;
        (*coada) = temp->next;
        free(temp);
        return rez;
    }
}

int numar_noduri(NodP* graf)
{
    if (graf != NULL)
    {
        NodP* curent = graf;
        int nr = 0;
        while (curent != NULL)
        {
            nr++;
            curent = curent->next;
        }
        return nr;
    }
    else
        return 0;
}

void parcurgere_latime(NodP* graf, int nodInceput)
{
    if (graf != NULL)
    {
        Coada* coada = NULL;
        int nr = numar_noduri(graf);
        char* vizitate = (char*)malloc(sizeof(char) * nr);
        for (int i = 0; i < nr; i++)
        {
            vizitate[i] = 0;
        }

        put(&coada, nodInceput);
        vizitate[nodInceput] = 1;

        while (coada)
        {
            int elem=get(&coada);
            NodP* nod = cautare_nod(graf, elem);
            afisare_localitate(nod->info);

            NodS* temp = nod->lista;
            while (temp != NULL)
            {
                if (vizitate[temp->nod->info.numar_oras] == 0)
                {
                    vizitate[temp->nod->info.numar_oras] = 1;
                    put(&coada, temp->nod->info.numar_oras);
                }
                temp = temp->next;
            }

        }
        if (vizitate != NULL)
        {
            free(vizitate);
        }
    }
}

void stergere_lista(NodS** lista)
{
    if ((*lista) != NULL)
    {
        while ((*lista) != NULL)
        {
            NodS* curent = (*lista)->next;
            free((*lista));
            (*lista) = curent;
        }
        (*lista) = NULL;
    }
}
void stergere_graf(NodP** graf)
{
    if ((*graf) != NULL)
    {
        while ((*graf) != NULL)
        {
            NodP* curent = (*graf)->next;
            free((*graf)->info.denumire);
            stergere_lista((*graf)->lista);
            free((*graf));
            (*graf) = curent;
        }
        (*graf) = NULL;
    }
}
void main()
{
    NodP* graf = NULL;
    citire_fisier("Localitate.txt", &graf);
    inserare_muchii(graf, 0, 1);
    inserare_muchii(graf, 0, 4);
    inserare_muchii(graf, 1, 2);
    inserare_muchii(graf, 2, 4);
    inserare_muchii(graf, 4, 3);
    inserare_muchii(graf, 2, 5);
    parcurgere_latime(graf, 0);
    stergere_graf(&graf);
    parcurgere_latime(graf, 0);
}
