## Chess_v2

A chess bot capable of playing complete games using the minimax algorithm to evaluate possible move sequences and identify the most advantageous option for the engine. The position evaluation function takes into consideration factors like material balance, pawn structure, mobility, trapped pieces and center control.

This project currently does not have a user interface, but is going to be added soon.

**Key Files**

**eval.cpp, eval.h**
contains all relevant evaluation functions to evaluate:
* material balance
* pawn structure
* outposts
* mobility
* trapped pieces
* hanging pieces
* king safety

**functions.cpp, functions.h**
contains auxilliary functions to
* display the board
* convert position from pair<int,int> to string and vice versa
* check if piece belongs to current player
* reverse the board
* check validity of piece positions

**fen.cpp, fen.h**
contains class Board_FEN which has the methods to create a board from a given fen notation, to return fen notation of a given board, to set/return boolean variables for enPassant and castling options.

**moves.cpp, moves.h**
contains class Moves which has the methods to generate possible moves. Some of the key functionalities are to

* generate valid moves for current player and for the opponent
* find the squares controlled by current player and the opponent
* check pinned squares for player and the opponent
* check trapped pieces for player and the opponent

**sq_tables.cpp, sq_tables.h**
struct psTables contains the piece square table values for each piece for midgame and endgame separately. eval_sq_tables evaluates the current advantage to black and white based on the board state and the tables.

**search.cpp, search.h**
This is in a way the "main" file. It contains the search function complete_eval which utilises play_one_move and the other evaluation_functions to determine what move is the best to play, and plays that move.

### How to use?
enter the board in FEN notation in the in.txt file and run the program. The out.txt file will display the board after the next move, the stats and the FEN of the new board state.
https://lichess.org/editor is useful to get FEN values of the move played by human.
