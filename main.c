#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include <unistd.h>


/**
 * @brief Affiche un message d'erreur.
 * 
 * Cette fonction affiche un message d'erreur.
 * 
 * @param code Le code de l'erreur.
 * @param message Le message d'erreur.
 * 
 * @author Arthur
 */
void error(int code, const char *message)
{
  printf("\e[1;31mErreur %d\e[0m\n", code);
  printf("\e[31m%s\e[0m\n", message);
}











/**
 * @brief Dessine une ligne du plateau de jeu.
 * 
 * Cette fonction dessine une ligne du plateau de jeu.
 * 
 * @param i La ligne de la case.
 * @param j La colonne de la case.
 * 
 * @author Arthur
 */
void draw_line(int i, int j) {
  if (j == 0) printf("  ");
  if (i == 0 && j == 0) printf("\e[34m╔\e[0m");
  else if (i == NB_LINES * 2 && j == 0) printf("\e[34m╚\e[0m");
  else if (i == 0 && j == NB_COLS * 2) printf("\e[34m╗\e[0m");
  else if (i == NB_LINES * 2 && j == NB_COLS * 2) printf("\e[34m╝\e[0m");
  else if (i == NB_LINES * 2) printf("\e[34m╩\e[0m");
  else if (j == NB_COLS * 2) printf("\e[34m╣\e[0m");
  else if (i == 0) printf("\e[34m╦\e[0m");
  else if (j == 0) printf("\e[34m╠\e[0m");
  else printf("\e[34m╬\e[0m");
}

/**
 * @brief Dessine le contenu d'une case du plateau de jeu.
 * 
 * Cette fonction dessine le contenu d'une case du plateau de jeu.
 * 
 * @param i La ligne de la case.
 * @param j La colonne de la case.
 * @param game Le plateau de jeu.
 * @param pseudoN Le pseudo du joueur Nord.
 * @param pseudoS Le pseudo du joueur Sud.
 * 
 * @author Arthur
 */
void draw_content(int i, int j, board game, char *pseudoN, char *pseudoS)
{
  switch (get_content(game, i, j))
  {
  case EMPTY:
    printf(" ");
    break;
  case KILLED:
    printf("\e[31m╳\e[0m");
    break;
  case NORTH_KING:
    printf("\e[35m%c\e[0m", pseudoN == NULL ? 'N' : pseudoN[0]);
    break;
  case SOUTH_KING:
    printf("\e[33m%c\e[0m", pseudoS == NULL ? 'S' : pseudoS[0]);
    break;
  default:
    printf("\e[31mE\e[0m");
    break;
  }
}
/**
 * @brief Dessine une case du plateau de jeu.
 * 
 * Cette fonction dessine une case du plateau de jeu.
 * 
 * @param i La ligne de la case.
 * @param j La colonne de la case.
 * @param game Le plateau de jeu.
 * @param pseudoN Le pseudo du joueur Nord.
 * @param pseudoS Le pseudo du joueur Sud.
 */
void draw_box(int i, int j, board game, char *pseudoN, char *pseudoS){
  if (i % 2 == 0) {
    if (j % 2 == 0){
      draw_line(i,j);
    }
    else printf("\e[34m═\e[0m");
  }
  else if (i % 2 == 1) {
    if (j % 2 == 0) {
      if (j == 0) printf("%d ", i / 2);
      printf("\e[34m║\e[0m");
    }
    else {
      draw_content(i / 2, j / 2, game, pseudoN, pseudoS);
    }
  }
}





/**
 * @brief Affiche la tête du plateau de jeu.
 * 
 * Cette fonction affiche la tête du plateau de jeu.
 * 
 * @param n La taille du plateau de jeu.
 * @param offset L'offset du plateau de jeu.
 * 
 * @author Arthur
 */
