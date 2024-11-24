#include <stdlib.h>
#include <stdio.h>
#include "tree.h"
#include "types.h"
#include "map.h"
#include "moves.h"
#include <limits.h>
#include "string.h"

const char* orientationToString(t_orientation ori) {
    switch (ori) {
        case NORTH: return "North";
        case EAST: return "East";
        case SOUTH: return "South";
        case WEST: return "West";
        default: return "Unknown";
    }
}

#define MAX_CHILDREN 9
// Crée un nœud dans l'arbre
t_node *createNode(t_position pos, t_orientation ori, t_move move, int cost) {
    t_node *node = malloc(sizeof(t_node));
    if (!node) {
        perror("Failed to allocate memory for node");
        exit(EXIT_FAILURE);
    }
    node->pos = pos;
    node->ori = ori;
    node->move = move;
    node->cost = cost;
    for (int i = 0; i < 9; i++) { // Initialiser 9 enfants à NULL
        node->children[i] = NULL;
    }
    return node;
}

// Ajoute un enfant à un nœud
int addChild(t_node *parent, t_node *child) {
    int childCount = 0;
    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (parent->children[i]) {
            // si l'enfant existe déjà dans les enfants du parent
            if(parent->children[i] == child) {
                printf("Erreur: tentative d'ajouter un enfant déjà ajouté.\n");
                return 0;
            }
            childCount++;
        }
    }

    if(childCount < MAX_CHILDREN) {
        parent->children[childCount] = child;  // Ajoute l'enfant à la première position disponible
        return 1;
    }

    //fprintf(stderr, "Error: Unable to add child. Node at position (%d, %d) already has %d children.\n",
      //      parent->pos.x, parent->pos.y, MAX_CHILDREN);
    return 0;
}





// Libère la mémoire allouée pour l'arbre
void freeTree(t_node *root) {
    if (!root) return;
    for (int i = 0; i < 9; i++) {
        freeTree(root->children[i]);
    }
    free(root);
}


// Explore l'arbre pour trouver la phase avec le coût minimal
void exploreTree(t_node *root, t_node **best_leaf, int *min_cost, int depth) {
    if (!root) return;

    // Condition d'arrêt : si c'est une feuille
    int is_leaf = 1;
    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (root->children[i] != NULL) {
            is_leaf = 0;
            break;
        }
    }

    if (is_leaf) {
        if (root->cost < *min_cost) {
            *min_cost = root->cost;
            *best_leaf = root; // Met à jour la meilleure feuille trouvée
        }
        return;
    }

    // Explore les enfants
    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (root->children[i]) {
            exploreTree(root->children[i], best_leaf, min_cost, depth + 1);
        }
    }
}

t_node *findParent(t_node *root, t_node *child) {
    if (!root || !child) return NULL;

    // Vérifie si l'un des enfants est le nœud recherché
    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (root->children[i] == child) {
            return root; // Parent trouvé
        }
    }

    // Parcourt récursivement les enfants
    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (root->children[i]) {
            t_node *parent = findParent(root->children[i], child);
            if (parent) return parent;
        }
    }

    return NULL; // Aucun parent trouvé
}


void retrievePath(t_node *root, t_node *leaf, t_move *moves, t_position *positions, int *costs, int max_depth) {
    if (!root || !leaf) {
        printf("Erreur : racine ou feuille invalide\n");
        return;
    }

    // Temporaire pour stocker les nœuds du chemin
    t_node *path[max_depth];
    int depth = 0;

    // Remonte l'arbre depuis la feuille jusqu'à la racine
    t_node *current = leaf;
    while (current && depth < max_depth) {
        path[depth++] = current;
        current = findParent(root, current); // Fonction qui trouve le parent
    }

    // Remplit les mouvements, positions et coûts à partir de 'path'
    int index = 0;
    for (int i = depth - 1; i >= 0; i--) {
        moves[index] = path[i]->move;
        positions[index] = path[i]->pos;
        costs[index] = path[i]->cost;
        index++;
    }

    // Complète avec NO_MOVE si on n'a pas atteint max_depth
    for (; index < max_depth; index++) {
        moves[index] = NO_MOVE;
        positions[index].x = -1;
        positions[index].y = -1;
        costs[index] = INT_MAX;
    }
}



