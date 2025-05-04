#include "../head/GameState.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

GameState state = {.map = NULL, .size = 0};
char lettres[] = {'?', '^', 'v', 'A', 'B', 'C', 'D', 'E', 'F', 'G'};

void groupe7_create_empty_game_state(GameState* state, int size) {
    state->map = (Color*)malloc(size * size * sizeof(Color));
    if (state->map == NULL) {
        printf("[ERROR] Allocation échouée\n");
        return;
    }
    state->size = size;
}

void groupe7_free_game(GameState* state) {
    if (state->map != NULL) {
        free(state->map);
        state->map = NULL;
    }
}

void groupe7_set_map_value(GameState* state, int x, int y, Color value) {
    if (x < 0 || x >= state->size || y < 0 || y >= state->size) {
        printf("[ERROR] Coordonnée hors de la carte\n");
        return;
    }
    state->map[y * state->size + x] = value;
}

Color groupe7_get_map_value(GameState* state, int x, int y) {
    if (state->map == NULL || x >= state->size || y >= state->size || x < 0 || y < 0) {
        printf("[ERROR] map not big enough or not initialized %p %i access (%i %i)\n", state->map, state->size, x, y);
        return ERROR;
    }
    return state->map[y * state->size + x];
}

void groupe7_fill_map(GameState* map) {
    if (map->map == NULL) {
        printf("[ERROR] Carte non initialisée\n");
        return;
    }
    int size = map->size;
    for (int i = 0; i < size; i++) {
        for (int k = 0; k < size; k++) {
            int valeur_aleatoire = 3 + rand() % 7; // entier entre 3 et 9 inclus
            map->map[i * map->size + k] = (Color)valeur_aleatoire;
        }
    }
    map->map[(size - 1) * map->size + 0] = (Color)1; // Coin bas gauche ('^')
    map->map[0 * map->size + (size - 1)] = (Color)2; // Coin haut droit ('v')
}

GameState* groupe7_copie_map(GameState* map) {
    int size = map->size;
    GameState* copie = (GameState*)malloc(sizeof(GameState));
    copie->size = map->size;
    copie->map = (Color*)malloc(copie->size * copie->size * sizeof(Color));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            copie->map[i * copie->size + j] = map->map[i * map->size + j];
        }
    }
    return copie;
}

bool groupe7_estunecasevoisine(GameState*map, int i, int k, Color car) {
    int size = map->size;
    if (i==size -1 && k!= 0 && k!= size-1){
        return(map -> map[i*size+k+1]== car || map -> map[i*size+k-1]==car|| map -> map[(i-1)*size+k]==car);
    }
    else if (i ==0 && k!= 0 && k!= size-1){
        return(map -> map[i*size+k+1]== car || map ->map[i*size+k-1]==car||map ->map[(i+1)*size+k]==car);
    }
    else if (k==0 && i != size-1 && i!= 0){
        return(map ->map[(i+1)*size+k]== car || map ->map[(i-1)*size+k]==car||map ->map[(i)*size+k+1]==car);
    }
    else if (k==size -1 && i!= 0 && i!= size-1){
        return(map ->map[(i+1)*size+k]== car || map ->map[(i-1)*size+k]==car||map ->map[(i)*size+k-1]==car);
    }
    else if (i==size-1 && k==0){
        return(map ->map[(i-1)*size+k]==car||map ->map[(i)*size+k+1]==car);
    }
    else if (i==size-1 && k==size-1){
        return(map ->map[(i-1)*size+k]==car||map ->map[(i)*size+k-1]==car);
    }
    else if (i==0 && k==0){
        return(map ->map[(i+1)*size+k]==car||map ->map[(i)*size+k+1]==car);
    }
    else if (i==0 && k==size -1){
        return(map ->map[(i+1)*size+k]==car||map ->map[(i)*size+k-1]==car);
    }
    else{
        return(map ->map[i*size+k+1]== car || map ->map[i*size+k-1]==car|| map ->map[(i-1)*size+k]==car || map ->map[(i+1)*size +k]== car);
    }
}

void groupe7_miseajour_map(GameState* map,Color car, Color car2) {
    int compteur = 1;
    int size = map->size;
    while (compteur!=0){
        compteur =0;
        for (int i =0; i < size ; i++){
            for (int k=0;k<size;k++){
                if(map->map[i*size +k]==car2 && groupe7_estunecasevoisine(map,i,k,car)==true){
                    map->map[i*size+k]=car;
                    compteur+=1;
                }
            }
        }
    }
}

int groupe7_compte_nombrecasesvoisines(GameState*map, Color joueur) {
    int size = map-> size;
    int nb_voisins = 0;
    GameState* copie = groupe7_copie_map(map);
    for (int i=0; i<size; i++){
        for (int k = 0; k<size; k++){
            copie-> map[i*size+k]=0;
        }
    }
    for (int i=0; i<size;i++){
        for (int k=0; k<size; k++){
            if (groupe7_estunecasevoisine(map, i, k, joueur)== true ){
                if (copie->map[i*size +k]==0){
                    nb_voisins+=1;
                    copie-> map[i*size +k]=1;
                }
            }
        }
    }
    groupe7_free_game(copie);
    free(copie);
    return nb_voisins;
}

