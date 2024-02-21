/*
Fruit
*/

//Included libraries
#pragma once
#include "common.h"
#include <iostream>

#include <deque>

class Fruit{
    protected:
    Rectangle body = {cellSize*8, cellSize*8, 16, 16};
    public:
    void drawFruit(){
        DrawRectangleRec(body, WHITE);
    }
    Rectangle getRec(){
        return body;
    }

    void changePos(std::deque<Rectangle> snekbody){
        body.x = (playableCellPos.x + (rand() % playableCellCount)) * cellSize;
        body.y = (playableCellPos.y + (rand() % playableCellCount)) * cellSize;

        for (unsigned int i = 0; i < snekbody.size(); i++ ) {
            if (CheckCollisionRecs(snekbody.at(i), body)) { changePos(snekbody); break;}
        }
        
    }
};