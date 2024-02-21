#include "include/states.h"
#include <queue>
#include <stdlib.h>         // Required for: calloc(), free()

Snake player;
Wall wall;
Fruit apple;

std::vector<Wall> setOfWalls;


double timer = 0.0;
bool pausegame = false;
bool finishGame = false;

int score = 0;
int hiscore = 0;

void InitGameState(void) {

    score = 0;
    hiscore = LoadStorageValue(STORAGE_POSITION_HISCORE);
    //Set the finishGame to false
    finishGame = false;

    //Set the walls's texture to whatever is in the class
    wall.setTexture();

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