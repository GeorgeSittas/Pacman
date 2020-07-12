#include <ncurses.h>
#include <stdlib.h>

#include "pacman.h"
#include "utilities.h"
#include "screen.h"

static int timer_for_next_scatter = 0;

void init_game(void) {
  game.score           = 0;
  game.lives           = 3;
  game.scatter_counter = 1; // Ghosts begin in scatter mode

  init_screen();
  init_pacman();
  init_ghost(BLINKY);
  // init_ghost(PINKY);
  // init_ghost(INKY);
  // init_ghost(CLYDE);
}

void init_pacman(void) {
  game.pacman.died         = false;
  game.pacman.direction    = WEST;
  game.pacman.position.row = 14;
  game.pacman.position.col = 14;
}

void init_ghost(char ghost_initial) {
  int index = ghost_index(ghost_initial);
  game.ghosts[index].behaviour = SCATTER;

  switch (ghost_initial) {
    case BLINKY:
    case PINKY:
    case INKY:
    case CLYDE:
    default:
      game.ghosts[index].direction    = EAST;
      game.ghosts[index].position.row =  8;
      game.ghosts[index].position.col = 14;
      game.ghosts[index].heuristic    = advance_blinky;
      break;
  }
}

/* Checks whether the user wants to change Pacman's direction
 * of movement, quit or pause/resume the game
 * Note: if Pacman is moving next to a wall, then attempting to change
 * his direction towards the wall will fail to alter his direction
 */

void process_input(void) {
  switch (getch()) {
    case KEY_LEFT:
      if (pacman_col() - 1 >= 0
      && !is_char(pacman_row(), pacman_col()-1, WALL))
        game.pacman.direction = WEST;
      break;

    case KEY_RIGHT:
      if (pacman_col() + 1 < COLUMNS
      && !is_char(pacman_row(), pacman_col()+1, WALL))
      game.pacman.direction = EAST;
      break;

    case KEY_UP:
      if (pacman_row() - 1 > 0
      && !is_char(pacman_row()-1, pacman_col(), WALL))
      game.pacman.direction = NORTH;
      break;

    case KEY_DOWN:
      if (pacman_row() + 1 < ROWS-1
      && !is_char(pacman_row()+1, pacman_col(), WALL))
      game.pacman.direction = SOUTH;
      break;

    case 'P':
    case 'p':
      while (1) {
        int ch = getch();
        if (ch == 'R' || ch == 'r') break;
        if (ch == 'Q' || ch == 'q') terminate_game_session("Ciao!");
      }

      break;

    case 'Q':
    case 'q':
      terminate_game_session("Ciao!");
  }
}

void form_next_frame(void) {
  advance_pacman();
  advance_ghost(BLINKY);
  // advance_ghost(PINKY);
  // advance_ghost(INKY);
  // advance_ghost(CLYDE);
}

