README - Projet de Simulation d'Exploration de Robot
Description
Ce projet implémente une simulation de robot explorateur sur une carte à base de cases. Le robot effectue des mouvements et prend des décisions en fonction du terrain pour atteindre une station de base tout en optimisant ses déplacements. Il est capable de gérer différents types de terrain, des obstacles, et des coûts associés à chaque déplacement.

Le projet est écrit en C et inclut plusieurs fonctionnalités comme la gestion d'un arbre de décisions, des phases de simulation, et des algorithmes pour explorer des chemins optimaux.

Fonctionnalités principales
Génération aléatoire :

Position initiale du robot
Orientation initiale du robot
Sélection des mouvements disponibles pour chaque phase
Types de terrains :

Station de base (BASE_STATION)
Plaine (PLAIN)
Erg (ERG)
Régolithe (REG)
Crevasse (CREVASSE)
Calcul des coûts :

Les coûts des cases sont calculés à partir de la station de base.
Les coûts tiennent compte des obstacles et des terrains difficiles (ergs, régolithes).
Gestion des mouvements :

Le robot peut se déplacer dans différentes directions, tourner, ou effectuer un demi-tour.
Les mouvements disponibles sont ajustés en fonction du terrain rencontré.
Optimisation :

Recherche du chemin optimal pour atteindre la station de base avec un coût minimal.
Simulation et évaluation de combinaisons de mouvements pour trouver la meilleure séquence.
Structure du projet
Fichiers principaux
main.c :

Point d'entrée du programme.
Gère la boucle principale de simulation.
Affiche les résultats de chaque phase.
map.c :

Gestion de la carte (chargement depuis un fichier, affichage).
Calcul des coûts des cases.
Gestion des types de terrain.
loc.c :

Gestion des localisations et des orientations du robot.
Fonctions pour vérifier la validité des positions.
moves.c :

Gestion des mouvements du robot.
Ajustement des mouvements en fonction du terrain.
Simulation des déplacements.
tree.c :

Gestion d'un arbre de décisions.
Exploration des branches pour trouver les chemins optimaux.
Construction récursive de l'arbre.
queue.c :

Implémentation d'une file pour les calculs de coûts.
stack.c :

Implémentation d'une pile pour la gestion des chemins.
types.h :

Déclarations des structures principales (t_map, t_position, t_move, etc.).

Auteurs
Anis Daly
Nathan Favry
Clément Pinto Ribeiro