void head_line(int n, int offset){

	for(int i=0;i<offset + 1;i++)
		printf(" ");
	for(int i=0;i<n;i++)
		printf("\e[36m╱ ╲\e[0m ");
	printf("\n");
}
/**
 * @brief Affiche la tête du plateau de jeu.
 * 
 * Cette fonction affiche la tête du plateau de jeu.
 * 
 * @param n La taille du plateau de jeu.
 * @param offset L'offset du plateau de jeu.
 * @param game Le plateau de jeu.
 * @param pseudoN Le pseudo du joueur Nord.
 * @param pseudoS Le pseudo du joueur Sud.
 * @param y La ligne du plateau de jeu.
 * @param x La colonne du plateau de jeu.
 * 
 * @author Arthur
 */
void body_line(int n, int offset, board game, char *pseudoN, char *pseudoS, int y, int x){

	for(int i=0;i<offset;i++)
		printf(" ");
  printf("\e[32m%d\e[0m", y);
	printf("\e[32m|\e[0m");
	for(int i=0;i<n;i++) {
		printf(" ");
    draw_content(y, i + x, game, pseudoN, pseudoS);
    printf(" \e[32m|\e[0m");
  }
	printf("\n");
}
/**
 * @brief Affiche le pied du plateau de jeu.
 * 
 * Cette fonction affiche le pied du plateau de jeu.
 * 
 * @param n La taille du plateau de jeu.
 * @param offset L'offset du plateau de jeu.
 * 
 * @author Arthur
 */
void foot_line(int n, int offset){

	for(int i=0;i<offset + 1;i++)
		printf(" ");
	for(int i=0;i<n;i++)
		printf("\e[36m╲ ╱\e[0m ");
	printf("\n");
}

/**
 * @brief Affiche le corps du plateau de jeu.
 * 
 * Cette fonction affiche le corps du plateau de jeu.
 * 
 * @param n La taille du plateau de jeu.
 * @param game Le plateau de jeu.
 * @param pseudoN Le pseudo du joueur Nord.
 * @param pseudoS Le pseudo du joueur Sud.
 * @param y La ligne du plateau de jeu.
 * 
 * @author Arthur
 */
void body(int n, board game, char *pseudoN, char *pseudoS, int y){

	head_line(n,1);
	body_line(n,0, game, pseudoN, pseudoS, y,0);
	foot_line(n,1);

} 

/**
 * @brief Affiche la tête du plateau de jeu.
 * 
 * Cette fonction affiche la tête du plateau de jeu.
 * 
 * @param n La taille du plateau de jeu.
 * @param x addition au numero
 * @param space espace en plus
 * 
 * @author Arthur 
 * 
 *  */
void numerote(int n, int x, int space) {
  printf("   ");
  for (int i = 0; i < n + space ; i++) {
    printf(" ");
  }
  for (int i = 0; i < n; i++) {
    printf("\e[36m%d\e[0m   ", i + x);
  }
  printf("\n");
}

/**
 * @brief Affiche la tête du plateau de jeu.
 * 
 * Cette fonction affiche la tête du plateau de jeu.
 * 
 * @param n La taille du plateau de jeu.
 * @param offset L'offset du plateau de jeu.
 * @param game Le plateau de jeu.
 * @param pseudoN Le pseudo du joueur Nord.
 * @param pseudoS Le pseudo du joueur Sud.
 * @param y La ligne du plateau de jeu.
 * 
 * @author Arthur
 */
void head(int n,int offset, board game, char *pseudoN, char *pseudoS, int y){

	head_line(n,offset+1);
	body_line(n,offset, game, pseudoN, pseudoS, y, 0);

}

/**
 * @brief Affiche le pied du plateau de jeu.
 * 
 * Cette fonction affiche le pied du plateau de jeu.
 * 
 * @param n La taille du plateau de jeu.
 * @param offset L'offset du plateau de jeu.
 * @param game Le plateau de jeu.
 * @param pseudoN Le pseudo du joueur Nord.
 * @param pseudoS Le pseudo du joueur Sud.
 * @param y La ligne du plateau de jeu.
 * @param x La colonne du plateau de jeu.
 * 
 * @author Arthur
 */
void foot(int n,int offset, board game, char *pseudoN, char *pseudoS, int y, int x){

	body_line(n,offset, game, pseudoN, pseudoS, y, x);
	foot_line(n,offset+1);

}

