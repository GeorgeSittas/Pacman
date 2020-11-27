# Pacman

Goal for this project is to make a (simplified) [Pacman](https://en.wikipedia.org/wiki/Pac-Man) game
in C. The game window looks like this:

<img src="https://imgur.com/azLBIPS.png" width="650">

### Install
```
git clone https://github.com/GeorgeSittas/Pacman.git
```
### Prerequisites / Dependencies
This project uses the NCurses C library, so you need to make sure it's already installed in your system.

([how to install the NCurses library in Linux](https://www.ostechnix.com/how-to-install-ncurses-library-in-linux/))

### Usage
#### Starting the game
```
cd Pacman
make play
```
#### File cleanup
```
cd Pacman
make clean
```

### How to play
The game instructions can be seen in the above image. Note that Pacman is represented by an
arrow character (<, >, ^, v), while a ghost is represented by the character B (for "Blinky").

### Screen configuration
Two adjacent characters that are located on different lines might be more distant apart than
when they are on the same line, depending on your terminal window configuration. In that case,
the characters (pacman, ghost) will seem to move faster when moving vertically. If that bothers
you, try adjusting the line spacing of your terminal window and select whichever one works best
for you.

### Notes
In the current version of this project, there's only one ghost chasing pacman, the ghost "Blinky".
There are some lines in the code that've been left commented out on purpose, so that addition of
other ghosts can be done easily & fast, by uncommenting these lines.
