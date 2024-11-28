#include <stdio.h>
#include "board.h"
#include <unistd.h>
void afficher_plateau(board game)
{
  printf("   ");
  for (int i = 0; i < NB_COLS * 2; i++)
  {
    if (i % 2  == 0) {
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
          if (j == 0) {
            printf("  ");
          }
          printf("+");
        }
        else
        {
          printf("-");
        }
      }
      else if (i % 2 == 1)
      {
        if (j % 2 == 0)
        {
          if (j == 0) {
            printf("%d ", i / 2);
          }
          printf("|");
        }
        else
        {
          cell c = get_content(game, i / 2, j / 2);
          if (c == NORTH_KING)
          {
            printf("\e[35mN\e[0m");
          }
          else if (c == SOUTH_KING)
          {
            printf("\e[33mS\e[0m");
          }
          else if (c == KILLED)
          {
            printf("\e[30m#\e[0m");
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
void afficher_joueur(board game)
{
  player p = current_player(game);
  if (p == NORTH)
  {
    printf("\e[35mJoueur Nord\e[0m");
  }
  else if (p == SOUTH)
  {
    printf("\e[33mJoueur Sud\e[0m");
  }
  else
  {
    printf("Pas de joueur");
  }
}


direction mouvement(board game)
{
  direction d;
  printf("Dans quelle direction voulez-vous déplacer votre roi ?\n");
  printf("  -------------\n");
  printf("  | 6 | 7 | 8 |\n");
  printf("  -------------\n");
  printf("  | 4 | * | 5 |\n");
  printf("  -------------\n");
  printf("  | 1 | 2 | 3 |\n");
  printf("  -------------\n");
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
  case 5:
    d = E;
    break;
  case 6:
    d = NW;
    break;
  case 7:
    d = N;
    break;
  case 8:
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
  while (ok == 1) {
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




/**
 * @file main
 * @brief MAIN file for the game
 * @author Arthur et Arnaud
 */
int main(int args, char **argv)
{
  board game = new_special_game(false, false);
  printf("Un plateau est créé.\n");
  while (get_winner(game) == NO_PLAYER)
  {
    afficher_plateau(game);
    afficher_joueur(game);
    printf("\n");
    check_movement(game);
  }
  return 0;
}