/**
 * @brief Affiche le plateau de jeu.
 * 
 * Cette fonction affiche le plateau de jeu.
 * 
 * @param n La taille du plateau de jeu.
 * @param game Le plateau de jeu.
 * @param pseudoN Le pseudo du joueur Nord.
 * @param pseudoS Le pseudo du joueur Sud.
 * 
 * @author Arthur
 */
void fig(int n, board game, char *pseudoN, char *pseudoS){
	int y = 0;
	int size=n;
	for(int i=n-1;i>0;i--){
		head(size,(i*2), game, pseudoN, pseudoS, y);
    y++;
		size++;
	}
	
	body(size, game, pseudoN, pseudoS,y);
	y++;
  int x = 1;
  
  for(int i=1;i<n;i++){
		size--;
		foot(size,i*2, game, pseudoN, pseudoS,y, x);
    y++;
    x++;
	}

}

/**
 * @brief Affiche le plateau de jeu.
 * 
 * Cette fonction affiche le plateau de jeu.
 * 
 * @param game Le plateau de jeu.
 * @param pseudoN Le pseudo du joueur Nord.
 * @param pseudoS Le pseudo du joueur Sud.
 * 
 * @author Arthur
 */
void afficher_plateau(board game, char *pseudoN, char *pseudoS)
{
  if (is_hex(game))
  {
    numerote(HEX_SIDE, 0, 5);
    fig((MAX_DIMENSION % 2 == 0) ? (MAX_DIMENSION / 2) : (MAX_DIMENSION / 2) + 1, game, pseudoN, pseudoS);
    numerote(HEX_SIDE, 4, 0);
  }
  else
  {
    printf("   ");
    for (int i = 0; i < NB_COLS * 2; i++)
    {
      if (i % 2 == 0)
      {
        printf("%d ", i / 2);
      }
    }
    printf("\n");
    for (int i = 0; i < NB_LINES * 2 + 1; i++)
    {
      for (int j = 0; j < NB_COLS * 2 + 1; j++)
      {
        draw_box(i, j, game, pseudoN, pseudoS);
      }
      printf("\n");
    }
  }
}

/**
 * @brief Affiche le joueur courant.
 * 
 * Cette fonction affiche le joueur courant.
 * 
 * @param game Le plateau de jeu.
 * @param pseudoN Le pseudo du joueur Nord.
 * @param pseudoS Le pseudo du joueur Sud.
 * 
 * @author Arthur
 */
void afficher_joueur(board game, char *pseudoN, char *pseudoS)
{
  player p = current_player(game);
  if (p == NORTH)
  {
    printf("\e[35m%s\e[0m", pseudoN);
  }
  else if (p == SOUTH)
  {
    printf("\e[33m%s\e[0m", pseudoS);
  }
  else
  {
    printf("Pas de joueur");
  }
}
/**
 * @brief Affiche un texte.
 * 
 * Cette fonction affiche un texte.
 * 
 * @param texte Le texte à afficher.
 * 
 * @author Arthur
 */
void afficher_texte(const char *texte)
{
  printf("%s", texte);
}
/**
 * @brief Trouve la position du roi.
 * 
 * Cette fonction trouve la position du roi.
 * 
 * @param game Le plateau de jeu.
 * @param king_line La ligne du roi.
 * @param king_column La colonne du roi.
 * 
 * @author Arthur
 */
void find_king(board game, int *king_line, int *king_column){
  for (int i = 0; i < MAX_DIMENSION; i++) {
    for (int j = 0; j < MAX_DIMENSION; j++) {
      if (current_player(game) == NORTH){
        if (get_content(game, i, j) == NORTH_KING){*king_line = i; *king_column = j;}
      }
      else{
        
        if (get_content(game, i, j) == SOUTH_KING){ *king_line = i; *king_column = j;}
      }
    }
  }
}
/** 
 * @brief Vérifie si un mouvement est possible.
 * 
 * Cette fonction vérifie si un mouvement est possible.
 * 
 * @param game Le plateau de jeu.
 * @param d La direction du mouvement.
 * @return true si le mouvement est possible, false sinon.
 * 
 * @author Arthur
 */
