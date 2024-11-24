#ifndef TYPES_H
#define TYPES_H

typedef struct {
    int x;
    int y;
} t_position;

typedef enum {
    NORTH = 0,
    EAST,
    SOUTH,
    WEST
} t_orientation;

typedef struct {
    t_position pos;
    t_orientation ori;
} t_localisation;

typedef enum e_soil
{
    BASE_STATION,
    PLAIN,
    ERG,
    REG,
    CREVASSE
} t_soil;

typedef enum {
    F_10 = 0,
    F_20,
    F_30,
    B_10,
    T_LEFT,
    T_RIGHT,
    U_TURN,
    NO_MOVE,
} t_move;





#endif // TYPES_H