// Moves pacman to his next position
void advance_pacman(void) {
  switch(pacman_direction()) {
    case EAST:
      if (pacman_col() == COLUMNS-1)
        game.pacman.position.col = 0; // Teleports to opposite side
      else if (!is_char(pacman_row(), pacman_col()+1, WALL)) {
        if (is_char(pacman_row(), pacman_col()+1, FOOD)
         || is_char(pacman_row(), pacman_col()+1, ENERGIZER))
        {
          if (is_char(pacman_row(), pacman_col()+1, ENERGIZER)) {
            ghost_change_to_frightened(BLINKY);
            // ghost_change_to_frightened(PINKY);
            // ghost_change_to_frightened(INKY);
            // ghost_change_to_frightened(CLYDE);
          }

          game.score++;
          game.screen[pacman_row()][pacman_col()+1] = EMPTY;
        }

        game.pacman.position.col++;
      }

      break;

    case WEST:
      if (pacman_col() == 0) {
        game.pacman.position.col = COLUMNS-1; // Teleports to opposite side
      }
      else if (!is_char(pacman_row(), pacman_col()-1, WALL)) {
        if (is_char(pacman_row(), pacman_col()-1, FOOD)
         || is_char(pacman_row(), pacman_col()-1, ENERGIZER))
        {
          if (is_char(pacman_row(), pacman_col()-1, ENERGIZER)) {
            ghost_change_to_frightened(BLINKY);
            // ghost_change_to_frightened(PINKY);
            // ghost_change_to_frightened(INKY);
            // ghost_change_to_frightened(CLYDE);
          }

          game.score++;
          game.screen[pacman_row()][pacman_col()-1] = EMPTY;
        }

        game.pacman.position.col--;
      }

      break;

    case NORTH:
      if (!is_char(pacman_row()-1, pacman_col(), WALL)) {
        if (is_char(pacman_row()-1, pacman_col(), FOOD)
         || is_char(pacman_row()-1, pacman_col(), ENERGIZER))
        {
          if (is_char(pacman_row()-1, pacman_col(), ENERGIZER)) {
            ghost_change_to_frightened(BLINKY);
            // ghost_change_to_frightened(PINKY);
            // ghost_change_to_frightened(INKY);
            // ghost_change_to_frightened(CLYDE);
          }

          game.score++;
          game.screen[pacman_row()-1][pacman_col()] = EMPTY;
        }

        game.pacman.position.row--;
      }

      break;

    case SOUTH:
      if (!is_char(pacman_row()+1, pacman_col(), WALL)) {
        if (is_char(pacman_row()+1, pacman_col(), FOOD)
         || is_char(pacman_row()+1, pacman_col(), ENERGIZER))
        {
          if (is_char(pacman_row()+1, pacman_col(), ENERGIZER)) {
            ghost_change_to_frightened(BLINKY);
            // ghost_change_to_frightened(PINKY);
            // ghost_change_to_frightened(INKY);
            // ghost_change_to_frightened(CLYDE);
          }

          game.score++;
          game.screen[pacman_row()+1][pacman_col()] = EMPTY;
        }

        game.pacman.position.row++;
      }

      break;

    default: terminate_game_session("Abnormal termination");
  }

  if (pacman_eaten()) {
    game.pacman.died = true;
    game.lives--;

    reset_positions();
    reset_directions();
  }
  else if (ghost_eaten(BLINKY)) {
    reset_ghost_direction(BLINKY);
    reset_ghost_position(BLINKY);
    game.ghosts[ghost_index(BLINKY)].behaviour = CHASE;
  }
  // else if (ghost_eaten(PINKY)) {
  //   reset_ghost_direction(PINKY);
  //   reset_ghost_position(PINKY);
  //   game.ghosts[ghost_index(PINKY)].behaviour = CHASE;
  // }
  // else if (ghost_eaten(INKY)) {
  //   reset_ghost_direction(INKY);
  //   reset_ghost_position(INKY);
  //   game.ghosts[ghost_index(INKY)].behaviour = CHASE;
  // }
  // else if (ghost_eaten(CLYDE)) {
  //   reset_ghost_direction(CLYDE);
  //   reset_ghost_position(CLYDE);
  //   game.ghosts[ghost_index(CLYDE)].behaviour = CHASE;
  // }
}

void advance_ghost(char ghost_initial) {
  ghost_heuristic(ghost_initial);
}

