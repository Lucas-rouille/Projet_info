#include "../head/GameState.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
 
GameState state = {.map = NULL, .size = 0};
char lettres[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', '^', 'v'};
const int size = 16;
 
void create_empty_game_state(GameState* state, int size) {
    state->map = (Color*)malloc(size * size * sizeof(Color));
    if (state->map == NULL) {
        printf("[ERROR] Allocation échouée\n");
        return;
    }
    state->size = size;
}
 
void free_game(GameState* state) {
    if (state->map != NULL) {
        free(state->map);
        state->map = NULL;
    }
}
 
void set_map_value(GameState* state, int x, int y, Color value) {
    if (x < 0 || x >= state->size || y < 0 || y >= state->size) {
        printf("[ERROR] Coordonnée hors de la carte\n");
        return;
    }
    state->map[y * state->size + x] = value;
}
 
Color get_map_value(GameState* state, int x, int y) {
    if (state->map == NULL || x >= state->size || y >= state->size || x < 0 || y < 0) {
        printf("[ERROR] map not big enough or not initialized %p %i access (%i %i)\n", state->map, state->size, x, y);
        return ERROR;
    }
    return state->map[y * state->size + x];
}
 
void fill_map(GameState* map) {
    if (map->map == NULL) {
        printf("[ERROR] Carte non initialisée\n");
        return;
    }
 
    for (int i = 0; i < size; i++) {
        for (int k = 0; k < size; k++) {
            int valeur_aleatoire = rand() % 7;
            map->map[i * map->size + k] = (Color)valeur_aleatoire;
        }
    }
 
    map->map[0 * map->size + (size - 1)] = (Color)7;
    map->map[map->size * (size - 1)] = (Color)8;
}
 
bool estunecasevoisine(GameState*map, int i, int k, Color car){//fonction qui retourne true si la case de coordonnée i,k possède un voisin de valeur car
    if (i==size -1 && k!= 0 && k!= size-1){// si on se trouve sur la dernière ligne (coins exclus)
        return(map -> map[i*size+k+1]== car || map -> map[i*size+k-1]==car|| map -> map[(i-1)*size+k]==car);
    }
    else if (i ==0){// première ligne
        return(map -> map[i*size+k+1]== car || map ->map[i*size+k-1]==car||map ->map[(i+1)*size+k]==car);
    }
    else if (k==0){// première colonne
        return(map ->map[(i+1)*size+k]== car || map ->map[(i-1)*size+k]==car||map ->map[(i)*size+k+1]==car);
    }
    else if (k==size -1){// dernière colonne
        return(map ->map[(i+1)*size+k]== car || map ->map[(i-1)*size+k]==car||map ->map[(i)*size+k-1]==car);
    }
    else if (i==size-1 && k==0){// coin bas gauche
        return(map ->map[(i-1)*size+k]==car||map ->map[(i)*size+k+1]==car);
    }
    else if (i==size-1 && k==size){//coin bas droite
        return(map ->map[(i-1)*size+k]==car||map ->map[(i)*size+k-1]==car);
    }
    else if (i==0 && k==0){//coin haut gauche
        return(map ->map[(i+1)*size+k]==car||map ->map[(i)*size+k+1]==car);
    }
    else if (i==0 && k==size -1){// coin haut droite
        return(map ->map[(i+1)*size+k]==car||map ->map[(i)*size+k-1]==car);
    }
    else{//partout aillleurs
        return(map ->map[i*size+k+1]== car || map ->map[i*size+k-1]==car|| map ->map[(i-1)*size+k]==car || map ->map[(i+1)*size +k]== car);
    }
}
 
void miseajour_map(GameState* map,Color car, Color car2){// fonction qui met à jour la map de sorte que toutes les case de valeur car2 voisines de car sont remplacées en car
    int compteur = 1;
    while (compteur!=0){
        compteur =0;
        for (int i =0; i < size ; i++){
            for (int k=0;k<size;k++){
                if(map->map[i*size +k]==car2 && estunecasevoisine(map,i,k,car)==true){
                    map->map[i*size+k]=car;
                    compteur+=1;
                }
            }
    }
    }
}


Color* cases_disponibles(GameState* map, Color joueur){//fonction qui renvoie un tableau des couleurs disponibles et leur nombre d'apparition
    Color* territoire =(Color*) malloc(7 * sizeof(Color));
    for (int i=0; i<7; i++){
        territoire[i]=0;
    }
    for (int i=0; i< size; i++){
        for (int k=0; k<size; k++){
            if (estunecasevoisine(map, i, k, joueur)==true){
                Color lettre = map->map[i*size+k];
                if (lettre >= 0 && lettre < 7) {// on evite les symboles des deux joueurs
                    territoire[lettre] += 1;  
                }
            }
        }
    }
    return (territoire);
}
 
bool verifie(GameState*map){//fonction qui renvoie true si la partie est finie (un joueur possède plus de 50% de la map)
    int compteur1=0;
    int compteur2=0;
    for (int i =0; i< size ; i++){
        for (int k=0; k<size;k++){
            if (map->map[i*size +k]==(Color)7){
                compteur1+=1;
            }
            else if (map->map[i*size +k]==(Color)8){
                compteur2+=1;
            }
        }
    }
    int moitie =size*size /2;
    if (compteur1>= moitie){
        printf("Le joueur 1 a gagné la partie");
    }
    else if (compteur2>= moitie){
        printf("Le joueur 2 a gagné la partie");
    }
    return(compteur1>= moitie || compteur2>= moitie);
}
 
Color lettre_couleur(char letter) {
    switch(letter) {
        case 'A': return (Color)0;  // Rouge
        case 'B': return (Color)1;  // Bleu
        case 'C': return (Color)2;  // Vert
        case 'D': return (Color)3;  // Jaune
        case 'E': return (Color)4;  // Magenta
        case 'F': return (Color)5;  // Cyan
        case 'G': return (Color)6;  // Blanc
        case '^': return (Color)7;  // Blanc
        case 'v': return (Color)8;  // Noir
        default: return (Color)-1;  // Erreur
    }
}
 
const char* couleurverslettre(Color couleur) {
    switch (couleur) {
        case ERROR: return "ERROR";
        case EMPTY: return "Rouge";
        case PLAYER_1: return "Bleu";
        case PLAYER_2: return "Vert";
        case RED: return "Jaune";
        case GREEN: return "Magenta";
        case BLUE: return "Cyan";
        case YELLOW: return "Blanc";
        case MAGENTA: return "Blanc";
        case CYAN: return "Noir";
        case WHITE: return "EMPTY";
        default: return "UNKNOWN_COLOR";
    }
}
 
 
void print_map(GameState* state) {
    if (state->map == NULL) {
        printf("[ERROR] Carte non initialisée\n");
        return;
    }
    
    // Définition des couleurs pour chaque lettre
    const char* colors[] = {
        "\x1B[31m", // Rouge pour 'A'
        "\x1B[34m", // Bleu pour 'B'
        "\x1B[32m", // Vert pour 'C'
        "\x1B[33m", // Jaune pour 'D'
        "\x1B[35m", // Magenta pour 'E'
        "\x1B[36m", // Cyan pour 'F'
        "\x1B[37m", // Blanc pour 'G'
        "\x1B[37m", // Blanc pour '^'
        "\x1B[30m"  // Noir pour 'v'
    };
 
    printf("Carte generee (%dx%d):\n", state->size, state->size);
 
    for (int i = 0; i < state->size; i++) {
        for (int j = 0; j < state->size; j++) {
            Color color = get_map_value(state, j, i);
            if (color >= 0 && color < 9) {
                printf("%s%c\x1B[0m ", colors[color], lettres[color]); // Ajout des couleurs
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
 
 
void init_players(GameState* state, Player* player1, Player* player2) {
 
    player1->x = 0;
    player1->y = size - 1;
    player1->symbol = (Color)7;
 
 
    player2->x = size - 1;
    player2->y = 0;
    player2->symbol = (Color)8;
 
 
    set_map_value(state, player1->x, player1->y, player1->symbol);
    set_map_value(state, player2->x, player2->y, player2->symbol);
}
 
Color aleatoire(GameState* map, Color joueur){
    int valeur_aleatoire = rand() % 7;
    return((Color)valeur_aleatoire);
}

Color aleatoireglouton(GameState*map, Color joueur){
    Color * disponibles = cases_disponibles(map, joueur);
    int valeur_aleatoire = rand()% 7;
    while (disponibles[valeur_aleatoire]==0){
        valeur_aleatoire=rand()%7;
    }
    free(disponibles);
    return((Color)valeur_aleatoire);
}

 Color gloutonintelligent(GameState*map, Color joueur){
    Color *disponibles = cases_disponibles(map, joueur);
    int max = 0;
    int i =0;
    int indice = 0;
    while (i<7){
        if (disponibles[i]> max ){
            max = disponibles[i];
            indice = i;
        }
        i++;
    }
    return((Color)indice);
 }

int main(int argc, char** argv) {
    srand(time(NULL));
 
    GameState game;
    Player player1;
    Player player2;
    char Coup1;
    int Tour = 1;
 
    create_empty_game_state(&game, size);
    fill_map(&game);
    print_map(&game);
    init_players(&game, &player1, &player2);
    
    while (verifie(&game)==false){// tant que la partie n'est pas finie
        printf("Tour %d - Joueur 1 : Choisis une lettre (A, B, C, D, E, F, G):\n", Tour);
        scanf(" %c", &Coup1);
        Color couleur_choisie = lettre_couleur(Coup1);
        miseajour_map(&game, (Color)7, couleur_choisie); // MAj carte selon choix joueur '^'
        print_map(&game);// afficher carte apres maj
 
        if (verifie(&game)) break;
 
        Color couleur_choisie2 = gloutonintelligent(&game, (Color)8);
        printf("Le joueur aléatoire a choisi la couleur %s\n", couleurverslettre(couleur_choisie2));
        miseajour_map(&game, (Color)8, couleur_choisie2);  // Mettre à jour  carte selon le choix
        print_map(&game);  // Afficher carte après le choix du joueur 2
 
        Tour++;
    }
 
    free_game(&game);
    
 
    return 0;
}