bool can_move(board game, direction d)
{
  int line = 0, column = 0;
  switch (d){  
  case NW: line = -1; column = -1; break;
  case N: line = -1; column = 0; break;
  case NE: line = -1; column = 1; break;
  case W: line = 0; column = -1; break;
  case E: line = 0; column = 1; break;
  case SW: line = 1; column = -1; break;
  case S: line = 1; column = 0; break;
  case SE: line = 1; column = 1; break;
  default: break;}
  int king_line = 0, king_column = 0;
  find_king(game, &king_line, &king_column);
  if (is_hex(game)) {
    if (king_line + line < 0 || king_line + line >= MAX_DIMENSION || king_column + column < 0 || king_column + column >= MAX_DIMENSION) return false;
  } else {
    if (king_line + line < 0 || king_line + line >= NB_LINES || king_column + column < 0 || king_column + column >= NB_COLS) return false;
  }
  if (get_content(game, king_line + line, king_column + column) == EMPTY) return true;
  return false;
}

/**
 * @brief Vérifie si un mouvement est possible.
 * 
 * Cette fonction vérifie si un mouvement est possible.
 * 
 * @param game Le plateau de jeu.
 * @param d La direction du mouvement.
 * @return true si le mouvement est possible, false sinon.
 * 
 * @author Arthur
 */
char *check_possible(board game, direction d)
{
  char *res = (char *)malloc(12 * sizeof(char));
  if (!res) {
    error(1, "Memory allocation failed");
    return NULL;
  }
  if (!can_move(game, d)) snprintf(res, 12, "\e[31m");
  else snprintf(res, 12, "\e[32m");
  switch (d){
  case 0: res[5] = '1'; break;
  case 1: res[5] = '2'; break;
  case 2: res[5] = '3'; break;
  case 3: res[5] = '4'; break;
  case 4: res[5] = '6'; break;
  case 5: res[5] = '7'; break;
  case 6: res[5] = '8'; break;
  case 7: res[5] = '9'; break;
  default: break;}
  snprintf(res + 6, 6, "\e[34m");
  return res;
}


/**
 * @brief Demande à l'utilisateur de choisir une direction.
 * 
 * Cette fonction demande à l'utilisateur de choisir une direction.
 * 
 * @param game Le plateau de jeu.
 * @return La direction choisie.
 * 
 * @author Arnaud et Arthur
 */
direction mouvement(board game)
{
  direction d;
  printf("Dans quelle direction voulez-vous déplacer votre roi ?\n");
  if (!is_hex(game)) {
    printf("  \e[34m╔═══╦═══╦═══╗\e[0m\n"
           "  \e[34m║ %s ║ %s ║ %s ║\e[0m\n"
           "  \e[34m╠═══╬═══╬═══╣\e[0m\n"
           "  \e[34m║ %s ║ * ║ %s ║\e[0m\n"
           "  \e[34m╠═══╬═══╬═══╣\e[0m\n"
           "  \e[34m║ %s ║ %s ║ %s ║\e[0m\n"
           "  \e[34m╚═══╩═══╩═══╝\e[0m\n",
           check_possible(game, NW), check_possible(game, N), check_possible(game, NE),
           check_possible(game, W), check_possible(game, E),
           check_possible(game, SW), check_possible(game, S), check_possible(game, SE));
  }
  else
  {
    printf("   \e[34m╱╲╱╲   \e[0m\n  \e[34m▕%s▕%s▕   \e[0m\n  \e[34m╱╲╱╲╱╲  \e[0m\n \e[34m▕%s▕*▕%s▕ \e[0m\n  \e[34m╲╱╲╱╲╱  \e[0m\n  \e[34m▕%s▕%s▕ \e[0m\n   \e[34m╲╱╲╱   \e[0m\n",
         check_possible(game, NW), check_possible(game, NE),
         check_possible(game, W), check_possible(game, E),
         check_possible(game, SW), check_possible(game, SE));
  }

  int choix;
  while (scanf("%d", &choix) != 1) {
    error(2, "Le charactère entrée est incorrecte");
    while (getchar() != '\n');
  }
  switch (choix) {
  case 1: d = SW; break;
  case 2: if (!is_hex(game)) d = S; break;
  case 3: d = SE; break;
  case 4: d = W; break;
  case 6: d = E; break;
  case 7: d = NW; break;
  case 8: if (!is_hex(game)) d = N; break;
  case 9: d = NE; break;
  default: error(2, "Le charactère entrée est incorrecte"); break;
  }
  return d;
}
/**
 * @brief Vérifie si un mouvement est possible.
 * 
 * Cette fonction vérifie si un mouvement est possible.
 * 
 * @param game Le plateau de jeu.
 * @return true si le mouvement est possible, false sinon.
 * 
 * @author Arnaud
 */
