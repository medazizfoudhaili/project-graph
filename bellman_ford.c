#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
int bellman_ford(liste *L, int nb, int source, float dist[], int pred[], int *passes) {
    for (int i = 0; i < nb; i++) {
        dist[i] = INF_FLT;
        pred[i] = -1;
    }
    dist[source] = 0.0f;

    int pass;
    for (pass = 1; pass < nb; pass++) {
        int any_update = 0;
        for (int u = 0; u < nb; u++) {
            for (sommet *p = L[u]; p; p = p->suiv) {
                int v = p->noeud - 1;
                if (dist[u] < INF_FLT && dist[u] + p->poids < dist[v]) {
                    dist[v] = dist[u] + p->poids;
                    pred[v] = u;
                    any_update = 1;
                }
            }
        }
        if (!any_update) break;
    }

    *passes = pass;

    for (int u = 0; u < nb; u++) {
        for (sommet *p = L[u]; p; p = p->suiv) {
            int v = p->noeud - 1;
            if (dist[u] < INF_FLT && dist[u] + p->poids < dist[v]) {
                return 0;
            }
        }
    }
    return 1;
}

