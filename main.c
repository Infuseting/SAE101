#include <stdio.h>
#include "board.h"
#include <unistd.h>
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
      if (i % 2 == 0)
      {
        if (j % 2 == 0)
        {
          if (j == 0)
          {
            printf("  ");
          }
          if (i == 0 && j == 0) {
            printf("\e[34m╔\e[0m");
          }
          else if (i == NB_LINES * 2 && j == 0) {
            printf("\e[34m╚\e[0m");
          }
          else if (i == 0 && j == NB_COLS * 2) {
            printf("\e[34m╗\e[0m");
          }
          else if (i == NB_LINES * 2 && j == NB_COLS * 2) {
            printf("\e[34m╝\e[0m");
          }
          else if (i == NB_LINES * 2)
          {
            printf("\e[34m╩\e[0m");
          }
          else if (j == NB_COLS * 2)
          {
            printf("\e[34m╣\e[0m");
          }
          else if (i == 0) {
            printf("\e[34m╦\e[0m");
          }
          else if (j == 0)
          {
            printf("\e[34m╠\e[0m");
          }
          else
          {
            printf("\e[34m╬\e[0m");
          }
        }
        
        else
        {
          printf("\e[34m═\e[0m");
        }
      }
      else if (i % 2 == 1)
      {
        if (j % 2 == 0)
        {
          if (j == 0)
          {
            printf("%d ", i / 2);
          }
          printf("\e[34m║\e[0m");
        }
        else
        {
          cell c = get_content(game, i / 2, j / 2);
          if (c == NORTH_KING)
          {
            printf("\e[35m%c\e[0m", pseudoN == NULL ? 'N' : pseudoN[0]);
          }
          else if (c == SOUTH_KING)
          {
            printf("\e[33m%c\e[0m", pseudoS == NULL ? 'S' : pseudoS[0]);
          }
          else if (c == KILLED)
          {
            printf("\e[31m╳\e[0m");
          }
          else if (c == EMPTY)
          {
            printf(" ");
          }
          else
          {
            printf("\e[31mE\e[0m");
          }
        }
      }
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


bool can_move(board game, direction d)
{
  int line = 0;
  int column = 0;
  switch (d)
  {
  case NW:
    line = -1;
    column = -1;
    break;
  case N:
    line = -1;
    column = 0;
    break;
  case NE:
    line = -1;
    column = 1;
    break;
  case W:
    line = 0;
    column = -1;
    break;
  case E:
    line = 0;
    column = 1;
    break;
  case SW:
    line = 1;
    column = -1;
    break;
  case S:
    line = 1;
    column = 0;
    break;
  case SE:
    line = 1;
    column = 1;
    break;
  default:
    break;
  }
  int king_line = 0;
  int king_column = 0;
  for (int i = 0; i < NB_LINES; i++)
  {
    for (int j = 0; j < NB_COLS; j++)
    {
      if (current_player(game) == NORTH) {

        if (get_content(game, i, j) == NORTH_KING)
        {
          king_line = i;
          king_column = j;
        }
      }
      else {
        if (get_content(game, i, j) == SOUTH_KING)
        {
          king_line = i;
          king_column = j;
        }
      }
    }
  }
  if (king_line + line < 0 || king_line + line >= NB_LINES || king_column + column < 0 || king_column + column >= NB_COLS)
  {
    return false;
  }
  if (get_content(game, king_line + line, king_column + column) == EMPTY)
  {
    return true;
  }
  return false;
}