void check_movement(board game)
{
  int ok = 1;
  while (ok == 1) {
    direction d = mouvement(game);
    if (move_toward(game, d) == OK) ok = 0;
    else if (move_toward(game, d) == BUSY) error(3, "Déplacement impossible case occupée");
    else if (move_toward(game, d) == OUT) error(3, "Déplacement impossible hors plateau");
    else if (move_toward(game, d) == RULES) error(3, "Déplacement impossible règles");
    else error(3, "Déplacement impossible");
  }
}
/** 
 * @brief Demande à l'utilisateur de choisir une case à tuer.
 * 
 * Cette fonction demande à l'utilisateur de choisir une case à tuer.
 * 
 * @param game Le plateau de jeu.
 * 
 * @author Arnaud
 * 
 */
void choix_kill(board game)
{
  int ok = 1;
  while (ok == 1)
  {
    printf("\nQuelle case voulez-vous tuer ?\n");
    int line;
    int column;
    printf("Entrer une ligne: ");
    while (scanf("%d", &line) != 1)
    {
      error(2, "Le charactère entrée est incorrecte");
      while (getchar() != '\n')
        ;
    }
    printf("Entrer une Colone : ");
    while (scanf("%d", &column) != 1)
    {
      error(2, "Le charactère entrée est incorrecte");
      while (getchar() != '\n')
        ;
    }
    if (kill_cell(game, line, column) == OK)
      ok = 0;
    else if (kill_cell(game, line, column) == OUT)
      error(1, "Case hors plateau");
    else if (kill_cell(game, line, column) == BUSY)
      error(1, "Case occupée");
    else if (kill_cell(game, line, column) == RULES)
      error(1, "Case trop loin de votre pion");
    else
      error(1, "Case impossible à tuer");
  }
}

/**
 * @brief Vérifie si un pseudo est valide.
 * 
 * Cette fonction vérifie si un pseudo est valide.
 * 
 * @param pseudo Le pseudo à vérifier.
 * @return true si le pseudo est valide, false sinon.
 * 
 * @author Arnaud
 */
bool is_valid_pseudo(const char *pseudo)
{
  int length = 0;
  while (pseudo[length] != '\0')
  {
    length++;
    if (length > 32 || length < 1)
    {
      return false;
    }
  }
  return length > 0;
}

/**
 * @brief Demande à l'utilisateur de choisir un pseudo.
 * 
 * Cette fonction demande à l'utilisateur de choisir un pseudo.
 * 
 * @param pseudo Le pseudo du joueur.
 * 
 * @author Arthur
 */
void input_pseudo(char *pseudo)
{
  do
  {
    printf("Entrez votre pseudo (max 32 caractères) : ");
    scanf("%s", pseudo);
    getchar();
    if (!is_valid_pseudo(pseudo))
    {
      printf("\033[2J");
      error(0, "Pseudo invalide. Veuillez réessayer.");
    }
  } while (!is_valid_pseudo(pseudo));
  printf("\033[2J");
}

/** 
 * @brief Demande aux joueurs de choisir un pseudo.
 * 
 * Cette fonction demande aux joueurs de choisir un pseudo.
 * 
 * @param pseudoN Le pseudo du joueur Nord.
 * @param pseudoS Le pseudo du joueur Sud.
 * 
 * @author Arthur
 */
void choix_pseudo(char *pseudoN, char *pseudoS)
{
  input_pseudo(pseudoN);
  input_pseudo(pseudoS);
}


/**
 * @brief Déplace un pion sur le plateau.
 * 
 * Cette fonction permet de déplacer un pion sur le plateau en demandant à l'utilisateur de choisir une direction.
 * 
 * @param game Le plateau de jeu.
 * @param pseudoN Le pseudo du joueur Nord.
 * @param pseudoS Le pseudo du joueur Sud.
 * 
 * @author Arnaud
 */
