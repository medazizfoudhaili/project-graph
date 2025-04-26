#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
int dijkstra(liste *L, int nb, int source, float dist[], int pred[]) {
    int *vis = (int *)calloc(nb, sizeof(int)), relax = 0;
    for (int i = 0; i < nb; i++) {
        dist[i] = INF_FLT;
        pred[i] = -1;
    }
    dist[source] = 0.0f;

    for (int iter = 0; iter < nb; iter++) {
        float best = INF_FLT;
        int u = -1;
        for (int i = 0; i < nb; i++) {
            if (!vis[i] && dist[i] < best) {
                best = dist[i];
                u = i;
            }
        }
        if (u < 0) break;
        vis[u] = 1;
        for (sommet *p = L[u]; p; p = p->suiv) {
            int v = p->noeud - 1;
            if (!vis[v] && dist[u] + p->poids < dist[v]) {
                dist[v] = dist[u] + p->poids;
                pred[v] = u;
                relax++;
            }
        }
    }
    free(vis);
    return relax;
}

