#include "include/common.h"
#include "include/states.h"


void ChangeToState(GameStates state) {
    //First, deinitialize and finish the current states
    switch (currentGameState){
        case logostate:
            UnloadLogoState();
        break;
        case titlestate:
        break;
        case gamestate:
            UnloadGameState();
        break;
        case overstate:
        break;
        default: break;
    }

    //Then initialize whichever state to change to
    switch (state){
        case titlestate: InitTitleState();
        break;
        case gamestate: InitGameState();
        break;
        case overstate: InitOverState();
        break;
        default: break;
    }

    //Finally, change the current state.
    currentGameState = state;
}

void TransitionToState(GameStates state) {
    alpha = 0.0f;
    onTransition = true;
    fadeOut = false;
    fromState = currentGameState;
    toState = state;
}

void UpdateTransition(){
    if (!fadeOut){
        alpha += 0.05f;
        if (alpha >= 1.01f) {
            alpha = 1.0f;
            ChangeToState(toState);
            fadeOut = true;
        }
    }
    else {
        alpha -= 0.05f;
        if (alpha <= -0.01f) {
            alpha = 0.0f;
            fadeOut = false;
            onTransition = false;
            fromState = unknown;
            toState = unknown;
        }
    }
}

void DrawTransition(){
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, alpha));
}

int main(void)
{
    int FPS = 60;

    InitWindow(cellSize*totalCellCount.x, cellSize*totalCellCount.y, "snek");
    SetTargetFPS(FPS);
    InitAudioDevice();

    Music mainmusic = LoadMusicStream("res//mus//snektitle.xm");
    Texture2D bg = LoadTexture("res/purplenebula.png");

    mainmusic.looping = true;
    
    PlayMusicStream(mainmusic);
    InitGameState();
    InitLogoState();



    while (!WindowShouldClose())
    {
    //Update
        
        

    UpdateMusicStream(mainmusic);

    if (!onTransition) {
        switch (currentGameState) {
        case logostate: {
            UpdateLogoState();
            if (FinishLogoState()) TransitionToState(titlestate);
        }
        case titlestate: {
            UpdateTitleState();
            if (FinishTitleState()) TransitionToState(gamestate);
            break;
        }
        case gamestate: {
            UpdateGameState();
            if (getPauseStatus()) currentGameState = pausestate;
            if (FinishGameState()) TransitionToState(overstate);
            break;
        }
        case pausestate: {
            if(IsKeyPressed(KEY_P)) setPauseStatus(false);

            if (getPauseStatus()) PauseMusicStream(mainmusic);
            else { ResumeMusicStream(mainmusic); currentGameState = gamestate;}
            break;
        }
        case overstate: {
            //First execute the important functions.
            UpdateOverState();
            if (FinishOverState()) ChangeToState(gamestate);
            else if (ReturnToTitleFromOverState()) TransitionToState(titlestate);
            break;
        }
        }
    }
    else UpdateTransition();
        //Drawing
        BeginDrawing();
            DrawTexture(bg,0,0,WHITE);

            switch (currentGameState) {
            case logostate:
                DrawLogoState();
                break;
            case titlestate:
                DrawTitleState();
            break;
            case pausestate:
            case gamestate:
                DrawGameState();
                if (currentGameState == pausestate) {
                    DrawRectangle(0,0,GetScreenWidth(),GetScreenHeight(), Fade(BLACK, 0.5f));
                    DrawText("Paused...", GetScreenWidth()/2, GetScreenHeight()/2, 20, LIGHTGRAY);
                }
            break;
            case overstate:
                DrawOverState();
            break;
            }

        if (onTransition) DrawTransition();

        EndDrawing();
        

    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}