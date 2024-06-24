/*
Fruit
*/

//Included libraries
#pragma once
#include "common.h"

class Fruit{
    protected:
    Rectangle body = {cellSize*0, cellSize*0, 16, 16};
    Texture2D texture;
    public:
    void drawFruit(){
        DrawTexture(texture, body.x, body.y, WHITE);
        //DrawRectangleRec(body, WHITE);
    }
    Rectangle getRec(){
        return body;
    }

    void setTexture(){
        texture = LoadTexture("res/fruit.png");
    }
    void changePos(std::deque<Rectangle> snekbody){
        body.x = (playableCellPos.x + (rand() % playableCellCount)) * cellSize;
        body.y = (playableCellPos.y + (rand() % playableCellCount)) * cellSize;

        for (unsigned int i = 0; i < snekbody.size(); i++ ) {
            if (CheckCollisionRecs(snekbody.at(i), body)) { changePos(snekbody); break;}
        }
        
    }
};