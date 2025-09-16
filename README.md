# Chess Engine v2

A C++ chess engine implementing minimax algorithm with alpha-beta pruning for intelligent move selection.

## Features

- **Complete Chess Engine**: Plays full games with legal move generation
- **Advanced Evaluation**: Multi-factor position assessment including material, pawn structure, mobility, and king safety
- **Search Algorithm**: Minimax with alpha-beta pruning for optimal move selection
- **FEN Support**: Standard chess notation for position input/output
- **Interactive Play**: Human vs computer gameplay mode

## Core Components

### `pieces.h`
- `Piece` class with position tracking and pin detection

### `fen.h` 
- `Board_FEN` class for FEN notation parsing and board state management

### `moves.h`
- `Moves` class for legal move generation and square control analysis

### `eval.h`
- Position evaluation functions:
  - Material balance
  - Pawn structure and outposts
  - Mobility and king safety
  - Trapped/hanging piece detection

### `search.h`
- `EvalBar` class implementing minimax search with alpha-beta pruning
- Game phase-specific evaluation weights

### `sq_tables_eval.h`
- Piece-square tables for positional scoring

### `functions.h`
- Utility functions for board manipulation and coordinate conversion

## Usage

1. **Interactive Game**: Run `test.cpp` for human vs computer play
2. **Position Analysis**: Use `positionTesting()` for detailed evaluation breakdown
3. **FEN Input**: Provide positions in standard FEN notation

## Build

```bash
make
./test
```

## Evaluation Factors

- Material balance (piece values)
- Pawn structure and outposts
- Piece mobility and control
- King safety assessment
- Trapped/hanging piece penalties
- Position-based piece-square tables

The engine automatically adjusts evaluation weights based on game phase (opening/middlegame/endgame) for optimal performance.
