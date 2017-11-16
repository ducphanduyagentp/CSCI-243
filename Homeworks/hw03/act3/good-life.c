/// http://www.dreamincode.net/forums/topic/55572-conways-game-of-life/
/// downloaded by copy-paste on 8/19/2015 and modified to use an infinite loop
/// and have different Rule functions.
///
/// This code needs serious work in these areas:
/// <ol>
/// <li>  syntax: there are a number of warnings that cause errors.
/// </li>
/// <li>  semantics: the game algorithm implementation is incorrect.
/// </li>
/// <li>  design: the implementation needs function and value refactoring.
/// </li>
/// <li>  style: formatting is poor; the mix of TAB and spaces indentation
/// needs correction, and spacing between tokens is inconsistent.
/// The course style puts the '{' at the end of the function header, not
/// on a line of its own, and function headers belong at the left margin.
/// </li>
/// <li>  documentation: documentation is non-existent except for this block.
/// </li>
/// </ol>
///

#define _BSD_SOURCE
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include "display.h"

/**
  Function for program header.
*/
void header(void) {
  printf("\n\t..Welcome to the Game of life..\n");
}

/**
  Function to get the number of neighbors of a cell.
  @param worldSize: the size of the grid
  @param life: the representation of the game
  @param row: the row number of the cell
  @param col: the column number of the cell
  
  @return the number of neighbors of the provided cell
*/
int getNumNeighbors(int worldSize, char life[][worldSize], int row, int col) {
  int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int dc[] = {-1, 0, 1, -1 ,1, -1, 0, 1};
  int neighbors = 0;
  int i;
  for (i = 0; i < 8; i ++) {
    int dRow = (row + dr[i] + worldSize) % worldSize;
    int dCol = (col + dc[i] + worldSize) % worldSize;
    neighbors += life[dRow][dCol] == '*';
  }
  return neighbors;
}

/**
  The function to apply game rule on each cell, producing the 
  next generation.
  @param worldSize: the size of the grid
  @param life: the representation of the game
*/
void lifeRule(int worldSize, char life[][worldSize]) {
  int row, col;
  int neighbors;
  char nextLife[worldSize][worldSize];

  for (row = 0; row < worldSize; row++) {
    for (col = 0; col < worldSize; col++) {
      neighbors = getNumNeighbors(worldSize, life, row, col);    

      if (life[row][col] == '*') { // A live cell
        if (neighbors < 2 || neighbors > 3) {
          // dies by underpopulation or overpopulation
          nextLife[row][col] = ' ';
        } else {
          // survived to the next generation
          nextLife[row][col] = '*';
        }
      } else if (life[row][col] == ' ') { // A dead cell
        if (neighbors == 3) {
          // Reproduced
          nextLife[row][col] = '*';
        } else {
          // Stay dead
          nextLife[row][col] = ' ';
        }
      }
    }
  }
  
  for (row = 0; row < worldSize; row++) {
    for (col = 0; col < worldSize; col++) {
      life[row][col] = nextLife[row][col];
    }
  }
  return;
}

/**
  Function to print out the current generation of the game
  @param worldSize: the size of the grid
  @param life: the representation of the game
  @param gens the current generation
*/
void printWorld(int worldSize, char life[][worldSize], int gens) {
  int row, col;
  for (row = 0; row < worldSize; row++) {
    set_cur_pos(row + 1, col + 1);
    for (col = 0; col < worldSize; col++) {
      //printf("%c", life[row][col]);
      put(life[row][col]);
    }
    put('\n');
  }
  
  int copy = 0;

  while(gens) {
    copy *= 10;
    copy += gens % 10;
    gens /= 10;
  }

  gens = copy;
  col = 1;
  while(gens) {
    set_cur_pos(worldSize + 1, col++);
    put(gens % 10 + '0');
    gens /= 10;
  }

  usleep( 81000 );
}

/**
  Main function that prompts for user input and starts the simulation.
*/
int main(int argc, char **argv) {
  int worldSize = 20;
  int orgs; //fix 5
  int i, row, col; //fix 5
  int gens = 0;
  
  if (argc > 1) {
    worldSize = atoi(argv[1]);
  }
  
  char life[worldSize][worldSize];
  for (row = 0; row < worldSize; row++) {
    for (col = 0; col < worldSize; col++) {
        life[row][col] = ' '; //fix 2
    }
  }
  
  //fix 5
  header();

  printf("\nPlease enter the initial number of organisms: ");
  scanf("%i", &orgs);

  clear();

  srand(31);

  for (i = 0; i < orgs; i++) {
    row = rand() % worldSize;
    col = rand() % worldSize;
    life[row][col] = '*'; //fix 2
  }
  
  printWorld(worldSize, life, gens++);

  while (1) {
    lifeRule(worldSize, life);
    printWorld(worldSize, life, gens++);
  }

  return 0;
}