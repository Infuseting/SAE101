#include <stdio.h>
#include "board.h"

/**
 * @file main_example.c
 * @brief a simple illustration of how to include and use board.h and board.o.
 * @author Dorbec
 */

/**
 * @brief example of a main function that simply 
 * make a moves for ::NORTH then quit
 */
int main(int args, char **argv){
  board game = new_game();
  printf("Un plateau est créé.\n");
  enum return_code result = move_toward(game, SE); 
  if(result == OK){
    printf("déplacement réussi.\n");
  }
  else{
    printf("Aïe, pas réussi à déplacer la pièce.\n");
  }
  printf("J'essaye de retirer une case : \n");
  enum return_code result2 = kill_cell(game, 0, 0); 
  if(result2 == RULES){
    printf("Il aurait fallu réussir à bouger avant.\n");
  }
  else if(result2 == OK){
    printf("ça marche!\n");
  }
  else{
    printf("Bizarre.\n");
  }
  // à vous de jouer
  // vous pouvez utiliser des fonctions intermédiaires ;-)
  destroy_game(game);
  printf("suppression du plateau et sortie\n");	
  return 0;
}


