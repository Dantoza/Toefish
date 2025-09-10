# Toefish

Toefish is a small Tic-Tac-Toe evaluation bot written in C.  
It can look at a board state and tell you the result or the best move.

---

## Getting Started

### Build

```bash
git clone https://github.com/Dantoza/Toefish.git
cd Toefish
gcc -o toefish toefish.c -lcjson
```

### Run

```bash
./toefish <args> <file path>
```
#### Arguments/flags are:
* -e
* -m
---
## How it works
Toefish uses the minimax algorithm to find the best possible move assuming that two perfect players are playing.
Since tic-tac-toe is a solved game, instead giving a score like stockfish, toefish can predict every move untill the end of the game,
always finding the fastest mate for any player if its possible
## Instructions
### There are 2 modes of how toefish can run:
1. _Evaluate mode:_ finds the fastest way to "mate" and tells you if the game is a forced draw(-e argument/flag when running the program)


2. _Play mode:_ finds the best next move( -m argument/flag when running the program)


---

## Board JSON example

```json
{
"0": "X",
"1": "O",
"2": "X",
"3": "O",
"4": "X",
"5": "O",
"6": "X",
"7": "O",
"8": "_"
}
```
which is the equivalent to: 
``` 
 0 | 1 | 2
---+---+---
 3 | 4 | 5 
---+---+---
 6 | 7 | 8
```
---
## Dependencies
To compile toefish, you need to install the [cJSON](https://github.com/DaveGamble/cJSON), since the code needs to parse the JSON.

### Installing cJSON
For better explanation of how to install the library, please reffer to the [cJSON](https://github.com/DaveGamble/cJSON) repo.
For installing on Windows, you can use CMake,for Linux you can use either CMake or, if supported, install it from your package manager:
#### Debian/Ubuntu-based systems

```bash
sudo apt-get update
sudo apt-get install libcjson-dev
```

#### Arch Linux-based systems

```bash
sudo pacman -S cjson
```

#### Fedora-based systems

```bash
sudo dnf install cjson-devel
```

#### openSUSE-based systems

```bash
sudo zypper install cjson-devel
```

If your distribution does not provide a package/you are using Windows, you can build cJSON from source:

```bash
git clone https://github.com/DaveGamble/cJSON.git
cd cJSON
mkdir build && cd build
cmake ..
make
sudo make install
```
---
## Future goals
- Implement alpha-beta pruning to optimize the code
- Add an addittional option to output the next-move board state into a new file for easier implementation with GUI-s
- Improve the JSON format or switch the format entirely for something better used for smaller amounts of data(possibly removing the dependency on cJSON)
- publishing the program to Linux and Windows repositories such as [https://scoop.sh/](scoop.sh) , apt, AUR,...
## License

This project is under the GPL-3.0 License.  
See the LICENSE file for details.

---

Made by [Dantoza](https://github.com/Dantoza) 🎣
