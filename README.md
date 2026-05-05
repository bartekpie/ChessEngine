


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

To build tests, go into given unit test folder and run the following commands

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
a1 would be 0 index, and precompiled_directions[knight][Bitboard::a1] will return as possible knight moves as a bitbaord. Then thorugh & operations we elminate our pieces, because we cannot move knight where our piece stands. We also use the same aproach for king moves.

Sliders are the category of figures : queen, rook and bishop. We use similar aproach, the problem is that we cannot go through given blocking piece. We use precompiled table for every direction, and if there is a blocking piece we need to do xor operation with the same table, but with blocking square as and index.

Lets consider given example. We have bishop on c5 and we want to calulate south-east direction. Lets assume that on square f2 stands our piece so we cannot go into f2 and g1.

When we use precompiled_directions[north_east][Bitboard::c5]
we get Bitboard A.

We take blocking square as ~emptySpaces and Bitboard A and perform mostsignificantbit operation and we get f2 as blocking square.

Then we can get bitboard B as precompiled_directions[north_east][blocking_square].

By performing xor operation  A ^ B we get Bitboard C, which is result where we cannot go through figures.

        A :                                    B :                                     C:                                         
        8 | X X X X X X X X                    8 | X X X X X X X X                      8 | X X X X X X X X  
        7 | X X X X X X X X                    7 | X X X X X X X X                      7 | X X X X X X X X  
        6 | X X X X X X X X                    6 | X X X X X X X X                      6 | X X X X X X X X  
        5 | X X X X X X X X                    5 | X X X X X X X X                      5 | X X X X X X X X  
        4 | X X X . X X X X                    4 | X X X X X X X X                      4 | X X X . X X X X  
        3 | X X X X . X X X                    3 | X X X X X X X X                      3 | X X X X . X X X  
        2 | X X X X X . X X                    2 | X X X X X . X X                      2 | X X X X X X X X  
        1 | X X X X X X . X                    1 | X X X X X X . X                      1 | X X X X X X X X  
            A B C D E F G H                        A B C D E F G H                          A B C D E F G H 
   
Pawn implementation is harder, because pawns have a lot of rules. Single push for white pawns we use bit shift operation by doing pawns << 8, and double push pawns on starting row << 16. We use bit shift operation fot calculating pawn captures through funcions get long and short offset attacks, where we shift by 7 and by 9. To implement right directrions I used templates with if contexpr to adjust right shift operators (<<, >>);

I also implemented enPassant, by knowing whether opponent double pushed in his last move. The position class provides that. 

All above are just pseudo legal moves, because in reality we know that if we are in check we cannot move pieces pseudo legally, but we have to either hide the king from check or escape with the king. 

generate_all_moves provides that by building the contex of the position, and manages other funcions in the way that provides only legal moves. 

## Move 
Move is uint16_t where first 6 bits are information about from square. Next 6 bits are info about to square and last 4 bits keep track of type of the move. We have standard move, capture, castling, en passant, promotion. Its important in simulating and undoing moves to know what kind of move is that actually. Moves clas provides nice api like from(), to(), type() in order to use encoding easily.

## Position 

In position class we store bitboard for every piece in the array. We also store bitboard for every color and emptySpaces for more clear and faster move generation. We also store more info manager where we keep track of addintional information in the heap. The information are in structure more info. During the runtime of search we do not allocate the memory. We reserve enougth in the constructor so it only happens once.

In simulate move we depending on the move perform setting and reseting right bits in Bitboard above. We also save more info in order to easily reverse the move.

In undo move we reverse simulate funcion and decrease index in more info manager. 

## Engine 

In engine we have standard nega max algorithm where we search for a given depth. 

We also have iterativeDeepening so we start from depth 1 and end in depth of 10. This way we improving the move as the time flies, but also are ready to provide move in short term search. 

In future I will implemenat multi-threading in search function, provide UCI interface and change primitive evaluation function to the self trainded neural network. 