void move_part(board game, char *pseudoN, char *pseudoS)
{
  printf("\033[2J");
  afficher_plateau(game, pseudoN, pseudoS);
  afficher_joueur(game, pseudoN, pseudoS);
  afficher_texte(" A vous de jouer !\n");
  printf("\n");
  check_movement(game);
}

/**
 * @brief Fonction qui permet de tuer une case.
 * 
 * Cette fonction permet de tuer une case en demandant à l'utilisateur de choisir une case à tuer.
 * 
 * @param game Le plateau de jeu.
 * @param pseudoN Le pseudo du joueur Nord.
 * @param pseudoS Le pseudo du joueur Sud.
 * 
 * @author Arnaud
 */
void kill_part(board game, char *pseudoN, char *pseudoS)
{
  printf("\033[2J");
  afficher_plateau(game, pseudoN, pseudoS);
  afficher_joueur(game, pseudoN, pseudoS);
  afficher_texte(" A vous de jouer !\n");
  printf("\n");
  choix_kill(game);
}

/**
 * @brief Lance une partie de jeu.
 * 
 * Cette fonction lance une partie de jeu en initialisant un plateau de jeu et en demandant aux joueurs
 * 
 * @param ranged true si la règle de distance est activée, false sinon.
 * @param hexagonal true si le plateau est hexagonal, false sinon.
 * 
 * @author Arthur et Arnaud
 */
void launch_game(bool ranged, bool hexagonal)
{
  board game = new_special_game(hexagonal, ranged);
  printf("\033[2J");
  char pseudoN[33];
  char pseudoS[33];
  choix_pseudo(pseudoN, pseudoS);
  while (get_winner(game) == NO_PLAYER)
  {
    move_part(game, pseudoN, pseudoS);
    kill_part(game, pseudoN, pseudoS);
  }
  printf("\033[2J");
  afficher_plateau(game, pseudoN, pseudoS);
  printf("le joueur ");
  afficher_joueur(game, pseudoN, pseudoS);
  printf(" a gagné\n (enter)");
  while (getchar() != '\n');
}

/**
 * @brief Convertit un caractère en minuscule.
 * 
 * @param c Le caractère à convertir.
 * @return Le caractère en minuscule.
 * @author Arthur
 */

char lower(char c)
{
  if (c >= 'A' && c <= 'Z')
  {
    return c + 32;
  }
  return c;
}


/**
 * @brief Vérifie si l'utilisateur souhaite jouer avec une règle spécifique.
 *
 * Cette fonction affiche un message demandant à l'utilisateur s'il souhaite jouer avec une règle donnée.
 * L'utilisateur doit répondre par 'o' pour oui ou 'n' pour non. Si l'entrée est incorrecte, un message
 * d'erreur est affiché et la question est reposée jusqu'à ce qu'une réponse valide soit fournie.
 *
 * @param rule Le nom de la règle à vérifier.
 * @return true si l'utilisateur souhaite jouer avec la règle, false sinon.
 *
 * @author Arthur
 */
bool is_rules(char *rule)
{
  char c;
  printf("\033[2J");
  printf("Voulez-vous jouer %s ? (o/n) ", rule);
  while (scanf(" %c%*[^\n]", &c) != 1 || (lower(c) != 'o' && lower(c) != 'n'))
  {
    printf("\033[2J");
    error(2, "Le charactère entrée est incorrecte");
    printf("Voulez-vous jouer %s ? (o/n) ", rule);
    while (getchar() != '\n');
  }
  return lower(c) == 'o';
}

/**
 * @file main
 * @brief MAIN file for the game
 * @author Arthur et Arnaud
 */
int main(int args, char **argv)
{
  do {
    bool ranged = false, hexagonal = false;

    hexagonal = is_rules(" avec la règle du plateau hexagonal");
    ranged = is_rules(" avec la règle  de limite de distance");
    launch_game(ranged, hexagonal);
  } while (is_rules("une nouvelle partie"));
  
  return 0;
}
