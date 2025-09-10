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

## License

This project is under the GPL-3.0 License.  
See the LICENSE file for details.

---

Made by [Dantoza](https://github.com/Dantoza) 🎣
