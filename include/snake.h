/*
Snake class
*/

//Included libraries
#pragma once
#include "common.h"
#include <deque>
#include <iostream>



//playerMoveState: enum specifying all possible directions of snake
enum playerMoveState {up, down, left, right};

class Snake {
    protected:

    //Object stats
    playerMoveState state = right; //Current direction of the player
    float speed = 1.0f; //Current speed of the player
    bool differentMoveState = false; //Did the direction change?
    bool didPlayerLose = false; //Did they lose?
    //------

    //Object body
    Rectangle head = {cellSize*0, cellSize*0, 16, 16}; //Head of snake's body
    unsigned int length = 1; //Length of the body
    std::deque<Rectangle> segments; //The entire body (including the head)
    bool isSegmentAdded = false; //Is the snake growing?
    //------
    public:
    //Returns the loss status of the snek
    bool checkPlayerLoss(){
        return didPlayerLose;
    }

    //Constructor and Destructor
    Snake(){ 
        //Constructor: This should only execute once.
        //The entire purpose is to add in the head immediately to prevent 
        //possible errors related to empty deque.
        segments.push_back(head);
    }
    ~Snake(){
        //Destructor: This should only execute once.
        //If snake is no longer, then pop all of the segments out from the deque
        //This is to prevent a possible memory leak.
        segments.erase(segments.begin(), segments.end());
        length = 0;
    }
    //------

    //Stat changers
    void setSpeed(float speed){
        //Sets the speed of snake.
        this->speed = (float)cellSize*speed;
    }

    void changePos(float changeX, float changeY){
        //Changes the position relatively according to changeX and changeY
        //It relies on class' speed
        head.x += changeX;
        head.y += changeY;
    }

    void changeMoveState(playerMoveState state){
        //Mostly to check if directions have indeed changed
        //If yes, then set the boolean to true.
        if (this->state != state) {differentMoveState = true;}
        this->state = state;
    }

    //------

    //Update and drawing routines
    void updatePos(){
        
        //Change the positions according to the direction
        if (state == right) changePos(speed, 0);
        if (state == left) changePos(-speed, 0);
        if (state == up) changePos(0, -speed);
        if (state == down) changePos(0, speed);

        //If the direction was changed, and we did change the position needed,
        //then set it to false. It is done like that to ensure instant direction change.
        differentMoveState = false;

        //This part is weird, but somewhat simple. It basically creates the head body every time the snek moves,
        //and if there is an excess of body (like if the size of deque is greater than what is allowed to be shown),
        //it removes the excess in a for loop to prevent memory leaks.
        segments.push_front(head);
        if (segments.size() > length) {
            for (unsigned i = 0; i < segments.size() - length; i++) {
                segments.pop_back();
            }
        }

        //Whether we added the segment or not, its still false.
        isSegmentAdded = false;
    }

    void drawSnek(){
        //For loop where it draws the available segments, and then has a basic collision check to see 
        //if the player has bit/touched their tail. It should be in an update function, but doing it here means
        //we save a good bit of computational power (like checking all in one loop instead of two separate loops)
        for (unsigned int i = 0; i < length; i++ ) {
            if (i > 0) {
                DrawRectangleRec(segments.at(i), BLUE);
                if (CheckCollisionRecs(segments.at(i), head) && !isSegmentAdded) {std::cout << "You have touched your tail! Real..." << std::endl; didPlayerLose = true; playerLost();}    
            }
            DrawRectangleRec(head, RED);
        }
    }

    //Basically add segments, good for when you eat food
    void addSegment(){
        isSegmentAdded = true;
        segments.push_back(head);
        length++;
        
    }

    //Debug function: Remove segments
    void removeSegments(){
        segments.pop_back();
        length--;
        
    }

    //Returns the head of the body
    Rectangle getRect(){
        return head;
    }

    //Returns the body itself
     std::deque<Rectangle> getBody(){
        return segments;
    }

    // Returns the condition of the different state
    bool isMoveStateDifferent(){
        return differentMoveState;
    }

    //Returns the state of the snek itself
    playerMoveState getState(){
        return state;
    }

    //Restart the player loss
    void resetPlayerLoss(){
        didPlayerLose = false;
    }


    //set the state to over, basically remove all the segments apart from head, and sets the length back to one.
    void playerLost(){
        segments.erase(segments.begin(), segments.end() - 1);
        length = 1;
        std::cout << "It should be erased now.";
    }

    //sets the position of the snek in a random way, multiplying it by the cell size, and randomizes it
    //in the playable area itself. it then changes direction based on where it is.
    void setRandomPos(){
        head.x = (playableCellPos.x + (rand() % playableCellCount)) * cellSize;
        head.y = (playableCellPos.y + (rand() % playableCellCount)) * cellSize;

        //Set the direction
        (head.x > ((playableCellPos.x+(playableCellCount/2))*cellSize)) ? changeMoveState(left) : changeMoveState(right);

        std::cout << "The position should be adjusted.";
    }

};