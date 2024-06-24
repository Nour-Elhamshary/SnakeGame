//States of the game


#ifndef STATES_H
#define STATES_H



// NOTE: Storage positions must start with 0, directly related to file memory layout


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