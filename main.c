#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"

int bellman_ford(liste *L, int nb, int source, float dist[], int pred[], int *passes);
int dijkstra(liste *L, int nb, int source, float dist[], int pred[]);

static double diff_timespec(const struct timespec *t1, const struct timespec *t2) {
    return (t2->tv_sec - t1->tv_sec) + (t2->tv_nsec - t1->tv_nsec) / 1e9;
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

void afficher_chemin_console(int pred[], int v) {
    if (v < 0) return;
    if (pred[v] >= 0) {
        afficher_chemin_console(pred, pred[v]);
        printf("-> ");
    }
    printf("%d", v + 1);
}

void afficher_chemin_fichier(int pred[], int v, FILE *f) {
    if (v < 0) return;
    if (pred[v] >= 0) {
        afficher_chemin_fichier(pred, pred[v], f);
        fprintf(f, "-> ");
    }
    fprintf(f, "%d", v + 1);
}

int main(void) {
    const char *files[] = {"graphe1.txt", "graphe2.txt", "graphe3.txt", "graphe4.txt","graphe5.txt"};
    int nF = sizeof(files) / sizeof(*files);
    FILE *fout = fopen("resultats.txt", "w");
    if (!fout) {
        perror("Ouverture resultats.txt");
        return 1;
    }

    for (int i = 0; i < nF; i++) {
        int nb, src = 0, passesBF = 0, relaxD;
        liste *L = remplir_list_adj(&nb, files[i]);
        float *dist = (float *)malloc(nb * sizeof *dist);
        int *pred = (int *)malloc(nb * sizeof *pred);
        struct timespec t0, t1;
        double dtBF, dtD;

        printf("\n===== TEST %s =====\n", files[i]);
        fprintf(fout, "\n===== TEST %s =====\n", files[i]);

        clock_gettime(CLOCK_MONOTONIC, &t0);
        bellman_ford(L, nb, src, dist, pred, &passesBF);
        clock_gettime(CLOCK_MONOTONIC, &t1);
        dtBF = diff_timespec(&t0, &t1);

        printf("--- Bellman–Ford ---\n");
        fprintf(fout, "--- Bellman–Ford ---\n");
        if (!passesBF) {
            printf("Cycle négatif détecté !\n");
            fprintf(fout, "Cycle négatif détecté !\n");
        } else {
            for (int v = 0; v < nb; v++) {
                printf("1->%d : %.2f (", v+1, dist[v]);
                afficher_chemin_console(pred, v);
                printf(")\n");

                fprintf(fout, "1->%d : %.2f (", v+1, dist[v]);
                afficher_chemin_fichier(pred, v, fout);
                fprintf(fout, ")\n");
            }
        }
        printf("Temps BF   : %.9f s\n", dtBF);
        fprintf(fout, "Temps BF   : %.9f s\n", dtBF);
        printf("Passes BF  : %d\n", passesBF);
        fprintf(fout, "Passes BF  : %d\n", passesBF);

        clock_gettime(CLOCK_MONOTONIC, &t0);
        relaxD = dijkstra(L, nb, src, dist, pred);
        clock_gettime(CLOCK_MONOTONIC, &t1);
        dtD = diff_timespec(&t0, &t1);

        printf("\n--- Dijkstra ---\n");
        fprintf(fout, "\n--- Dijkstra ---\n");
        for (int v = 0; v < nb; v++) {
            printf("1->%d : %.2f (", v+1, dist[v]);
            afficher_chemin_console(pred, v);
            printf(")\n");

            fprintf(fout, "1->%d : %.2f (", v+1, dist[v]);
            afficher_chemin_fichier(pred, v, fout);
            fprintf(fout, ")\n");
        }
        printf("Temps Dijkstra: %.9f s\n", dtD);
        fprintf(fout, "Temps Dijkstra: %.9f s\n", dtD);
        printf("Relaxations   : %d\n", relaxD);
        fprintf(fout, "Relaxations   : %d\n", relaxD);

        free(dist);
        free(pred);
        liberer_graphe(L, nb);
    }

    fclose(fout);
    return 0;
}

