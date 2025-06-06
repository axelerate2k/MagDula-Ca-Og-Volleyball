#pragma once

enum PlayerState {
    IDLE,
    RUNNING,
    JUMPING
};

class Object {
private:
    float x, y;
public:
    float getX() { return x; }
    float getY() { return y; }
    void setX(float xval) { x = xval; }
    void setY(float yval) { y = yval; }
};

class Player {
private:
    Object position;
    int score;
    bool isJumping, isBottom, isFalling, isMoving;
    PlayerState currentState, previousState;
    float prevX = 0, prevY = 0;

public:
    // Position methods
    float getX() { return position.getX(); }
    float getY() { return position.getY(); }
    void setX(float xval) { position.setX(xval); }
    void setY(float yval) { position.setY(yval); }
    float getPrevX() { return prevX; }
    float getPrevY() { return prevY; }
    void setPrevX(float x) { prevX = x; }
    void setPrevY(float y) { prevY = y; }

    // State methods
    bool getIsJumping() { return isJumping; }
    bool getIsBottom() { return isBottom; }
    bool getIsFalling() { return isFalling; }
    bool getIsMoving() { return isMoving; }
    void setIsJumping(bool jumping) { isJumping = jumping; }
    void setIsBottom(bool bottom) { isBottom = bottom; }
    void setIsFalling(bool falling) { isFalling = falling; }
    void setIsMoving(bool moving) { isMoving = moving; }

    // Score methods
    void setScore(int s) { score = s; }
    int getScore() { return score; }

    // Animation state methods
    PlayerState getCurrentState() { return currentState; }
    void setCurrentState(PlayerState state) {
        previousState = currentState;
        currentState = state;
    }
    bool stateChanged() { return currentState != previousState; }
};