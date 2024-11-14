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

typedef enum e_soil {
    BASE_STATION,
    PLAIN,
    ERG,
    REG,
    CREVASSE,
    SLOPE
} t_soil;

typedef enum {
    F_10 = 0,
    F_20,
    F_30,
    B_10,
    T_LEFT,
    T_RIGHT,
    U_TURN,
    NO_MOVE
} t_move;

typedef struct {
    t_soil **soils; // Matrice des types de terrain
    int **costs;    // Matrice des coûts
    int x_max;      // Largeur de la carte
    int y_max;      // Hauteur de la carte
} t_map;

typedef struct s_node {
    t_position pos;               // Position actuelle
    t_orientation ori;            // Orientation actuelle
    t_move move;                  // Mouvement associé à ce nœud
    int cost;                     // Coût total pour atteindre ce nœud
    struct s_node *children[5];   // Pointeurs vers les nœuds enfants (5 mouvements possibles max)
} t_node;


#endif // TYPES_H
