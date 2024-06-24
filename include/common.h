
#ifndef COMMON_H
#define COMMON_H

#include "raylib.h"
#include <stdlib.h>         // Required for: calloc(), free()
#include <queue>
#include <deque>
#include <iostream>


#include <random>


#define STORAGE_DATA_FILE   "hiscore.data"   // Storage file
struct intVector2{
    int x, y;
};

typedef enum {
    STORAGE_POSITION_HISCORE      = 0
} StorageData;

//Cell size and count: The entire grid
inline int cellSize = 16; //What is the size of the cell
inline int playableCellCount = 16; //How many cells in width/height is playable
inline intVector2 totalCellCount = {50, 50}; //How many total cells, first is width, second is height
inline intVector2 playableCellPos = {totalCellCount.x/2 - playableCellCount/2, totalCellCount.y/2 - playableCellCount/2};


#endif