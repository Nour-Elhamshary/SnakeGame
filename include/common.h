
#ifndef COMMON_H
#define COMMON_H

#include "raylib.h"
#include <random>



struct intVector2{
    int x, y;
};



//Cell size and count: The entire grid
inline int cellSize = 16; //What is the size of the cell
inline int playableCellCount = 16; //How many cells in width/height is playable
inline intVector2 totalCellCount = {50, 50}; //How many total cells, first is width, second is height
inline intVector2 playableCellPos = {totalCellCount.x/2 - playableCellCount/2, totalCellCount.y/2 - playableCellCount/2};



//Game states: An enumerator that starts from 0 and tells the state of the game.
enum GameStates {
    unknown = -1, //Unknown state, good idea to have when needed.
    logostate = 0, //Logo state, for logo stuff.
    titlestate, //Title state, used when the game is in the title screen
    gamestate, //Game state, basically when the game is currently playing
    pausestate, //Pause state, just, erm, pause?
    overstate //Over state, game over.
};

inline GameStates currentGameState = logostate; //Current state of the game.

#endif