void advance_blinky(void) {
  if (pacman_died()) return;

  int g_row = ghost_row(BLINKY);
  int g_col = ghost_col(BLINKY);
  int index = ghost_index(BLINKY);

  static int scatter_timer = 0;

  // Check if Blinky's behaviour needs to change
  if (ghost_behaviour(BLINKY) == SCATTER
   && scatter_timer++ > FPS*SCATTER_DURATION)
  {
    ghost_scatter_to_chase(BLINKY);
    timer_for_next_scatter = 0;
  }
  else if (ghost_behaviour(BLINKY) == CHASE
        && scatter_counter() < SCATTER_LIMIT
        && timer_for_next_scatter++ > FPS*SCATTER_INTERVAL)
  {
    ghost_chase_to_scatter(BLINKY);
    scatter_timer = 0;
  }
  else if (ghost_behaviour(BLINKY) == FRIGHTENED
        && game.ghosts[index].frightened_timer++ > FPS*FRIGHTENED_DURATION)
  {
    game.ghosts[index].behaviour = CHASE;
    timer_for_next_scatter = 0;
  }

  // Possible next positions for Blinky
  position_t candidate_1, candidate_2, candidate_3;
  candidate_1 = candidate_2 = candidate_3 = ghost_position(BLINKY);

  if (is_intersection(ghost_position(BLINKY))) {
    if (ghost_behaviour(BLINKY) == FRIGHTENED) {
      advance_frightened_ghost(BLINKY);
      return;
    }

    switch(ghost_direction(BLINKY)) {
      case EAST: // Check which of the north, south and east tiles are empty
        if (g_row > 1 && !is_char(g_row-1, g_col, WALL))
          candidate_1.row--;

        if (g_row < ROWS-1 && !is_char(g_row+1, g_col, WALL)) {
          if (candidate_1.row == g_row) candidate_1.row++;
          else candidate_2.row++;
        }

        if (g_col < COLUMNS-1 && !is_char(g_row, g_col+1, WALL)) {
          if (candidate_2.row == g_row) candidate_2.col++;
          else candidate_3.col++;
        }

        break;

      case WEST: // Check which of the north, south and west tiles are empty
        if (g_row > 1 && !is_char(g_row-1, g_col, WALL))
          candidate_1.row--;

        if (g_row < ROWS-1 && !is_char(g_row+1, g_col, WALL)) {
          if (candidate_1.row == g_row) candidate_1.row++;
          else candidate_2.row++;
        }

        if (g_col > 1 && !is_char(g_row, g_col-1, WALL)) {
          if (candidate_2.row == g_row) candidate_2.col--;
          else candidate_3.col--;
        }

        break;

      case NORTH: // Check which of the north, west and east tiles are empty
        if (g_col > 1 && !is_char(g_row, g_col-1, WALL))
          candidate_1.col--;

        if (g_row > 1 && !is_char(g_row-1, g_col, WALL)) {
          if (candidate_1.col == g_col) candidate_1.row--;
          else candidate_2.row--;
        }

        if (g_col < COLUMNS-1 && !is_char(g_row, g_col+1, WALL)) {
          if (candidate_2.row == g_row) candidate_2.col++;
          else candidate_3.col++;
        }

        break;

      case SOUTH: // Check which of the south, west and east tiles are empty
        if (g_col > 1 && !is_char(g_row, g_col-1, WALL))
          candidate_1.col--;

        if (g_row < ROWS-1 && !is_char(g_row+1, g_col, WALL)) {
          if (candidate_1.col == g_col) candidate_1.row++;
          else candidate_2.row++;
        }

        if (g_col < COLUMNS-1 && !is_char(g_row, g_col+1, WALL)) {
          if (candidate_2.row == g_row) candidate_2.col++;
          else candidate_3.col++;
        }

        break;

      default: terminate_game_session("Abnormal termination");
    }

    if (candidate_3.row == g_row && candidate_3.col == g_col)
      candidate_3 = candidate_2;

    position_t target_tile = (ghost_behaviour(BLINKY) == SCATTER)
                             ? scatter_target_tile(BLINKY)
                             : pacman_position();

    game.ghosts[index].position = closest_tile_to_target(candidate_1,
                                                         candidate_2,
                                                         candidate_3,
                                                         target_tile);

    if (ghost_row(BLINKY) == g_row + 1 && ghost_direction(BLINKY) != NORTH)
      game.ghosts[index].direction = SOUTH;
    else if (ghost_row(BLINKY) == g_row - 1 && ghost_direction(BLINKY) != SOUTH)
      game.ghosts[index].direction = NORTH;
    else if (ghost_col(BLINKY) == g_col + 1 && ghost_direction(BLINKY) != WEST)
      game.ghosts[index].direction = EAST;
    else if (ghost_direction(BLINKY) != EAST)
      game.ghosts[index].direction = WEST;
  }
  else
    advance_non_intersection(BLINKY);

  if (pacman_eaten()) {
    game.pacman.died = true;
    game.lives--;

    reset_positions();
    reset_directions();
  }
}

