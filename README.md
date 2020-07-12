# Pacman

This is a classic pacman game written in C. The game window looks like this:

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

### How to play
The game instructions can be seen in the above image.

### Screen configuration
Two adjacent characters that are located on different lines might be more distant apart than
when they are on the same line, depending on your terminal window configuration. In that case,
the characters (pacman, ghost) will seem to move faster when moving vertically. If that bothers
you, try adjusting the line spacing of your terminal window and select whichever one works best
for you.
