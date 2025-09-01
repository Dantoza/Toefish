# Toefish

Toefish is a small Tic-Tac-Toe evaluation bot written in C.  
It can look at a board state and tell you the result or the best move.

---

## Getting Started

### Build

```bash
git clone https://github.com/Dantoza/Toefish.git
cd Toefish
gcc -o toefish toefish.c
```

### Run

```bash
./toefish
```

*(You can pass a board file — check the code for details.)*

---

## Example

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

Running Toefish on this might say:  
```
Best move: (A,B)
```

---

## License

This project is under the GPL-3.0 License.  
See the LICENSE file for details.

---

Made by [Dantoza](https://github.com/Dantoza) 🎣
