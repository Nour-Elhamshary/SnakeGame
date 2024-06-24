

#include "common.h"

#include "snake.h"
#include "wall.h"
#include "fruit.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

//--------------------------------------
// Region: Game State Functions
//--------------------------------------

Snake player;
Wall wall;
Fruit apple;

std::vector<Wall> setOfWalls;


double timer = 0.0;
bool pausegame = false;
bool finishGame = false;

int score = 0;
int hiscore = 0;

// [Taken directly from raylib's storage example]
// Save integer value to storage file (to defined position)
// NOTE: Storage positions is directly related to file memory layout (4 bytes each integer)
bool SaveStorageValue(unsigned int position, int value)
{
    bool success = false;
    unsigned int dataSize = 0;
    unsigned int newDataSize = 0;
    unsigned char *fileData = LoadFileData(STORAGE_DATA_FILE, &dataSize);
    unsigned char *newFileData = NULL;

    if (fileData != NULL)
    {
        if (dataSize <= (position*sizeof(int)))
        {
            // Increase data size up to position and store value
            newDataSize = (position + 1)*sizeof(int);
            newFileData = (unsigned char *)RL_REALLOC(fileData, newDataSize);

            if (newFileData != NULL)
            {
                // RL_REALLOC succeded
                int *dataPtr = (int *)newFileData;
                dataPtr[position] = value;
            }
            else
            {
                // RL_REALLOC failed
                TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to realloc data (%u), position in bytes (%u) bigger than actual file size", STORAGE_DATA_FILE, dataSize, position*sizeof(int));

                // We store the old size of the file
                newFileData = fileData;
                newDataSize = dataSize;
            }
        }
        else
        {
            // Store the old size of the file
            newFileData = fileData;
            newDataSize = dataSize;

            // Replace value on selected position
            int *dataPtr = (int *)newFileData;
            dataPtr[position] = value;
        }

        success = SaveFileData(STORAGE_DATA_FILE, newFileData, newDataSize);
        RL_FREE(newFileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Saved storage value: %i", STORAGE_DATA_FILE, value);
    }
    else
    {
        TraceLog(LOG_INFO, "FILEIO: [%s] File created successfully", STORAGE_DATA_FILE);

        dataSize = (position + 1)*sizeof(int);
        fileData = (unsigned char *)RL_MALLOC(dataSize);
        int *dataPtr = (int *)fileData;
        dataPtr[position] = value;

        success = SaveFileData(STORAGE_DATA_FILE, fileData, dataSize);
        UnloadFileData(fileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Saved storage value: %i", STORAGE_DATA_FILE, value);
    }

    return success;
}

// Load integer value from storage file (from defined position)
// NOTE: If requested position could not be found, value 0 is returned
int LoadStorageValue(unsigned int position)
{
    int value = 0;
    unsigned int dataSize = 0;
    unsigned char *fileData = LoadFileData(STORAGE_DATA_FILE, &dataSize);

    if (fileData != NULL)
    {
        if (dataSize < (position*4)) TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to find storage position: %i", STORAGE_DATA_FILE, position);
        else
        {
            int *dataPtr = (int *)fileData;
            value = dataPtr[position];
        }

        UnloadFileData(fileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Loaded storage value: %i", STORAGE_DATA_FILE, value);
    }

    return value;
}



void InitGameState(void) {

    score = 0;
    hiscore = LoadStorageValue(STORAGE_POSITION_HISCORE);
    //Set the finishGame to false
    finishGame = false;

    //Set the walls's texture to whatever is in the class
    wall.setTexture();

    //And then the fruit's texture
    apple.setTexture();
    /*
    Looks complicated, but all it does is wrap the walls around the playable area.
    It is made with flexibility in mind, so in case playable area shrunk or grew,
    it will wrap around it.
    */
    int modifyX = cellSize*(playableCellPos.x-2);
    int modifyY = cellSize*(playableCellPos.y);

    setOfWalls.clear();

    do {    
        wall.changePos(modifyX, cellSize*(playableCellPos.y-2));
        setOfWalls.push_back(wall);
        wall.changePos(modifyX, cellSize*(playableCellPos.y+playableCellCount));
        setOfWalls.push_back(wall);

        wall.changePos(cellSize*(playableCellPos.x-2), modifyY);
        setOfWalls.push_back(wall);
        wall.changePos(cellSize*(playableCellPos.x+playableCellCount), modifyY);
        setOfWalls.push_back(wall);

        modifyX += 2*cellSize;
        if (modifyX < cellSize*(playableCellPos.x+playableCellCount)) modifyY += 2*cellSize;
    }
    while (modifyX <= cellSize*(playableCellPos.x+playableCellCount));

    //Set the player properties
    player.setSpeed(1.0f);
    player.setRandomPos();
    player.resetPlayerLoss();

    //Set the apple position
    apple.changePos(player.getBody());
}

void UpdateGameState(){
            
            //Supposedly delta time, but the raylib function GetFrameTime() does not work well so this should do.
            double seconds = (double)GetFPS()*0.1;

            //Controls. It only accepts WASD for now.
            if (IsKeyPressed(KEY_W) && player.getState() != down) player.changeMoveState(up);
            else if (IsKeyPressed(KEY_S) && player.getState() != up) player.changeMoveState(down);
            else if (IsKeyPressed(KEY_A) && player.getState() != right) player.changeMoveState(left);
            else if (IsKeyPressed(KEY_D) && player.getState() != left) player.changeMoveState(right);

            if (IsKeyPressed(KEY_P)) {
                pausegame = true;
            }

            /*
            If player has moved in a different direction, then the position gets updated
            immediately, and restarts the timer. Otherwise, it just updates when the timer gets
            to zero.
            */
            if (player.isMoveStateDifferent()) { player.updatePos(); timer = seconds; }
            else if (timer <= 0.0) { player.updatePos();}

            /*
            Collision check!! :O
            Checks only two things:
            -   If head of the snake is outside the boundaries of the playable area, it means that it
                has hit the wall, but we only check if it doesn't collide with the playable area as to
                save a lot of computational power. Goes straight to game over state.
            -   If the player's head collides with the fruit, it means that it has eaten the apple, and therefore
                we grow the snake's body and put the fruit in a different area.
            */
            if (timer == seconds) {
                if (!CheckCollisionRecs(player.getRect(), {(float)(playableCellPos.x*cellSize), ((float)playableCellPos.y*cellSize), (float)(playableCellCount*cellSize), (float)(playableCellCount*cellSize)})) { std::cout << "You touched the wall, real!!" << std::endl; player.playerLost(); finishGame = true;}
                if (CheckCollisionRecs(player.getRect(), apple.getRec())) { player.addSegment(); apple.changePos(player.getBody()); (playableCellCount == 16) ? score += 50 : score += 10;}
            }

            //Timer to keep the movement consistent (jumping from one grid box to another).
            (timer <= 0.0) ? timer = seconds : timer--;

            //If player lost, finish the game! Simple.
            if (player.checkPlayerLoss()) {finishGame = true; return;}
}

void DrawGameState(){
    //Drawing the snek
    player.drawSnek();

    //Draw the set of walls that we created while initializing.
    for (int i = 0; i < setOfWalls.size(); i++) {
        setOfWalls[i].updateWall();
    }

    //Draw the fruit!
    apple.drawFruit();

    DrawText(TextFormat("Score: %i", score), (float)(setOfWalls.at(0).getPos().x), (float)(setOfWalls.at(0).getPos().y-(2*cellSize)), 20, LIGHTGRAY);
    DrawText(TextFormat("High score: %i", hiscore), (float)(setOfWalls.at(0).getPos().x), (float)(setOfWalls.at(0).getPos().y-(4*cellSize)), 20, LIGHTGRAY);
}   

void UnloadGameState(){
    SaveStorageValue(STORAGE_POSITION_HISCORE, hiscore);

    //Clear everything in the set of walls
    setOfWalls.clear();

    //Unload the textures (for now its just the wall that has it)
    wall.unloadWallTexture();
}

bool getPauseStatus(){
    return pausegame;
}

void setPauseStatus(bool pauseStatus){
    pausegame = pauseStatus;
}
bool FinishGameState(){
    if (score > hiscore) hiscore = score;

    return finishGame;
}


//--------------------------------------

//--------------------------------------
// Region: Logo State Functions
//--------------------------------------

//Local variables
Texture2D logo;
bool FinishLogo = false;
float scale = 0.0f;

float timerAnimation = 0.0f;

//Easing functions (courtesy of Andrey Sitnik and Ivan Solovev)

float easeOutQuad(float timer) {
    if (timer >= 1.01f) return 1.0f;
    return 1.0f - (1.0f - timer) * (1.0f - timer);
}

float easeInQuad(float timer) {
    if (timer >= 1.01f) return 1.0f;
    else if (timer <= -0.01f) return 0.0f;
    return timer * timer;
}

void InitLogoState(){
    logo = LoadTexture("res/wavelighty.png");
    FinishLogo = false;
}

void UpdateLogoState(){
    if (timerAnimation <= 2.01f) scale = 0.5f * easeOutQuad(timerAnimation*0.5f);
    else if (timerAnimation >= 5.99f && timerAnimation <=7.01f) scale = 0.5f * easeInQuad(7.01f - (timerAnimation));
    else if (timerAnimation > 7.01f) FinishLogo = true;
    timerAnimation += GetFrameTime();

    std::cout << "timerAnimation: " << timerAnimation << std::endl;
}

void DrawLogoState(){
    Vector2 positionToCenter = {(GetScreenWidth()/2)-(((float)logo.width/2)*scale), (GetScreenHeight()/2)-(((float)logo.height/2)*scale)};
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
    DrawTextureEx(logo, positionToCenter, 0.0f, scale, WHITE);
}

void UnloadLogoState(){
    scale = 0.0f;
    timerAnimation = 0.0f;
    UnloadTexture(logo);
}

bool FinishLogoState(){
    return FinishLogo;
}
//--------------------------------------

//--------------------------------------
// Region: Over State Functions
//--------------------------------------
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
//--------------------------------------

//--------------------------------------
// Region: Title State Functions
//--------------------------------------

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
