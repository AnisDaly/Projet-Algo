#ifndef TREE_H
#define TREE_H

#include "types.h"
#include "map.h"

typedef struct s_node {
    t_position pos;               // Position actuelle
    t_orientation ori;            // Orientation actuelle
    t_move move;                  // Mouvement associé à ce nœud
    int cost;                     // Coût total pour atteindre ce nœud
    struct s_node *children[9];   // Pointeurs vers les nœuds enfants (9 mouvements possibles max)
} t_node;

// Fonctions de gestion de l'arbre
t_node *createNode(t_position pos, t_orientation ori, t_move move, int cost);
int addChild(t_node *parent, t_node *child);
void freeTree(t_node *root);

// Fonctions de construction et exploration de l'arbre
t_node *buildTree(t_map *map, t_position start_pos, t_orientation start_ori, int depth, t_move *phase_moves);
void exploreTree(t_node *root, t_node **best_leaf, int *min_cost, int depth);
void findAndPrintOptimalPath(t_node *root, t_map *map);
void printOptimalMoves(t_move *optimal_moves, int size);
void printOptimalPath(t_move *moves, t_position *positions, int *costs, int max_depth);
void retrievePath(t_node *root, t_node *leaf, t_move *moves, t_position *positions, int *costs, int max_depth);
void generatePhaseMoves(t_move *phase_moves, int *availability);
void resetMovementAvailability(int *availability);
void findOptimalPhase(t_map *map, t_position start_pos, t_orientation start_ori, t_move *phase_moves);


#endif // TREE_H