char *check_possible(board game, direction d)
{
  static char res[12];
  if (!can_move(game, d))
  {
    res[0] = '\e';
    res[1] = '[';
    res[2] = '3';
    res[3] = '1';
    res[4] = 'm';
  }
  else
  {
    res[0] = '\e';
    res[1] = '[';
    res[2] = '3';
    res[3] = '2';
    res[4] = 'm';
  }
  switch (d)
  {
  case 0:
    res[5] = '1';
    break;
  case 1:
    res[5] = '2';
    break;
  case 2:
    res[5] = '3';
    break;
  case 3:
    res[5] = '4';
    break;
  case 4:
    res[5] = '6';
    break;
  case 5:
    res[5] = '7';
    break;
  case 6:
    res[5] = '8';
    break;
  case 7:
    res[5] = '9';
    break;
  default:
    break;
  }
  res[6] = '\e';
  res[7] = '[';
  res[8] = '3';
  res[9] = '4';
  res[10] = 'm';
  res[11] = '\0';
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
  while (scanf("%d", &choix) != 1)
  {
    printf("Mauvaise entrée\n");
    while (getchar() != '\n')
      ;
  }
  switch (choix)
  {
  case 1:
    d = SW;
    break;
  case 2:
    d = S;
    break;
  case 3:
    d = SE;
    break;
  case 4:
    d = W;
    break;
  case 6:
    d = E;
    break;
  case 7:
    d = NW;
    break;
  case 8:
    d = N;
    break;
  case 9:
    d = NE;
    break;
  default:
    printf("Mauvaise direction\n");
    break;
  }
  return d;
}
void check_movement(board game)
{
  int ok = 1;
  while (ok == 1)
  {
    direction d = mouvement(game);
    if (move_toward(game, d) == OK)
    {
      printf("Déplacement effectué\n");
      ok = 0;
    }
    else if (move_toward(game, d) == BUSY)
    {
      printf("Déplacement impossible, case occupée\n");
    }
    else if (move_toward(game, d) == OUT)
    {
      printf("Déplacement impossible, case hors plateau\n");
    }
    else
    {
      printf("Déplacement impossible\n");
    }
  }
}

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
      printf("Mauvaise entrée\n");
      while (getchar() != '\n')
        ;
    }
    printf("Entrer une Colone : ");
    while (scanf("%d", &column) != 1)
    {
      printf("Mauvaise entrée\n");
      while (getchar() != '\n')
        ;
    }
    if (kill_cell(game, line, column) == OK)
    {
      printf("Case tuée\n");
      ok = 0;
    }
    else if (kill_cell(game, line, column) == OUT)
    {
      printf("Case hors plateau\n");
    }
    else if (kill_cell(game, line, column) == BUSY)
    {
      printf("Case occupée\n");
    }
    else
    {
      printf("Case impossible à tuer\n");
    }
  }
}

bool is_valid_pseudo(const char *pseudo) {
  int length = 0;
  while (pseudo[length] != '\0') {
    length++;
    if (length > 32) {
      return false;
    }
  }
  return length > 0;
}

void choix_pseudo(char *pseudoN, char *pseudoS) {
  do {
    printf("Joueur Nord, entrez votre pseudo (max 32 caractères) : ");
    scanf("%s", pseudoN);
    getchar();
    if (!is_valid_pseudo(pseudoN)) {
      printf("\033[2J");
      printf("Pseudo invalide. Veuillez réessayer.\n");
    }
  } while (!is_valid_pseudo(pseudoN));
  printf("\033[2J");
  do {
    printf("Joueur Sud, entrez votre pseudo (max 32 caractères) : ");
    scanf("%s", pseudoS);
    getchar();
    if (!is_valid_pseudo(pseudoS)) {
      printf("\033[2J");
      printf("Pseudo invalide. Veuillez réessayer.\n");
    }
  } while (!is_valid_pseudo(pseudoS));
  printf("\033[2J");
}


/**
 * @file main
 * @brief MAIN file for the game
 * @author Arthur et Arnaud
 */
int main(int args, char **argv)
{
  board game = new_special_game(false, false);
  printf("\033[2J");
  printf("Un plateau est créé.\n");
  char pseudoN[33];
  char pseudoS[33];
  choix_pseudo(pseudoN, pseudoS);
  while (get_winner(game) == NO_PLAYER)
  {
    
    afficher_plateau(game, pseudoN, pseudoS);
    afficher_joueur(game, pseudoN, pseudoS);
    afficher_texte(" A vous de jouer !\n");
    printf("\n");
    check_movement(game);
    printf("\033[2J");
    afficher_plateau(game, pseudoN, pseudoS);
    afficher_joueur(game, pseudoN, pseudoS);
    afficher_texte(" A vous de jouer !\n");
    printf("\n");
    choix_kill(game);
    printf("\033[2J");
  }
  printf("le joueur %s a gagné\n", get_winner(game) == NORTH ? "\e[35mNord\e[0m" : "\e[33mSud\e[0m");
  return 0;
}
