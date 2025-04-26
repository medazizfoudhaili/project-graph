#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"

int bellman_ford(liste *L, int nb, int source, float dist[], int pred[], int *passes);
int dijkstra(liste *L, int nb, int source, float dist[], int pred[]);

void afficher_chemin(int pred[], int v) {
    if (v < 0) return;
    if (pred[v] >= 0) {
        afficher_chemin(pred, pred[v]);
        printf("-> ");
    }
    printf("%d", v + 1);
}

void liberer_graphe(liste *L, int nb) {
    for (int i = 0; i < nb; i++) {
        while (L[i]) {
            sommet *tmp = L[i]->suiv;
            free(L[i]);
            L[i] = tmp;
        }
    }
    free(L);
}

static double diff_timespec(const struct timespec *t1,
                            const struct timespec *t2) {
    return (t2->tv_sec  - t1->tv_sec) +
           (t2->tv_nsec - t1->tv_nsec) / 1e9;
}

int main(void) {
    const char *files[] = {
        "graphe1.txt",
        "graphe2.txt",
        "graphe3.txt",
        "graphe4.txt"
    };
    int nF = sizeof(files) / sizeof(*files);

    for (int idx = 0; idx < nF; idx++) {
        int nb, src = 0;
        printf("\n===== TEST %s =====\n", files[idx]);

        liste *L = remplir_list_adj(&nb, files[idx]);
        if (!L) { fprintf(stderr, "Erreur chargement %s\n", files[idx]); continue; }

        float *dist = (float *)malloc(nb * sizeof *dist);
        int   *pred = (int   *)malloc(nb * sizeof *pred);
        if (!dist || !pred) { free(dist); free(pred); liberer_graphe(L, nb); continue; }

        struct timespec t0, t1;
        double dtBF, dtD;
        int passesBF = 0, relaxD;

        clock_gettime(CLOCK_MONOTONIC, &t0);
        bellman_ford(L, nb, src, dist, pred, &passesBF);
        clock_gettime(CLOCK_MONOTONIC, &t1);
        dtBF = diff_timespec(&t0, &t1);

        printf("\n--- Bellman–Ford ---\n");
        if (!passesBF) {
            printf("Cycle négatif détecté !\n");
        } else {
            for (int v = 0; v < nb; v++) {
                printf("1->%d : %.2f (", v + 1, dist[v]);
                afficher_chemin(pred, v);
                printf(")\n");
            }
        }
        printf("Temps BF   : %.9f s\n", dtBF);
        printf("Passes BF  : %d\n", passesBF);

        clock_gettime(CLOCK_MONOTONIC, &t0);
        relaxD = dijkstra(L, nb, src, dist, pred);
        clock_gettime(CLOCK_MONOTONIC, &t1);
        dtD = diff_timespec(&t0, &t1);

        printf("\n--- Dijkstra ---\n");
        for (int v = 0; v < nb; v++) {
            printf("1->%d : %.2f (", v + 1, dist[v]);
            afficher_chemin(pred, v);
            printf(")\n");
        }
        printf("Temps Dijkstra: %.9f s\n", dtD);
        printf("Relaxations   : %d\n", relaxD);

        free(dist);
        free(pred);
        liberer_graphe(L, nb);
    }

    return 0;
}