void advance_frightened_ghost(char ghost_initial) {
  int index = ghost_index(ghost_initial);
  int g_row = ghost_row(ghost_initial);
  int g_col = ghost_col(ghost_initial);

  int random_direction = rand() % 4;
  switch (random_direction) {
    case NORTH:
      if (ghost_direction(ghost_initial) != SOUTH
      &&  g_row > 0 && !is_char(g_row-1, g_col, WALL))
      {
        game.ghosts[index].position.row--;
        game.ghosts[index].direction = NORTH;
      }

      break;

    case WEST:
      if (ghost_direction(ghost_initial) != EAST
      &&  g_col > 0 && !is_char(g_row, g_col-1, WALL))
      {
        game.ghosts[index].position.col--;
        game.ghosts[index].direction = WEST;
      }

      break;

    case SOUTH:
      if (ghost_direction(ghost_initial) != NORTH
      &&  g_row < ROWS-1 && !is_char(g_row+1, g_col, WALL))
      {
        game.ghosts[index].position.row++;
        game.ghosts[index].direction = SOUTH;
      }

      break;

    case EAST:
      if (ghost_direction(ghost_initial) != WEST
      &&  g_col < COLUMNS-1 && !is_char(g_row, g_col+1, WALL))
      {
        game.ghosts[index].position.col++;
        game.ghosts[index].direction = EAST;
      }

      break;

    default: terminate_game_session("Abnormal termination");
  }

  if (ghost_eaten(ghost_initial)) {
    reset_ghost_direction(ghost_initial);
    reset_ghost_position(ghost_initial);
    game.ghosts[index].behaviour = CHASE;
  }
}

void advance_non_intersection(char ghost_initial) {
  int index = ghost_index(ghost_initial);
  int g_row = ghost_row(ghost_initial);
  int g_col = ghost_col(ghost_initial);

  switch(ghost_direction(ghost_initial)) {
    case EAST:
      if (g_col == COLUMNS-1)
        game.ghosts[index].position.col = 0;
      else if (g_col < COLUMNS-1 && !is_char(g_row, g_col+1, WALL))
        game.ghosts[index].position.col++;
      else if (g_row > 1 && !is_char(g_row-1, g_col, WALL)) {
        game.ghosts[index].direction = NORTH;
        game.ghosts[index].position.row--;
      }
      else {
        game.ghosts[index].direction = SOUTH;
        game.ghosts[index].position.row++;
      }

      break;

    case WEST:
      if (g_col == 0)
        game.ghosts[index].position.col = COLUMNS-1;
      else if (g_col > 0 && !is_char(g_row, g_col-1, WALL))
        game.ghosts[index].position.col--;
      else if (g_row > 1 && !is_char(g_row-1, g_col, WALL)) {
        game.ghosts[index].direction = NORTH;
        game.ghosts[index].position.row--;
      }
      else {
        game.ghosts[index].direction = SOUTH;
        game.ghosts[index].position.row++;
      }

      break;

    case NORTH:
      if (g_row > 0 && !is_char(g_row-1, g_col, WALL))
        game.ghosts[index].position.row--;
      else if (g_col < COLUMNS-1 && !is_char(g_row, g_col+1, WALL)) {
        game.ghosts[index].direction = EAST;
        game.ghosts[index].position.col++;
      }
      else if (g_col > 1 && !is_char(g_row, g_col-1, WALL)) {
        game.ghosts[index].direction = WEST;
        game.ghosts[index].position.col--;
      }

      break;

    case SOUTH:
      if (g_row < ROWS-1 && !is_char(g_row+1, g_col, WALL))
        game.ghosts[index].position.row++;
      else if (g_col < COLUMNS-1 && !is_char(g_row, g_col+1, WALL)) {
        game.ghosts[index].direction = EAST;
        game.ghosts[index].position.col++;
      }
      else if (g_col > 1 && !is_char(g_row, g_col-1, WALL)) {
        game.ghosts[index].direction = WEST;
        game.ghosts[index].position.col--;
      }

      break;

    default: terminate_game_session("Abnormal termination");
  }
}

state_t game_state(void) {
  if (lives() == 0)         return LOST;
  if (score() == MAX_SCORE) return WON;

  return STILL_PLAYING;
}
