//States of the game


#ifndef STATES_H
#define STATES_H

#include "common.h"

#include "snake.h"
#include "wall.h"
#include "fruit.h"



#define STORAGE_DATA_FILE   "hiscore.data"   // Storage file

// NOTE: Storage positions must start with 0, directly related to file memory layout
typedef enum {
    STORAGE_POSITION_HISCORE      = 0
} StorageData;

//Transition variables (for fade in and fade out)
static float alpha = 0.0f;
static bool onTransition = false;
static bool fadeOut = false;
static GameStates fromState = unknown;
static GameStates toState = unknown;

//Logo state functions
void InitLogoState();
void UpdateLogoState();
void DrawLogoState();
void UnloadLogoState();
bool FinishLogoState();

//Title state functions
void InitTitleState(void);
void UpdateTitleState(void);
void DrawTitleState(void);
void UnloadTitleState(void);
bool FinishTitleState();

//Game state functions
void InitGameState(void);
void UpdateGameState(void);
void DrawGameState(void);
void UnloadGameState(void);
bool getPauseStatus();
void setPauseStatus(bool);
bool FinishGameState();
static bool SaveStorageValue(unsigned int position, int value);
static int LoadStorageValue(unsigned int position);

//Game over state functions
void InitOverState(void);
void UpdateOverState(void);
void DrawOverState(void);
void UnloadOverState(void);
bool FinishOverState();
bool ReturnToTitleFromOverState();


#endif