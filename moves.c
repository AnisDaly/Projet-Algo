//
// Created by flasque on 19/10/2024.
//

#include "moves.h"
#include "types.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
/* prototypes of local functions */
/* local functions are used only in this file, as helper functions */

/**
 * @brief Function to rotate the robot according to a move and its actual orientation
 * @param ori : the actual orientation of the robot
 * @param move : the move to do
 * @return the new orientation of the robot
 */
t_orientation rotate(t_orientation, t_move );

/**
 * @brief function to translate the robot according to a move and its actual position
 * @param loc : the actual localisation of the robot
 * @param move : the move to do
 * @return the new localisation of the robot
 */
t_localisation translate(t_localisation , t_move);

/* definition of local functions */

t_orientation rotate(t_orientation ori, t_move move)
{
    int rst;
    switch (move)
    {
        case T_LEFT:
            rst=3;
            break;
        case T_RIGHT:
            rst=1;
            break;
        case U_TURN:
            rst=2;
            break;
        default:
            break;
    }
    return (ori+rst)%4;
}

t_localisation translate(t_localisation loc, t_move move)
{
    /** rules for coordinates:
     *  - x grows to the right with step of +1
     *  - y grows to the bottom with step of +1
     *  - the origin (x=0, y=0) is at the top left corner
     */
    t_position res = loc.pos;
    switch (move) {
        case F_10:
            switch (loc.ori) {
                case NORTH:
                    res.y = loc.pos.y - 1;
                    break;
                case EAST:
                    res.x = loc.pos.x + 1;
                    break;
                case SOUTH:
                    res.y = loc.pos.y + 1;
                    break;
                case WEST:
                    res.x = loc.pos.x - 1;
                    break;
                default:
                    break;
            }
            break;
        case F_20:
            switch (loc.ori) {
                case NORTH:
                    res.y = loc.pos.y - 2;
                    break;
                case EAST:
                    res.x = loc.pos.x + 2;
                    break;
                case SOUTH:
                    res.y = loc.pos.y + 2;
                    break;
                case WEST:
                    res.x = loc.pos.x - 2;
                    break;
                default:
                    break;
            }
            break;
        case F_30:
            switch (loc.ori) {
                case NORTH:
                    res.y = loc.pos.y - 3;
                    break;
                case EAST:
                    res.x = loc.pos.x + 3;
                    break;
                case SOUTH:
                    res.y = loc.pos.y + 3;
                    break;
                case WEST:
                    res.x = loc.pos.x - 3;
                    break;
                default:
                    break;
            }
            break;
        case B_10:
            switch (loc.ori) {
                case NORTH:
                    res.y = loc.pos.y + 1;
                    break;
                case EAST:
                    res.x = loc.pos.x - 1;
                    break;
                case SOUTH:
                    res.y = loc.pos.y - 1;
                    break;
                case WEST:
                    res.x = loc.pos.x + 1;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return loc_init(res.x, res.y, loc.ori);

}

/* definitions of exported functions */

char *getMoveAsString(t_move move)
{
    return _moves[move];
}

t_localisation move(t_localisation loc, t_move move)
{
    t_localisation new_loc;
    new_loc.ori = rotate(loc.ori, move);
    new_loc = translate(loc, move);
    return new_loc;
}

void updateLocalisation(t_localisation *p_loc, t_move m)
{
    *p_loc = move(*p_loc, m);
    return;
}







void adjust_move_for_terrain(t_map *map, t_position *pos, t_move *move) {
    t_soil current_terrain = map->soils[pos->y][pos->x];

    switch (current_terrain) {
        case CREVASSE:
            *move = U_TURN; // Demi-tour obligatoire
            break;

        case ERG:
            // Si le terrain est sableux (ERG), limiter les déplacements
            if (*move == F_30) {
                *move = F_10; // Réduire le mouvement
            }
            break;

        case REG:
            // Régolithe (REG) : pas d'ajustement pour le moment
            break;

        default:
            break;
    }
}

#include <stdio.h>
#include "types.h" // Assurez-vous que ce fichier contient les définitions des types t_map, t_position, t_orientation, t_move, etc.

int apply_move(t_map *map, t_position *pos, t_orientation *ori, t_move *move, int *remaining_moves) {
    // Vérifier s'il reste des mouvements
    if (*remaining_moves <= 0) {
        printf("Erreur : Aucun mouvement restant.\n");
        return 0;
    }

    // Gestion des types de sol
    t_soil current_soil = map->soils[pos->y][pos->x];

    // Réduction des mouvements restants
    (*remaining_moves)--;

    // Appliquer le mouvement
    switch (*move) {
        case F_10:
            if (current_soil == ERG) {

                return 1; // Mouvement valide mais pas de déplacement
            }
            switch (*ori) {
                case NORTH: pos->y--; break;
                case EAST:  pos->x++; break;
                case SOUTH: pos->y++; break;
                case WEST:  pos->x--; break;

            }
            break;

        case F_20:
            if (current_soil == ERG) {

                switch (*ori) {
                    case NORTH: pos->y--; break;
                    case EAST:  pos->x++; break;
                    case SOUTH: pos->y++; break;
                    case WEST:  pos->x--; break;

                }
            } else {
                switch (*ori) {
                    case NORTH: pos->y -= 2; break;
                    case EAST:  pos->x += 2; break;
                    case SOUTH: pos->y += 2; break;
                    case WEST:  pos->x -= 2; break;

                }
            }
            break;

        case T_LEFT:
            *ori = (*ori + 3) % 4; // Tourne à gauche
            break;

        case T_RIGHT:
            *ori = (*ori + 1) % 4; // Tourne à droite
            break;

        case U_TURN:
            *ori = (*ori + 2) % 4; // Demi-tour
            break;

        default:

            return 0; // Mouvement invalide
    }

    // Vérifications après le mouvement
    if (!isValidLocalisation(*pos, map->x_max, map->y_max)) {
        //printf("Le robot est sorti de la carte! Fin de la phase. \n");
        return 0; // Mouvement invalide
    }

    if (map->soils[pos->y][pos->x] == CREVASSE) {

        return 0; // Mouvement invalide
    }

    if (map->soils[pos->y][pos->x] == REG) {

        *remaining_moves = 4; // Limiter les mouvements
    }

    return 1; // Mouvement valide
}






const char *moveToString(t_move move) {
    switch (move) {
        case F_10:
            return "Avancer de 10 unites";
        case F_20:
            return "Avancer de 20 unites";
        case F_30:
            return "Avancer de 30 unites";
        case B_10:
            return "Reculer de 10 unites";
        case T_LEFT:
            return "Tourner a gauche";
        case T_RIGHT:
            return "Tourner a droite";
        case U_TURN:
            return "Faire un demi-tour";
        default:
            return "Mouvement inconnu";
    }
}
