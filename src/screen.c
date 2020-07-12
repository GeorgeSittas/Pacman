#include <ncurses.h>

#include "pacman.h"
#include "screen.h"
#include "utilities.h"

// Initializes the cursor session settings
void setup_term(void) {
  newterm(NULL, stdout, stdin); // Initializes the screen (curses mode)
  noecho(); // Prevents the input characters from showing up
  curs_set(false); // Makes the cursor invisible
  keypad(stdscr, true); // Enables reading of function keys (eg. arrow keys)
  timeout(1); // Sets an input timer of 1 msec (to keep the snake in motion)
}

void init_screen(void) {
  char screen[ROWS][COLUMNS] = {"############################",
                                "#............##............#",
                                "#*####.#####.##.#####.####*#",
                                "#.####.#####.##.#####.####.#",
                                "#..........................#",
                                "#.####.##.########.##.####.#",
                                "#......##....##....##......#",
                                "######.##### ## #####.######",
                                "     #.##          ##.#     ",
                                "     #.## ######## ##.#     ",
                                "######.## ######## ##.######",
                                "      .   ########   .      ",
                                "######.## ######## ##.######",
                                "     #.## ######## ##.#     ",
                                "     #.##          ##.#     ",
                                "######.## ######## ##.######",
                                "#............##............#",
                                "#.####.#####.##.#####.####.#",
                                "#*..##................##..*#",
                                "###.##.##.########.##.##.###",
                                "#......##....##....##......#",
                                "#.##########.##.##########.#",
                                "#..........................#",
                                "############################"};

  for (int i = 0; i < ROWS; i++)
    for (int j = 0; j < COLUMNS; j++)
      game.screen[i][j] = screen[i][j];
}

void print_info(void) {
  mvprintw(2, COLUMNS + 10, "Pacman game written by George Sittas");
  mvprintw(4, COLUMNS + 10, "Instructions:");
  mvprintw(5, COLUMNS + 10, "1) Use the arrows to move");
  mvprintw(6, COLUMNS + 10, "2) Press p or P to pause the game");
  mvprintw(7, COLUMNS + 10, "3) Press r or R to resume the game");
  mvprintw(8, COLUMNS + 10, "4) Press q or Q to quit the game");
  mvprintw(ROWS/2, COLUMNS + 10, "Remaining lives: %d", lives());
  mvprintw(ROWS/2 + 2, COLUMNS + 10, "Score: %d", score());
}

char pacman_symbol(void) {
  switch(pacman_direction()) {
    case EAST:  return '>';
    case WEST:  return '<';
    case NORTH: return '^';
    case SOUTH: return 'v';
    default: terminate_game_session("Abnormal termination");
  }
}

// Prints the game screen -- a game frame
void render(void) {
  /* mvaddch() forms the frame on a buffer window,
   * before it's printed on the actual game window
   */

  for (int i = 0; i < ROWS; i++)
    for (int j = 0; j < COLUMNS; j++)
      mvaddch(i, j, game.screen[i][j]);

  // The info text, Pacman and the ghosts are printed in bold letters
  attron(A_BOLD);

  mvaddch(pacman_row(), pacman_col(), pacman_symbol());

  // Whenever a ghost is frightened, it will also be underlined

  if (ghost_behaviour(BLINKY) == FRIGHTENED) attron(A_UNDERLINE);
  mvaddch(ghost_row(BLINKY), ghost_col(BLINKY), BLINKY);
  attroff(A_UNDERLINE);

  // if (ghost_behaviour(PINKY) == FRIGHTENED) attron(A_UNDERLINE);
  // mvaddch(ghost_row(PINKY), ghost_col(PINKY), PINKY);
  // attroff(A_UNDERLINE);

  // if (ghost_behaviour(INKY) == FRIGHTENED) attron(A_UNDERLINE);
  // mvaddch(ghost_row(INKY), ghost_col(INKY), INKY);
  // attroff(A_UNDERLINE);

  // if (ghost_behaviour(CLYDE) == FRIGHTENED) attron(A_UNDERLINE);
  // mvaddch(ghost_row(CLYDE), ghost_col(CLYDE), CLYDE);
  // attroff(A_UNDERLINE);

  print_info();

  attroff(A_BOLD);

  // Prints the buffer window's contents to the game window
  refresh();
}
