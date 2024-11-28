#include <stdio.h>
#include "board.h"
#include <unistd.h>

void error(int code, const char *message)
{
  printf("\e[1;31mErreur %d\e[0m\n", code);
  printf("\e[31m%s\e[0m\n", message);
}
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
void afficher_plateau(board game, char *pseudoN, char *pseudoS)
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
void afficher_texte(const char *texte)
{
  printf("%s", texte);
}

void find_king(board game, int *king_line, int *king_column){
  for (int i = 0; i < NB_LINES; i++) {
    for (int j = 0; j < NB_COLS; j++) {
      if (current_player(game) == NORTH){
        if (get_content(game, i, j) == NORTH_KING){*king_line = i; *king_column = j;}
      }
      else{
        if (get_content(game, i, j) == SOUTH_KING){ *king_line = i; *king_column = j;}
      }
    }
  }
}

bool can_move(board game, direction d)
{
  int line = 0, column = 0;
  switch (d){  case NW: line = -1; column = -1; break;
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
  if (king_line + line < 0 || king_line + line >= NB_LINES || king_column + column < 0 || king_column + column >= NB_COLS) return false;
  if (get_content(game, king_line + line, king_column + column) == EMPTY) return true;
  return false;
}

char *check_possible(board game, direction d)
{
  static char res[12];
  if (!can_move(game, d)) snprintf(res, sizeof(res), "\e[31m");
  else snprintf(res, sizeof(res), "\e[32m");
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
  snprintf(res + 6, sizeof(res) - 6, "\e[34m");
  return res;
}

direction mouvement(board game)
{
  direction d;
  printf("Dans quelle direction voulez-vous déplacer votre roi ?\n");
  printf("  \e[34m╔═══╦═══╦═══╗\e[0m\n");
  printf("  \e[34m║ %s ", check_possible(game, NW));
  printf("║ %s ", check_possible(game, N));
  printf("║ %s ║\e[0m\n", check_possible(game, NE));
  printf("  \e[34m╠═══╬═══╬═══╣\e[0m\n");
  printf("  \e[34m║ %s ", check_possible(game, W));
  printf("║ * ");
  printf("║ %s ║\e[0m\n", check_possible(game, E));
  printf("  \e[34m╠═══╬═══╬═══╣\e[0m\n");
  printf("  \e[34m║ %s ", check_possible(game, SW));
  printf("║ %s ", check_possible(game, S));
  printf("║ %s ║\e[0m\n", check_possible(game, SE));
  printf("  \e[34m╚═══╩═══╩═══╝\e[0m\n");
  int choix;
  while (scanf("%d", &choix) != 1) {
    error(2, "Le charactère entrée est incorrecte");
    while (getchar() != '\n');
  }
  switch (choix) {
  case 1: d = SW; break;
  case 2: d = S; break;
  case 3: d = SE; break;
  case 4: d = W; break;
  case 6: d = E; break;
  case 7: d = NW; break;
  case 8: d = N; break;
  case 9: d = NE; break;
  default: error(2, "Le charactère entrée est incorrecte"); break;
  }
  return d;
}

void check_movement(board game)
{
  int ok = 1;
  while (ok == 1) {
    direction d = mouvement(game);
    if (move_toward(game, d) == OK) ok = 0;
    else if (move_toward(game, d) == BUSY) error(3, "Déplacement impossible case occupée");
    else if (move_toward(game, d) == OUT) error(3, "Déplacement impossible hors plateau");
    else error(3, "Déplacement impossible");
  }
}

/**
 * @brief Asks the user to kill a cell.
 * @param game the game to play.
 * @note can kill a cell already kill.
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
    else
      error(1, "Case impossible à tuer");
  }
}

/**
 * @brief Checks if a pseudo is valid.
 * @param pseudo the pseudo to check.
 * @return true if the pseudo is valid, false otherwise.
 * @author Arthur
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
 * @brief Asks the user to input a pseudo.
 * @param pseudo the pseudo to input.
 * @note The pseudo must be at most 32 characters long.
 * @note The pseudo must not be empty.
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
 * @brief Asks the user to input two pseudos.
 * @param pseudoN the North player's pseudo.
 * @param pseudoS the South player's pseudo.
 * @note a little function to ask the user to input two pseudos.
 * @author Arthur
 *
 */
void choix_pseudo(char *pseudoN, char *pseudoS)
{
  input_pseudo(pseudoN);
  input_pseudo(pseudoS);
}

/**
 * @brief Asks the user to move their king.
 * @param game the game to play.
 * @param pseudoN the North player's pseudo.
 * @param pseudoS the South player's pseudo.
 * @note The game will be played by two players.
 * @author Arthur
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
 * @brief Asks the user to kill a cell.
 * @param game the game to play.
 * @param pseudoN the North player's pseudo.
 * @param pseudoS the South player's pseudo.
 * @note The game will be played by two players.
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
 * @brief Launches a game with the specified rules.
 * @param ranged whether the game uses the ranged killing rule.
 * @param hexagonal whether the game uses the hexagonal board rule.
 * @note The game will be played by two players.
 * @author Arthur
 */
void launch_game(bool ranged, bool hexagonal)
{
  board game = new_special_game(ranged, hexagonal);
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
  printf("le joueur %s a gagné\n", get_winner(game) == NORTH ? "\e[35mNord\e[0m" : "\e[33mSud\e[0m");
}

/**
 * @brief Converts a character to lowercase.
 * @param c the character to convert.
 * @return the lowercase character.
 * @note Only works for ASCII characters.
 * @note If the character is already lowercase, it will be returned as is.
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
 * @brief Asks the user if they want to play with a specific rule.
 * @author Arthur
 */
bool is_rules(char *rule)
{
  char c;
  printf("\033[2J");
  printf("Voulez-vous jouer avec la règle %s ? (o/n) ", rule);
  while (scanf(" %c", &c) != 1 || (lower(c) != 'o' && lower(c) != 'n'))
  {
    while (getchar() != '\n')
      ;
    printf("\033[2J");
    error(0, "Caractère invalide. Veuillez réessayer.");
    printf("Voulez-vous jouer avec la règle %s ? (o/n) ", rule);
  }
  getchar();
  return c == 'o';
}

/**
 * @file main
 * @brief MAIN file for the game
 * @author Arthur et Arnaud
 */
int main(int args, char **argv)
{
  bool ranged = false, hexagonal = false;

  hexagonal = is_rules("du plateau hexagonal");
  ranged = is_rules("de limite de distance");

  printf("%d\n", hexagonal);
  printf("%d\n", ranged);
  launch_game(ranged, hexagonal);
  return 0;
}
