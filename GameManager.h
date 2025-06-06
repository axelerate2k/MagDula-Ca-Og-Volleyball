#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Animation.h"
#include "GameObjects.h"

using namespace sf;

#define WIDTH 1600
#define HEIGHT 900

class GameManager {
private:
    RenderWindow window;
    Player p1C, p2C;
    Object ballC;

    // Game states
    bool instructionScreen = true;
    bool start = false;
    bool over = false;

    // Physics constants
    const int gravity = 10;
    const float gravityAcceleration = 0.3f;
    const float maxFallSpeed = 14.0f;
    const float Y_BOTTOM = 840;

    // Ball physics
    float dx = 0, dy = 10;

    // Animations
    Animation p1IdleAnim, p1RunAnim, p1JumpAnim;
    Animation p2IdleAnim, p2RunAnim, p2JumpAnim;
    Sprite p1Sprite, p2Sprite;
    Clock animationClock;

    // UI Elements
    Font font;
    Text title, startText, Score, p1Score, p2Score, endText, retryText, instructionText;
    RectangleShape retryButton, wall;

    // Audio
    Music music;
    Sound bounce, crowd, scoreM;
    SoundBuffer bounceb, crowdb, scoreb;

    // Graphics
    Texture cloud, t1, t2, t3;
    Sprite cloud1, cloud2, mainBackground, background, ball;
    float cloudSpeed = 25.f;
    Clock clock;

    // Input tracking
    bool wasP1JumpPressed = false;
    bool wasP2JumpPressed = false;

public:
    GameManager();
    void initialize();
    void run();

private:
    void setupAnimations();
    void setupUI();
    void setupAudio();
    void setupGraphics();
    void handleEvents();
    void updateGame(float deltaTime);
    void updatePlayer1(float deltaTime);
    void updatePlayer2(float deltaTime);
    void updateBall();
    void checkCollisions();
    void render();
    void showInstructions();
    void showMainMenu();
    void resetGame();
};