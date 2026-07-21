# CHESS Engine and GUI
My love for chess inspired me to build a fully playable two-player chess engine along with its pizel-art Graphical User Interface built in C++ with SFML. The project began in my first semester and continued into my second, requiring significant efforts and time, but I am proud of the final result and the knowledge I gained throughout the development process.
_ _ _
## 🎮Demo



_ _ _
## ✨Features

> Core Chess Rules
- Display of Full legal-move generation for all six pieces(pawn, rook, knight, bishop, queen, king)
- Check, checkmate, and stalemate detection
- Pin detection, discovered check and double check handling
- illegal moves are filtered out
- Castling (kingside & queenside) with checks for attacked squares and blocked paths
- En passant capture
- Pawn promotion with an in-game popup
- Draw detection

> Interface & Visuals 
- Main menu and dedicated Options screen, both image-based with clickable toggles and buttons
- Move highlighting: selected piece, legal move dots, capture rings, last-move squares (lichess-style), and a red highlight on a king in check with specified sound effects
- Optional board rotation each turn (view flips to face the current player, like a real board, can commanly be observed in `chess.com`)
- Turn indicator and a game-over overlay with result messages

> Quality-of-Life
- Undo move (press Z) with full state restoration (board, turn, castling rights, en passant state, move number)
- Move history logged in algebraic notation, printed to console/terminal
- Configurable toggles: music, sound effects, board rotation, and undo, all avaialable in Options menu


_ _ _
