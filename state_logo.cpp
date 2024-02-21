#include "include/states.h"
#include <iostream>
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