int groupe7_compte_frontieres(Color*territoire) {
    int somme = 0;
    for (int i=0; i<7; i++){
        somme += territoire[i];
    }
    return(somme);
}

bool groupe7_verifie(GameState*map) {
    int size = map->size;
    int compteur1=0;
    int compteur2=0;
    for (int i =0; i< size ; i++){
        for (int k=0; k<size;k++){
            if (map->map[i*size +k]==(Color)1){
                compteur1+=1;
            }
            else if (map->map[i*size +k]==(Color)2){
                compteur2+=1;
            }
        }
    }
    int moitie =size*size /2;
    return(compteur1>= moitie || compteur2>= moitie);
}

int groupe7_compte_territoire(GameState* map, Color joueur) {
    int size = map->size;
    int compteur =0;
    for (int i = 0 ; i<size; i++){
        for (int k = 0; k<size ; k++){
            if (map->map[i*size +k]==joueur){
                compteur +=1;
            }
        }
    }
    return compteur;
}

Color groupe7_glouton(GameState* map, Color joueur) {
    int liste[7]={0,0,0,0,0,0,0};
    for (int i = 3; i < 10; i++) {
        GameState* copie = groupe7_copie_map(map);
        groupe7_miseajour_map(copie, joueur,(Color)i);
        liste[i-3]=groupe7_compte_territoire(copie,joueur);
        groupe7_free_game(copie);
        free(copie);
    }
    int max = liste[0];
    int index = 3;
    for (int p=0; p<7; p++){
        if (liste[p]>max){
            max = liste[p];
            index = p+3;
        }
    }
    return (Color)index;
}

Color groupe7_hegemonique(GameState* map, Color joueur) {
    int liste[7]={0,0,0,0,0,0,0};
    for (int i = 3; i < 10; i++) {
        GameState* copie = groupe7_copie_map(map);
        groupe7_miseajour_map(copie, joueur,(Color)i);
        liste[i-3]= groupe7_compte_nombrecasesvoisines(copie, joueur);
        groupe7_free_game(copie);
        free(copie);
    }
    int max = -1;
    int index = -1;
    for (int p=0; p<7; p++){
        if (liste[p]>max){
            max = liste[p];
            index = p+3;
        }
    }
    return (Color)index;
}

Color groupe7_mixte(GameState* map, Color joueur) {
    int liste_frontieres[7] = {0, 0, 0, 0, 0, 0, 0};
    int liste_glouton[7] = {0, 0, 0, 0, 0, 0, 0};
    int max_frontieres = -1;
    int max_glouton = -1;
    int index_frontieres = -1;
    int index_glouton = -1;
    for (int i = 3; i < 10; i++) {
        GameState* copie = groupe7_copie_map(map);
        groupe7_miseajour_map(copie, joueur, (Color)i);
        liste_frontieres[i-3] = groupe7_compte_nombrecasesvoisines(copie, joueur);
        liste_glouton[i - 3] = groupe7_compte_territoire(copie, joueur);
        groupe7_free_game(copie);
        free(copie);
    }
    for (int p = 0; p < 7; p++) {
        if (liste_frontieres[p] > max_frontieres) {
            max_frontieres = liste_frontieres[p];
            index_frontieres = p + 3;
        }
    }
    for (int p = 0; p < 7; p++) {
        if (liste_glouton[p] > max_glouton) {
            max_glouton = liste_glouton[p];
            index_glouton = p + 3;
        }
    }
    GameState*copie2 = groupe7_copie_map(map);
    groupe7_miseajour_map(copie2, (Color)index_frontieres,joueur);
    if (groupe7_compte_territoire(copie2, joueur)==groupe7_compte_territoire(map, joueur)){
        groupe7_free_game(copie2);
        free(copie2);
        return((Color)index_glouton);
    }
    else{
        groupe7_free_game(copie2);
        free(copie2);
        return (Color)index_frontieres;
    }
}

Color groupe7_lettre_couleur(char letter) {
    switch(letter) {
        case 'A': return (Color)3;  // Rouge
        case 'B': return (Color)4;  // Bleu
        case 'C': return (Color)5;  // Vert
        case 'D': return (Color)6;  // Jaune
        case 'E': return (Color)7;  // Magenta
        case 'F': return (Color)8;  // Cyan
        case 'G': return (Color)9;  // Blanc
        case '^': return (Color)1;  // Blanc
        case 'v': return (Color)2;  // Noir
        default: return (Color)-1;  // Erreur
    }
}

const char* groupe7_couleurverslettre(Color couleur) {
    switch (couleur) {
        case RED: return "A";       // Rouge
        case GREEN: return "B";     // Bleu
        case BLUE: return "C";      // Vert
        case YELLOW: return "D";    // Jaune
        case MAGENTA: return "E";   // Magenta
        case CYAN: return "F";      // Cyan
        case WHITE: return "G";     // Blanc
        default: return "UNKNOWN_COLOR"; // Erreur ou valeur invalide
    }
}

