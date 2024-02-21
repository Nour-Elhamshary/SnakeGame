#include "include/states.h"

#define RAYGUI_IMPLEMENTATION
#include "include/raygui.h"

bool FinishState = false;

Vector2 buttonSize = {12, 3};
Rectangle normalButton = {((totalCellCount.x/2)-(buttonSize.x/2))*cellSize, ((totalCellCount.y/2)-buttonSize.y)*cellSize, buttonSize.x*cellSize, buttonSize.y*cellSize};
Rectangle hardButton = {((totalCellCount.x/2)-(buttonSize.x/2))*cellSize, ((totalCellCount.y/2))*cellSize, buttonSize.x*cellSize, buttonSize.y*cellSize};

void InitTitleState(){
    FinishState = false;
}


void UpdateTitleState() {
    if (GuiButton(normalButton, "Normal")) {
        playableCellCount = 32;
        playableCellPos = {totalCellCount.x/2 - playableCellCount/2, totalCellCount.y/2 - playableCellCount/2}; 
        FinishState = true;
    }
    else if (GuiButton(hardButton, "Hard")) {
        playableCellCount = 16; 
        playableCellPos = {totalCellCount.x/2 - playableCellCount/2, totalCellCount.y/2 - playableCellCount/2}; 
        FinishState = true;
    }
}

void DrawTitleState() {
        DrawText("Welcome to snek game!", 190, 200, 20, LIGHTGRAY);
        GuiButton(normalButton, "Normal");
        GuiButton(hardButton, "Hard");
}

bool FinishTitleState(){
    return FinishState;
}