#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
//LISTA DE ADIACENTA (LISTA DE LISTE)
typedef struct Autobuz Autobuz;
struct Autobuz
{
	int nr_linie; //cheie unica
	char* capatLinie;
};
typedef struct NodP NodP;
typedef struct NodS NodS;
struct NodS
{
    NodS* next;
    NodP* nod;
};
struct NodP
{
    NodP* next;
    NodS* lista;
    Autobuz info;
};

//functie de inserare la sfarsit in lista pp
void inserare_final_LP(NodP** graf,Autobuz a)
{
    NodP* nod_nou = (NodP*)malloc(sizeof(NodP));
    nod_nou->info = a;
    nod_nou->next = NULL;
    nod_nou->lista = NULL;
    if ((*graf) != NULL)
    {
        NodP* curent = (*graf);
        while (curent->next != NULL)
        {
            curent = curent->next;
        }
        curent->next = nod_nou;
    }
    else
    {
        (*graf) = nod_nou;
    }
}

Autobuz initializareAutobuz(int linie, const char* capat)
{
    Autobuz a;
    a.nr_linie = linie;
    a.capatLinie = (char*)malloc(sizeof(char) * strlen(capat + 1));
    strcpy(a.capatLinie, capat);
    return a;
}

//functie de inserare la final pentru lista s
void inserare_final_LS(NodS** lista, NodP* info)
{
    NodS* nou = (NodS*)malloc(sizeof(NodS));
    nou->next = NULL;
    nou->nod = info;
    if ((*lista) != NULL)
    {
        NodS* curent = (*lista);
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

//functie de cautare dupa cheie "nr linie autobuz" (numar unic)
NodP* cauta_dupa_cheie(NodP* graf, int nr_linie)
{
    if (graf != NULL)
    {
        NodP* curent = graf;
        while (curent != NULL)
        {
            if (curent->info.nr_linie == nr_linie)
            {
                return curent;
            }
            curent = curent->next;
        }
        return curent;//aici e null
    }
}

//functie de inserare a unei muchii (cand vreau sa ii adaug lui 1, nodul 2, in lista s a lui 1 adaug adresa lui 2, si in lista pp a lui 2 adaug adresa lui 1
//pentru ca am graf neorientat (daca am graf orientat inseram doar in lista s a lui 1, pe 2
void inserareMuchie(NodP* graf, int nod1, int nod2)
{
    NodP* nodStart = cauta_dupa_cheie(graf, nod1);
    NodP* nodStop = cauta_dupa_cheie(graf, nod2);
    inserare_final_LS(&nodStart->lista, nodStop);// (pentru graf orientat folosesc doar asta)
    inserare_final_LS(&nodStop->lista, nodStart);
 }

void citire_fisier(const char* nume_fisier,NodP** graf)
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
        Autobuz a;
        char* element = strtok(buffer, delimitator);
        a.nr_linie = atoi(element);

        element = strtok(NULL, delimitator);
        a.capatLinie = (char*)malloc(sizeof(char) * (strlen(element) + 1));
        strcpy(a.capatLinie, element);

        inserare_final_LP(&(*graf), a);
    }
    fclose(f);
}
void afisareAutobuz(Autobuz a)
{
    printf("id %d are capatul la %s \n", a.nr_linie, a.capatLinie);
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

//PARCURGERI
//1. LATIME - COADA (put - final / get - inceput)

typedef struct Coada Coada;
struct Coada
{
    Coada* next;
    int info;//pun doar cheia (pentru ca am functia care imi cauta dupa id)
};

void put(Coada** coada, int id)
{
    Coada* nou = (Coada*)malloc(sizeof(Coada));
    nou->next = NULL;
    nou->info = id;
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
        Coada* curent = (*coada);
        int temp = curent->info;
        (*coada) = curent->next;
        free(curent);
        return temp;
    }
    else
        return -1;
}

void parcurgere_in_Latime(NodP* graf, int nodPlecare)
{
    if (graf != NULL)
    {
        Coada* coada = NULL;
        //vector de vizitate
        int nr_noduri = numar_noduri(graf);
        char* vizitate = (char*)malloc(sizeof(char) * nr_noduri);
        for (int i = 0; i < nr_noduri; i++)
        {
            vizitate[i] = 0;
        }

        put(&coada, nodPlecare);
        vizitate[nodPlecare] = 1;

        while (coada != NULL)
        {
            int varf_coada = get(&coada);//extrag primul element din coada
            NodP* nod = cauta_dupa_cheie(graf, varf_coada);
            afisareAutobuz(nod->info);

            NodS* temp = nod->lista;
            while (temp != NULL)
            {
                if (vizitate[temp->nod->info.nr_linie] == 0)
                {
                    vizitate[temp->nod->info.nr_linie] = 1;
                    put(&coada, temp->nod->info.nr_linie);
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

//2. ADANCIME - STIVA (push - inceput / pop - inceput)
typedef struct Stiva Stiva;
struct Stiva
{
    int info;
    Stiva* next;
 };

void push(Stiva** stiva, int id)
{
    Stiva* nou = (Stiva*)malloc(sizeof(Stiva));
    nou->next = (*stiva);
    nou->info = id;
    if ((*stiva) != NULL)
    {
        (*stiva) = nou;
    }
    else
    {
        (*stiva) = nou;
    }
}

int pop(Stiva** stiva)
{
    if ((*stiva) != NULL)
    {
        int temp = (*stiva)->info;
        Stiva* curent = (*stiva);
        (*stiva) = curent->next;
        free(curent);
        return temp;
    }
}

void pacurgere_in_adancime(NodP* graf, int nodPlecare)
{
    if (graf != NULL)
    {
        Stiva* stiva = NULL;
        int nr_noduri = numar_noduri(graf);
        char* vizitate = (char*)malloc(sizeof(char) * nr_noduri);
        for (int i = 0; i < nr_noduri; i++)
        {
            vizitate[i] = 0;
        }

        push(&stiva, nodPlecare);
        vizitate[nodPlecare] = 1;

        while (stiva != NULL)
        {
            int nr = pop(&stiva);
            NodP* nod = cauta_dupa_cheie(graf, nr);
            afisareAutobuz(nod->info);

            NodS* temp = nod->lista;
            while (temp != NULL)
            {
                if (vizitate[temp->nod->info.nr_linie] == 0)
                {
                    vizitate[temp->nod->info.nr_linie] = 1;
                    push(&stiva, temp->nod->info.nr_linie);
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
void stergere_LS(NodS** lista)
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
void sterge_graf(NodP** graf)
{
    if ((*graf) != NULL)
    {
        while ((*graf) != NULL)
        {
            NodP* curent = (*graf)->next;
            free((*graf)->info.capatLinie);
            free((*graf));
            (*graf) = curent;
        }
        (*graf) = NULL;
    }
}
void main()
{
    NodP* graf = NULL;
    citire_fisier("Autobuz.txt", &graf);

    NodP* cautat = cauta_dupa_cheie(graf, 2);
    afisareAutobuz(cautat->info);

    inserareMuchie(graf, 0, 1);
    inserareMuchie(graf, 0, 4);
    inserareMuchie(graf, 1, 2);
    inserareMuchie(graf, 2, 4);
    inserareMuchie(graf, 4, 3);

    printf("\nNr noduri: %d", numar_noduri(graf));
    printf("\nPARCURGERE LATIME\n");
    parcurgere_in_Latime(graf, 0);
    printf("\nPARCURGERE ADANCIME\n");
    pacurgere_in_adancime(graf, 0);

    sterge_graf(&graf);
    pacurgere_in_adancime(graf,0);
}
