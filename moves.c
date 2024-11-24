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

void apply_move(t_position *pos, t_orientation *ori, t_move move) {
    switch (move) {
        case F_10:
        case F_20:
        case F_30: {
            int step = (move == F_10) ? 1 : (move == F_20) ? 2 : 3;
            switch (*ori) {
                case NORTH: pos->y -= step; break; // Y diminue vers le haut
                case EAST:  pos->x += step; break; // X augmente vers la droite
                case SOUTH: pos->y += step; break; // Y augmente vers le bas
                case WEST:  pos->x -= step; break; // X diminue vers la gauche
            }
            break;
        }
        case B_10: {
            int step = 1; // Marche arrière de 10 unités
            switch (*ori) {
                case NORTH: pos->y += step; break;
                case EAST:  pos->x -= step; break;
                case SOUTH: pos->y -= step; break;
                case WEST:  pos->x += step; break;
            }
            break;
        }
        case T_LEFT:
            *ori = (*ori + 3) % 4; // Cycle : 0 -> 3 -> 2 -> 1 -> 0
            break;
        case T_RIGHT:
            *ori = (*ori + 1) % 4; // Cycle : 0 -> 1 -> 2 -> 3 -> 0
            break;
        case U_TURN:
            *ori = (*ori + 2) % 4; // Cycle : 0 -> 2 et 1 -> 3
            break;
        default:
            break; // Pas de mouvement
    }
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
