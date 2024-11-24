//
// Created by flasque on 19/10/2024.
//

#include "loc.h"
#include "moves.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h> // Pour rand() et srand()

t_localisation loc_init(int x, int y, t_orientation ori)
{
    t_localisation loc;
    loc.pos.x = x;
    loc.pos.y = y;
    loc.ori = ori;
    return loc;
}

int isValidLocalisation(t_position loc, int x_max, int y_max)
{
    return (loc.x >= 0 && loc.x < x_max && loc.y >= 0 && loc.y < y_max);
}

t_position LEFT(t_position pos)
{
    t_position new_pos;
    new_pos.x = pos.x - 1;
    new_pos.y = pos.y;
    return new_pos;
}

t_position RIGHT(t_position pos)
{
    t_position new_pos;
    new_pos.x = pos.x + 1;
    new_pos.y = pos.y;
    return new_pos;
}

t_position UP(t_position pos)
{
    t_position new_pos;
    new_pos.x = pos.x;
    new_pos.y = pos.y - 1;
    return new_pos;
}

t_position DOWN(t_position pos)
{
    t_position new_pos;
    new_pos.x = pos.x;
    new_pos.y = pos.y + 1;
    return new_pos;
}

void executeMovementPhase(t_map *map, t_position *pos, t_orientation *orientation, t_move *moves) {
    for (int i = 0; i < 5; i++) {
        adjust_move_for_terrain(map, pos, &moves[i]);
        apply_move(&map, &pos, &orientation, &move, &moves);

        // Vérifier si la position est valide
        if (!isValidLocalisation(*pos, map->x_max, map->y_max)) {
            printf("MARC est sorti de la carte ou est tombé dans une crevasse !\n");
            return;
        }
    }
}

t_position generateRandomPosition(t_map *map) {
    t_position random_pos;
    srand(time(NULL)); // Initialiser la graine aléatoire une seule fois

    do {
        random_pos.x = rand() % map->x_max-1; // Générer une position aléatoire en x
        random_pos.y = rand() % map->y_max-1; // Générer une position aléatoire en y
    } while (!isValidLocalisation(random_pos, map->x_max, map->y_max) ||
             map->soils[random_pos.y][random_pos.x] == CREVASSE); // Assurez-vous que la position est valide

    return random_pos;
}
t_orientation generateRandomOrientation() {
    return rand() % 4; // Génère un entier entre 0 et 3 correspondant aux 4 orientations possibles
}