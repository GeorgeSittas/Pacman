#pragma once

// FPS: number of frames per second (basically the frame printing frequency)
// DELAY: time delay needed to achieve the specified FPS (in micro seconds)

#define SEC_IN_MICROSEC 1000000

#define FPS 8 // Frame printing frequency

#define FRAME_DELAY (SEC_IN_MICROSEC / FPS) // time = 1 (sec) / frequency
#define DEATH_DELAY (2*SEC_IN_MICROSEC) // 2 sec delay every time Pacman dies

#define SCATTER_LIMIT       4 // Scatter mode can occur only 4 times
#define SCATTER_DURATION    3 // Scatter mode lasts 3 seconds
#define FRIGHTENED_DURATION 8 // Frightened mode lasts 8 seconds
#define SCATTER_INTERVAL   15 // Scatter mode is activated every 15 seconds

#include "pac_types.h"

bool pacman_died(void);
int pacman_row(void);
int pacman_col(void);
position_t pacman_position(void);
direction_t pacman_direction(void);

int ghost_index(char);
int ghost_row(char);
int ghost_col(char);
position_t ghost_position(char);
direction_t ghost_direction(char);
behaviour_t ghost_behaviour(char);
void ghost_heuristic(char);

int score(void);
int lives(void);
int scatter_counter(void);

bool pacman_eaten(void);
bool ghost_eaten(char);
bool is_char(int, int, char);
bool is_intersection(position_t);

int distance_squared(position_t, position_t);
int adjacent_available_tiles(position_t);

position_t scatter_target_tile(char);
position_t closest_tile_to_target(position_t, position_t,
                                  position_t, position_t);

void ghost_change_to_frightened(char);
void ghost_chase_to_scatter(char);
void ghost_scatter_to_chase(char);
void reverse_ghost_direction(char);
void reset_positions(void);
void reset_pacman_position(void);
void reset_ghost_position(char);
void reset_directions(void);
void reset_pacman_direction(void);
void reset_ghost_direction(char);
void revive_pacman(void);
void terminate_game_session(char *);
