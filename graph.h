#ifndef GRAPH_H
#define GRAPH_H

#include <float.h>
#define INF_FLT  FLT_MAX

typedef struct sommet {
    int    noeud;
    float  poids;
    struct sommet *suiv;
} sommet;

typedef sommet* liste;

liste* remplir_list_adj(int *nbSommets,const char *filename);
liste ajout_arc(liste l, int d, float v);

#endif