void groupe7_print_map(GameState* state) {
    if (state->map == NULL) {
        printf("[ERROR] Carte non initialisée\n");
        return;
    }

    const char* colors[] = {
        "\x1B[37m", // Par défaut ('?')
        "\x1B[37m", // Blanc pour '^'
        "\x1B[30m", // Noir pour 'v'
        "\x1B[31m", // Rouge pour 'A'
        "\x1B[34m", // Bleu pour 'B'
        "\x1B[32m", // Vert pour 'C'
        "\x1B[33m", // Jaune pour 'D'
        "\x1B[35m", // Magenta pour 'E'
        "\x1B[36m", // Cyan pour 'F'
        "\x1B[37m"  // Blanc pour 'G'
    };

    printf("Carte generee (%dx%d):\n", state->size, state->size);

    for (int i = 0; i < state->size; i++) {
        for (int j = 0; j < state->size; j++) {
            Color color = groupe7_get_map_value(state, j, i);
            if (color >= 0 && color <= 9) { 
                printf("%s%c\x1B[0m ", colors[color], lettres[color]);
            } else {
                printf("?");
            }
        }
        printf("\n");
    }
}

typedef struct {
    int x, y;
    Color symbol;
} Player;

void groupe7_init_players(GameState* state, Player* player1, Player* player2) {
    int size = state->size;

    player1->x = 0;
    player1->y = size - 1;
    player1->symbol = (Color)1;

    player2->x = size - 1;
    player2->y = 0;
    player2->symbol = (Color)2;

    groupe7_set_map_value(state, player1->x, player1->y, player1->symbol);
    groupe7_set_map_value(state, player2->x, player2->y, player2->symbol);
}

Color groupe7_aleatoire() {
    int valeur_aleatoire =3+ rand() % 7;
    return((Color)valeur_aleatoire);
}

Color groupe7_aleatoireintelligent(GameState* map, Color joueur) {
    int size = map->size;
    bool couleurs_possibles[7] = {false, false, false, false, false, false, false};
    for (int i = 0; i < size; i++) {
        for (int k = 0; k < size; k++) {
            if (groupe7_estunecasevoisine(map, i, k, joueur)==true) {
                Color c = map->map[i * size + k];
                if (c >= 3 && c <= 9) {
                    couleurs_possibles[c - 3] = true;
                }
            }
        }
    }
    int compteur = 0; 
    for (int i =0; i<7; i++){
        if(couleurs_possibles[i]==true){
            compteur = 1;
        }
    }
    if (compteur==0){
        int valeur_aleatoire = 3 + rand()%7;
        return (Color)valeur_aleatoire;
    }
    int valeuraleatoire = 3 +rand()%7;
    while (couleurs_possibles[valeuraleatoire-3]==false){
        valeuraleatoire = 3+ rand()%7;
    }
    return((Color)valeuraleatoire);
}

int main(int argc, char** argv) {
    srand(time(NULL));
    int hegemonique_victoires = 0; 
    int glouton_victoires = 0;
    int size = atoi(argv[1]);
    for (int partie = 1; partie <= 500; partie++) {
        GameState game;
        Player player1;
        Player player2;
        groupe7_create_empty_game_state(&game, size);
        groupe7_fill_map(&game);
        groupe7_init_players(&game, &player1, &player2);
        int tour = 0;
        while (groupe7_verifie(&game) == false) {
            tour++;
            if (tour > 500) {
                printf("Erreur : trop de tours (%d). État actuel :\n", tour);
                groupe7_print_map(&game);
                break;
            }
            if (partie % 2 == 0) {
                Color couleur_choisie = groupe7_hegemonique(&game, (Color)1);
                groupe7_miseajour_map(&game, (Color)1, couleur_choisie);
                if (groupe7_verifie(&game)) {
                    hegemonique_victoires++;
                    break;
                }
                Color couleur_choisie2 = groupe7_glouton(&game, (Color)2);
                groupe7_miseajour_map(&game, (Color)2, couleur_choisie2);
                if (groupe7_verifie(&game)) {
                    glouton_victoires++;
                    break;
                }
            } else {
                Color couleur_choisie = groupe7_glouton(&game, (Color)2);
                groupe7_miseajour_map(&game, (Color)2, couleur_choisie);
                if (groupe7_verifie(&game)) {
                    glouton_victoires++;
                    break;
                }
                Color couleur_choisie2 =  groupe7_hegemonique(&game, (Color)1);
                groupe7_miseajour_map(&game, (Color)1, couleur_choisie2);
                if (groupe7_verifie(&game)) {
                    hegemonique_victoires++;
                    break;
                }
            }
        }
        groupe7_free_game(&game);
    }
    printf("Résultats après 500 parties :\n");
    printf("IA hegemonique a gagné %d parties.\n", hegemonique_victoires);
    printf("IA glouton a gagné %d parties.\n", glouton_victoires);
    return 0;
}
