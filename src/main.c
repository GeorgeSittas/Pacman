#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "pacman.h"
#include "screen.h"
#include "utilities.h"

game_t game;

int main(void) {
  srand(time(NULL));
  setup_term();

  // Sets the initial game configurations
  init_game();

  while (true) {
    // Check whether the user wants to change Pacman's direction or quit
    process_input();

    form_next_frame(); // Move each character to it's new position

    // Check if the game is over, in which case the program will terminate
    switch (game_state()) {
      case WON:  terminate_game_session("You win!");
      case LOST: terminate_game_session("You lose!");
      case STILL_PLAYING: break;
    }

    render(); // Print the current frame

    // Whenever pacman dies, a small death delay is applied
    if (pacman_died()) {
      revive_pacman();
      usleep(DEATH_DELAY);
    }
    else
      usleep(FRAME_DELAY);
  }

  return 0;
}
