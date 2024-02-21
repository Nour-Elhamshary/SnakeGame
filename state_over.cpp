#include "include/states.h"
#include "include/raygui.h"

bool finishOver = false;
bool returnToTitle = false;


Vector2 buttonSizeOver = {12, 3};
Rectangle restartButton = {((totalCellCount.x/2)-(buttonSizeOver.x/2))*cellSize, ((totalCellCount.y/2)-buttonSizeOver.y)*cellSize, buttonSizeOver.x*cellSize, buttonSizeOver.y*cellSize};
Rectangle titleButton = {((totalCellCount.x/2)-(buttonSizeOver.x/2))*cellSize, ((totalCellCount.y/2))*cellSize, buttonSizeOver.x*cellSize, buttonSizeOver.y*cellSize};

void InitOverState(){
    finishOver = false;
    returnToTitle = false;
}
void UpdateOverState(){
    if (GuiButton(restartButton, "Restart the game")) { finishOver = true; }
    else if (GuiButton(titleButton, "Return to title")) { returnToTitle = true;}
}

void DrawOverState(){
    DrawText("Game over!!", 190, 200, 20, LIGHTGRAY);
    GuiButton(restartButton, "Restart the game");
    GuiButton(titleButton, "Return to title");
}

bool FinishOverState(){
    return finishOver;
}

bool ReturnToTitleFromOverState(){
    return returnToTitle;
}