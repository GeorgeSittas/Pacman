#pragma once

#include <ncurses.h>

// Character representations of the fixed grid entities
#define WALL      '#'
#define FOOD      '.'
#define EMPTY     ' '
#define ENERGIZER '*'

// Character representations of the four ghosts
#define BLINKY 'B'
#define PINKY  'P'
#define INKY   'I'
#define CLYDE  'C'

typedef enum {STILL_PLAYING, WON, LOST} state_t;
typedef enum {NORTH, EAST, SOUTH, WEST} direction_t;
typedef enum {CHASE, SCATTER, FRIGHTENED} behaviour_t;

typedef struct position {
  int row;
  int col;
} position_t;

typedef struct pacman {
  bool died;
  position_t position;
  direction_t direction;
} pacman_t;

typedef void (*heuristic_t)(void);

typedef struct ghost {
  int frightened_timer;
  position_t position;
  direction_t direction;
  behaviour_t behaviour;
  heuristic_t heuristic; // Each ghost chooses the target tile differently
} ghost_t;

#define ROWS    24
#define COLUMNS 28

typedef struct game {
  int score;
  int lives;
  int scatter_counter; // Number of times scatter mode has been activated
  pacman_t pacman;
  ghost_t ghosts[4];
  char screen[ROWS][COLUMNS];
} game_t;
