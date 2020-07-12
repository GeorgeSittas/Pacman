#pragma once

#include "pac_types.h"

#define MAX_SCORE 214 // There are 214 food pieces in the map

extern game_t game;

void init_game(void);
void init_pacman(void);
void init_ghost(char);

void process_input(void);
void form_next_frame(void);

void advance_pacman(void);
void advance_ghost(char);
void advance_blinky(void);

void advance_frightened_ghost(char); // Only used at intersection tiles
void advance_non_intersection(char); // Advances ghosts on non intersection tiles

state_t game_state(void);