void printOptimalPath(t_move *moves, t_position *positions, int *costs, int max_depth) {
    printf("Chemin optimal trouve :\n");
    for (int i = 0; i < max_depth; i++) {
        printf("Mouvement %d : ", i + 1);
        switch (moves[i]) {
            case F_10: printf("Avancer de 10 unites\n"); break;
            case F_20: printf("Avancer de 20 unites\n"); break;
            case F_30: printf("Avancer de 30 unites\n"); break;
            case B_10: printf("Reculer de 10 unites\n"); break;
            case T_LEFT: printf("Tourner a gauche\n"); break;
            case T_RIGHT: printf("Tourner a droite\n"); break;
            case U_TURN: printf("Faire un demi-tour\n"); break;
            default: printf("Aucun mouvement\n"); break;
        }

        if (positions[i].x != -1 && positions[i].y != -1) {
            printf("Position apres mouvement : (%d, %d)\n", positions[i].x, positions[i].y);
            printf("Cout total a cette etape : %d\n", costs[i]);
        }
    }
}


void printOptimalMoves(t_move *optimal_moves, int size) {
    printf("Vérification des mouvements optimaux :\n");
    for (int i = 0; i < size; i++) {
        switch (optimal_moves[i]) {
            case F_10: printf("Mouvement %d : Avancer de 10 unités\n", i + 1); break;
            case F_20: printf("Mouvement %d : Avancer de 20 unités\n", i + 1); break;
            case F_30: printf("Mouvement %d : Avancer de 30 unités\n", i + 1); break;
            case B_10: printf("Mouvement %d : Reculer de 10 unités\n", i + 1); break;
            case T_LEFT: printf("Mouvement %d : Tourner à gauche\n", i + 1); break;
            case T_RIGHT: printf("Mouvement %d : Tourner à droite\n", i + 1); break;
            case U_TURN: printf("Mouvement %d : Faire un demi-tour\n", i + 1); break;
            case NO_MOVE: printf("Mouvement %d : Aucun mouvement\n", i + 1); break;
            default: printf("Mouvement %d : Inconnu\n", i + 1); break;
        }
    }
}

void generatePhaseMoves(t_move *phase_moves, int *availability) {
    static const char *move_names[] = {
            "Avancer de 10 unites",  // F_10
            "Avancer de 20 unites",  // F_20
            "Avancer de 30 unites",  // F_30
            "Reculer de 10 units",  // B_10
            "Tourner a gauche",      // T_LEFT
            "Tourner a droite",      // T_RIGHT
            "Faire un demi-tour"     // U_TURN
    };

    int count = 0; // Compteur pour s'assurer d'avoir exactement 9 mouvements

    while (count < 9) {
        int random_move = rand() % 7; // Tirer un mouvement aléatoire parmi les 7 possibles
        if (availability[random_move] > 0) {
            phase_moves[count] = (t_move)random_move;
            availability[random_move]--;
            printf("Mouvement %d genere : %s (reste %d)\n", count, move_names[random_move], availability[random_move]);
            count++; // Incrémenter après un mouvement valide
        }
    }
}

void resetMovementAvailability(int *availability) {
    int initial_availability[] = {22, 15, 7, 7, 21, 21, 7};
    for (int i = 0; i < 7; i++) {
        availability[i] = initial_availability[i];
    }
}



t_node *buildTree(t_map *map, t_position start_pos, t_orientation start_ori, int depth, t_move *phase_moves) {
    const int max_depth = 5; // Toujours explorer jusqu'à 5 niveaux
    if (depth == max_depth) return NULL; // Arrêter la construction au niveau max

    // Créez le nœud racine
    t_node *root = createNode(start_pos, start_ori, F_10, map->costs[start_pos.y][start_pos.x]); // F_10 est un mouvement par défaut ici
    if (root->cost == 0) return root; // Si on atteint la base station, retourne immédiatement

    int child_count = 0; // Compteur pour limiter les enfants

    // Boucle pour générer les nœuds enfants
    for (int i = 0; i < 9; i++) { // Toujours considérer les 9 mouvements générés
        if (child_count >= MAX_CHILDREN) break; // Ne pas dépasser la limite d'enfants

        t_position new_pos = start_pos;
        t_orientation new_ori = start_ori;

        // Applique le mouvement
        apply_move(&new_pos, &new_ori, phase_moves[i]);

        // Vérifie si la position est valide
        if (isValidLocalisation(new_pos, map->x_max, map->y_max)) {
            int cost = root->cost + map->costs[new_pos.y][new_pos.x];
            t_node *child = createNode(new_pos, new_ori, phase_moves[i], cost);

            // Ajout du nœud enfant au parent
            if (addChild(root, child)) {
                child_count++; // Incrémenter si l'ajout est réussi
            } else {
                free(child); // Libérer la mémoire si l'ajout échoue
            }

            // Construction récursive des sous-arbres
            t_node *child_tree = buildTree(map, new_pos, new_ori, depth + 1, phase_moves);
            if (child_tree) {
                if (addChild(root, child_tree)) {
                    child_count++; // Incrémenter si l'ajout est réussi
                } else {
                    free(child_tree); // Libérer la mémoire si l'ajout échoue
                }
            }
        }
    }

    return root;
}

