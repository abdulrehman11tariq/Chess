# Chess Engine and GUI
My passion for chess inspired me to build a fully playable two-player chess engine along with its custom pixel-art Graphical User Interface using C++ with SFML. The project began in my first semester and continued into my second, requiring significant time, effort, and persistence. I am proud of the final result and the knowledge I gained throughout the development process.

---

## 🎮 Demo



---

## ✨ Features

### Core Chess Rules
- Complete legal-move generation for all six pieces: pawn, rook, knight, bishop, queen, king
- Check, checkmate, and stalemate detection
- Pin detection, discovered check and double check handling
- Automatic prevention of illegal moves
- Castling (kingside & queenside) with checks for attacked squares and blocked paths
- En passant capture
- Pawn promotion with an in-game popup
- Draw detection
 
### Interface & Visuals 
- Main menu and dedicated Options screen, both image-based with clickable toggles and buttons
- Move highlighting: selected piece, legal move dots, capture rings, last-move squares (lichess-style), and a red highlight on a king in check, all accompanied by sound effects
- Optional board rotation each turn (view flips to face the current player, like a real board)
- Turn indicator and a game-over overlay with result messages

### Quality-of-Life
- Undo move (press Z) with full state restoration (board, turn, castling rights, en passant state, move number)
- Move history logged in algebraic notation, printed to console/terminal
- Configurable toggles: music, sound effects, board rotation, and undo, all available in Options menu

---

## 🛠️ Technologies Used

- C++
- SFML

---

## 🕹️ Controls
 
| Action              | Input                            |
|---------------------|----------------------------------|
| Select / move piece | Left mouse click                 |
| Undo last move      | Using `Z`                        |
| Quit                | Using `Esc`                      |
| Promotion choice    | Clickable popup |
 
---

## 🚀 How to Run

1. Clone the repository:

```Bash
git clone https://github.com/abdulrehman11tariq/Chess
```
2. Open `Chess.slnx` in Visual Studio 2022.

3. Build and run the project.

The required SFML libraries are included in this repository.

--- 

## ⚠️ Limitations
 
- Font loading path is hardcoded to Windows (`C:/Windows/Fonts/...`) — won't work on Linux/macOS without changes

 
