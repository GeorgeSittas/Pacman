#include <ncurses.h>
#include <stdlib.h>

#include "pacman.h"
#include "utilities.h"

bool pacman_died(void) {
  return game.pacman.died;
}

int pacman_row(void) {
  return game.pacman.position.row;
}

int pacman_col(void) {
  return game.pacman.position.col;
}

position_t pacman_position(void) {
  return game.pacman.position;
}

direction_t pacman_direction(void) {
  return game.pacman.direction;
}

int ghost_index(char ghost_initial) {
  switch (ghost_initial) {
    case BLINKY: return  0;
    case PINKY:  return  1;
    case INKY:   return  2;
    case CLYDE:  return  3;
    default:     return -1; // Invalid index
  }
}

int ghost_row(char ghost_initial) {
  return game.ghosts[ghost_index(ghost_initial)].position.row;
}

int ghost_col(char ghost_initial) {
  return game.ghosts[ghost_index(ghost_initial)].position.col;
}

position_t ghost_position(char ghost_initial) {
  return game.ghosts[ghost_index(ghost_initial)].position;
}

direction_t ghost_direction(char ghost_initial) {
  return game.ghosts[ghost_index(ghost_initial)].direction;
}

behaviour_t ghost_behaviour(char ghost_initial) {
  return game.ghosts[ghost_index(ghost_initial)].behaviour;
}

void ghost_heuristic(char ghost_initial) {
  game.ghosts[ghost_index(ghost_initial)].heuristic();
}

int score(void) {
  return game.score;
}

int lives(void) {
  return game.lives;
}

int scatter_counter(void) {
  return game.scatter_counter;
}

bool pacman_eaten(void) {
  bool eaten_by_blinky = (pacman_row() == ghost_row(BLINKY)
                       && pacman_col() == ghost_col(BLINKY)
                       && ghost_behaviour(BLINKY) != FRIGHTENED);

  // bool eaten_by_pinky = (pacman_row() == ghost_row(PINKY)
  //                     && pacman_col() == ghost_col(PINKY)
  //                     && ghost_behaviour(PINKY) != FRIGHTENED);

  // bool eaten_by_inky = (pacman_row() == ghost_row(INKY)
  //                    && pacman_col() == ghost_col(INKY)
  //                    && ghost_behaviour(INKY) != FRIGHTENED);

  // bool eaten_by_clyde = (pacman_row() == ghost_row(CLYDE)
  //                     && pacman_col() == ghost_col(CLYDE)
  //                     && ghost_behaviour(CLYDE) != FRIGHTENED);

  // return (eaten_by_blinky || eaten_by_pinky
  //      || eaten_by_inky   || eaten_by_clyde);

  return eaten_by_blinky;
}

bool ghost_eaten(char ghost_initial) {
  return (pacman_row() == ghost_row(ghost_initial)
       && pacman_col() == ghost_col(ghost_initial)
       && ghost_behaviour(ghost_initial) == FRIGHTENED);
}

bool is_char(int row, int col, char ch) {
  return (game.screen[row][col] == ch);
}

bool is_intersection(position_t tile) {
  return (adjacent_available_tiles(tile) > 2);
}

int distance_squared(position_t a, position_t b) {
  return (a.row - b.row)*(a.row - b.row) + (a.col - b.col)*(a.col - b.col);
}

int adjacent_available_tiles(position_t tile) {
  int available_tile_count = 0;

  if (tile.row > 1 && !is_char(tile.row-1, tile.col, WALL))
    available_tile_count++;

  if (tile.row < ROWS-1 && !is_char(tile.row+1, tile.col, WALL))
    available_tile_count++;

  if (tile.col == 0 || tile.col == COLUMNS-1)
    available_tile_count++;

  if (tile.col > 1 && !is_char(tile.row, tile.col-1, WALL))
    available_tile_count++;

  if (tile.col < COLUMNS-1 && !is_char(tile.row, tile.col+1, WALL))
    available_tile_count++;

  return available_tile_count;
}

position_t scatter_target_tile(char ghost_initial) {
  position_t target_tile;

  switch (ghost_initial) {
    case BLINKY:
    case PINKY:
    case INKY:
    case CLYDE:
    default:
      target_tile.row = 0;
      target_tile.col = COLUMNS-1;
      break;
  }

  return target_tile;
}

position_t closest_tile_to_target(position_t tile_1, position_t tile_2,
                                  position_t tile_3, position_t target)
{
  int dist_1 = distance_squared(tile_1, target);
  int dist_2 = distance_squared(tile_2, target);
  int dist_3 = distance_squared(tile_3, target);

  return (dist_1 < dist_3) ? ((dist_1 < dist_2) ? tile_1 : tile_2)
                           : ((dist_2 < dist_3) ? tile_2 : tile_3);
}

void ghost_change_to_frightened(char ghost_initial) {
  int index = ghost_index(ghost_initial);
  reverse_ghost_direction(ghost_initial);
  game.ghosts[index].behaviour = FRIGHTENED;
  game.ghosts[index].frightened_timer = 0;
}

void ghost_chase_to_scatter(char ghost_initial) {
  reverse_ghost_direction(ghost_initial);
  game.ghosts[ghost_index(ghost_initial)].behaviour = SCATTER;
  game.scatter_counter++;
}

void ghost_scatter_to_chase(char ghost_initial) {
  reverse_ghost_direction(ghost_initial);
  game.ghosts[ghost_index(ghost_initial)].behaviour = CHASE;
}

void reverse_ghost_direction(char ghost_initial) {
  int index = ghost_index(ghost_initial);

  switch (ghost_direction(ghost_initial)) {
    case EAST:
      game.ghosts[index].direction = WEST;
      break;
    case WEST:
      game.ghosts[index].direction = EAST;
      break;
    case SOUTH:
      game.ghosts[index].direction = NORTH;
      break;
    case NORTH:
      game.ghosts[index].direction = SOUTH;
      break;
  }
}

void reset_positions(void) {
  reset_pacman_position();
  reset_ghost_position(BLINKY);
  // reset_ghost_position(PINKY);
  // reset_ghost_position(INKY);
  // reset_ghost_position(CLYDE);
}

void reset_pacman_position(void) {
  game.pacman.position.row = 14;
  game.pacman.position.col = 14;
}

void reset_ghost_position(char ghost_initial) {
  switch (ghost_initial) {
    case BLINKY:
    case PINKY:
    case INKY:
    case CLYDE:
    default:
      game.ghosts[ghost_index(ghost_initial)].position.row =  8;
      game.ghosts[ghost_index(ghost_initial)].position.col = 14;
      break;
  }
}

void reset_directions(void) {
  reset_pacman_direction();
  reset_ghost_direction(BLINKY);
  // reset_ghost_direction(PINKY);
  // reset_ghost_direction(INKY);
  // reset_ghost_direction(CLYDE);
}

void reset_pacman_direction(void) {
  game.pacman.direction = WEST;
}

void reset_ghost_direction(char ghost_initial) {
  switch (ghost_initial) {
    case BLINKY:
    case PINKY:
    case INKY:
    case CLYDE:
    default:
      game.ghosts[ghost_index(ghost_initial)].direction = EAST;
      break;
  }
}

void revive_pacman(void) {
  game.pacman.died = false;
}

void terminate_game_session(char *exit_message) {
  endwin(); // Exits the curses mode
  printf("%s\nScore: %d\n", exit_message, score());
  exit(EXIT_SUCCESS);
}
