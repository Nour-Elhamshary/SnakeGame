#pragma once
#include "common.h"

class Wall {
    protected:
    Rectangle body = {cellSize*0, cellSize*0, 32, 32};
    Vector2 pos = {0, 0};
    Texture2D texture;
    public:
    void updateWall(){
        DrawTextureRec(texture, body, pos, WHITE);
    }

    void changePos(int x, int y){
        pos.x = x;
        pos.y = y;
    }

    Rectangle getRec(){
        return body;
    }

    Vector2 getPos(){
        return pos;
    }

    void setTexture(){
        texture = LoadTexture("res/walls/gray.png");
    }

    void unloadWallTexture(){
        UnloadTexture(texture);
    }

};