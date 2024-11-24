#include <stdio.h>
#include "map.h"
#include "tree.h"
#include "types.h"
#include <stdlib.h>
#include "loc.h"
#include <time.h>

void print_orientation(t_orientation ori) {
    switch (ori) {
        case NORTH: printf("Orientation actuelle : North\n"); break;
        case EAST: printf("Orientation actuelle : East\n"); break;
        case SOUTH: printf("Orientation actuelle : South\n"); break;
        case WEST: printf("Orientation actuelle : West\n"); break;
        default: printf("Orientation inconnue\n"); break;
    }
}

void measureExecutionTime(void (*func)(void *), void *args) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    func(args); // Appelle la fonction passée en paramètre
    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Temps d'execution : %f secondes\n", cpu_time_used);
}

int main() {
    t_map map;

    
#if defined(_WIN32) || defined(_WIN64)
    map = createMapFromFile("..\\maps\\example1.map");
#else
    map = createMapFromFile("../maps/example1.map");
#endif

    t_position start_pos = generateRandomPosition(&map);
    printf("Position de depart aleatoire : (%d, %d)\n", start_pos.x, start_pos.y);

    printf("Map created with dimensions %d x %d\n", map.y_max, map.x_max);
    for (int i = 0; i < map.y_max; i++)
    {
        for (int j = 0; j < map.x_max; j++)
        {
            printf("%d ", map.soils[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < map.y_max; i++)
    {
        for (int j = 0; j < map.x_max; j++)
        {
            printf("%-5d ", map.costs[i][j]);
        }
        printf("\n");
    }
    displayMap(map);


    t_orientation start_ori = generateRandomOrientation();
    print_orientation(start_ori);

    int availability[7] = {22, 15, 7, 7, 21, 21, 7}; // Disponibilités des mouvements
    t_move phase_moves[9];

    clock_t start, end;
    double cpu_time_used;

    int n = 100000000; // Exemple de boucle
    long sum = 0;

    start = clock();

    t_node *root = buildTree(&map, start_pos, start_ori, 0, phase_moves);
    if (!root) {
        printf("Erreur : impossible de construire l'arbre\n");
        return EXIT_FAILURE;
    }
    printf("Arbre construit avec succes\n");

    t_node *best_leaf = NULL;
    int min_cost = INT_MAX;

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Temps d'execution : %f secondes\n", cpu_time_used);


    start = clock();
    t_position positions[5];
    int total_cost = 0;
    while (1) {
        // Générer les 9 mouvements pour la phase
        generatePhaseMoves(phase_moves, availability);

        // Exécuter la phase optimale
        findOptimalPhase(&map, &start_pos, &start_ori, phase_moves);

        // Si le robot a atteint la base (position avec un coût de 0)
        if (map.costs[start_pos.y][start_pos.x] == 0) {
            printf("Le robot a atteint la base. Exploration terminee.\n");
            break;
        }

        // Réinitialiser les disponibilités des mouvements pour la prochaine phase
        resetMovementAvailability(availability);

        printf("\n--- Nouvelle phase ---\n");
    }

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Temps d'execution : %f secondes\n", cpu_time_used);

    return 0;
}