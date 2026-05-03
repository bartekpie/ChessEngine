


# Backend of ChessEngine

This project uses low level optimalizations, to generate and simulate chess moves, that are based on modern chess engines techniques. It also has algorithms that help simulate moves ahead and based on evaluation funcion say what moves to pick. 



## Building

to build this project you need to run this three commands:

```bash
  mkdir build
  cd build
  cmake ..
  cmake --build .

```

 make sure you have installed cmake.     

## Running program

To run the main program run

```bash
  ./main.exe
```

## Building Tests

To build tests, go into unit test folder and run the following commands

```bash
  mkdir build
  cd build
  cmake ..
  cmake --build .
```
By using ls you can see what executable to run 


## Architecture 

Three main components are generator, position and engine. 

Generatator takes responsibilty for generating legal moves and storing them efficietnly in MoveList data structure.

Position takes responisbilty for storing current position of chessboard and providing simulate and undo move funcions.

Engine uses provided functionality and implements methods to search and evaluate position efficietnly.

## Generator 

Generator uses bits operations on Bitboards(uint64_t) in order to perform those calculations efficiently. 

Knights are the easiest part because we use precompiled tables, that we init through contexpr lambda. Index of a table corresponds to square on a chess board. For example 
a1 would be 0 index, and precompiled_directions[knight][Bitboard::a1] will return as possible knight moves as a bitbaord. Then thorugh & operations we elminate our pieces, because we cannot move knight where our piece stands.

Sliders are the category of figures : queen, rook and bishop. We use similar aproach, the problem is that we cannot go through given blocking piece. We use precompiled table for every direction, and if there is a blocking piece we need to do xor operation with the same table, but with blocking square as and index.

Lets consider given example. We have bishop on c5 and we want to calulate south-east direction. 
When we use precompiled_directions[north_east][Bitboard::c5]
we get :

                  8 | X X X X X X X X 
                  7 | X X X X X X X X 
                  6 | X X X X X X X X 
                  5 | X X X X X X X X 
                  4 | X X X . X X X X 
                  3 | X X X X . X X X 
                  2 | X X X X X . X X 
                  1 | X X X X X X . X 
                      A B C D E F G H
   
On square f2 stands our piece so we cannot go into g1 nad f2. We take blocking square as ~emptySpaces & table above. 
Then we perform get most singnificant bit and it returns blocking square. We can do: 
table above ^ precompiled_directions[north_east][Bitboard::blovcking_piece] to eliminate moves that go through blocking piece.