int apply_move_simulation(t_map *map, t_position *pos, t_orientation *ori, t_move move) {
    t_position new_pos = *pos; // Copier la position actuelle
    t_orientation new_ori = *ori; // Copier l'orientation actuelle

    // Appliquer le mouvement
    switch (move) {
        case F_10:
            if (*ori == NORTH) new_pos.y--;
            else if (*ori == SOUTH) new_pos.y++;
            else if (*ori == EAST) new_pos.x++;
            else if (*ori == WEST) new_pos.x--;
            break;
        case B_10:
            if (*ori == NORTH) new_pos.y++;
            else if (*ori == SOUTH) new_pos.y--;
            else if (*ori == EAST) new_pos.x--;
            else if (*ori == WEST) new_pos.x++;
            break;
        case T_LEFT:
            new_ori = (new_ori + 3) % 4; // Tourner à gauche
            break;
        case T_RIGHT:
            new_ori = (new_ori + 1) % 4; // Tourner à droite
            break;
        case U_TURN:
            new_ori = (new_ori + 2) % 4; // Demi-tour
            break;
        default:
            return 0; // Mouvement invalide
    }

    // Vérifier si la nouvelle position est valide
    if (new_pos.x < 0 || new_pos.x >= map->x_max || new_pos.y < 0 || new_pos.y >= map->y_max) {
        return 0; // Mouvement invalide (hors de la carte)
    }

    // Mettre à jour la position et l'orientation
    *pos = new_pos;
    *ori = new_ori;

    return 1; // Mouvement valide
}
#define TOTAL_COMBINATIONS 126

void generateCombinations(int n, int k, int combinations[TOTAL_COMBINATIONS][5]) {
    int indices[5];
    int count = 0;

    // Générer les indices pour les combinaisons
    for (indices[0] = 0; indices[0] < n; indices[0]++) {
        for (indices[1] = indices[0] + 1; indices[1] < n; indices[1]++) {
            for (indices[2] = indices[1] + 1; indices[2] < n; indices[2]++) {
                for (indices[3] = indices[2] + 1; indices[3] < n; indices[3]++) {
                    for (indices[4] = indices[3] + 1; indices[4] < n; indices[4]++) {
                        memcpy(combinations[count], indices, sizeof(indices));
                        count++;
                    }
                }
            }
        }
    }
}

void findOptimalPhase(t_map *map, t_position start_pos, t_orientation start_ori, t_move *phase_moves) {
    t_position best_position = start_pos;
    t_orientation best_orientation = start_ori;
    int min_cost = INT_MAX;
    t_move best_phase[5];

    int combination_indices[TOTAL_COMBINATIONS][5]; // Tableau 2D pour les combinaisons
    generateCombinations(9, 5, combination_indices); // Générer toutes les combinaisons

    for (int c = 0; c < TOTAL_COMBINATIONS; c++) {
        t_position current_pos = start_pos;
        t_orientation current_ori = start_ori;
        t_move current_phase[5];

        // Charger la combinaison courante
        for (int i = 0; i < 5; i++) {
            current_phase[i] = phase_moves[combination_indices[c][i]];
        }

        // Simuler la phase
        int valid = 1;
        for (int i = 0; i < 5; i++) {
            if (!apply_move_simulation(map, &current_pos, &current_ori, current_phase[i])) {
                valid = 0; // Mouvement invalide
                break;
            }

            // Si on atteint la base, c'est gagné
            if (map->costs[current_pos.y][current_pos.x] == 0) {
                printf("C'est gagne ! Phase optimale trouvee :\n");
                for (int j = 0; j <= i; j++) {
                    printf("Mouvement %d : %s\n", j + 1, moveToString(current_phase[j]));
                }
                printf("Position finale : (%d, %d) | Orientation finale : %s | Cout total : 0\n",
                       current_pos.x, current_pos.y, orientationToString(current_ori));
                return;
            }
        }

        // Vérifier si la phase est valide et le coût minimal
        if (valid) {
            int final_cost = map->costs[current_pos.y][current_pos.x];
            if (final_cost < min_cost) {
                min_cost = final_cost;
                best_position = current_pos;
                best_orientation = current_ori;
                memcpy(best_phase, current_phase, sizeof(best_phase));
            }
        }
    }

    // Afficher la phase optimale
    printf("Phase optimale trouvee :\n");
    for (int i = 0; i < 5; i++) {
        printf("Mouvement %d : %s\n", i + 1, moveToString(best_phase[i]));
    }
    printf("Position finale : (%d, %d) | Orientation finale : %s | Cout total : %d\n",
           best_position.x, best_position.y, orientationToString(best_orientation), min_cost);
}











