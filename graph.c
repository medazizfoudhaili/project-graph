#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

liste ajout_arc(liste l, int d, float v) {
    sommet *S = (sommet *)malloc(sizeof(sommet));
    if (!S) {
        perror("Allocation mémoire impossible");
        exit(EXIT_FAILURE);
    }
    S->noeud = d;
    S->poids = v;
    S->suiv  = l;
    return S;
}

liste* remplir_list_adj(int *nbSommets,const char *filrname) {
    FILE *f = fopen(filrname, "r");
    if (!f) {
        perror("Erreur ouverture graphe.txt");
        exit(EXIT_FAILURE);
    }
    int nbarc, s, d;
    float v;
    fscanf(f, "%d %d", nbSommets, &nbarc);
    liste *L = (liste *)calloc(*nbSommets, sizeof(liste));
    if (!L) {
        perror("Allocation mémoire impossible");
        exit(EXIT_FAILURE);
    }
    while (fscanf(f, "%d %d %f", &s, &d, &v) == 3) {
        L[s - 1] = ajout_arc(L[s - 1], d, v);
    }
    fclose(f);
    return L;
}

