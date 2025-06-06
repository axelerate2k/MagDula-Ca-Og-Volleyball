#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <math.h>
#include <Windows.h>
#include <string>
#include <vector>
#include "Animation.h"
#include "GameObjects.h"
#include "GameManager.h"

using namespace std;
using namespace sf;

GameManager::GameManager() : window(VideoMode(WIDTH, HEIGHT), "Magdula Ca ug Volleyball!") {
    window.setFramerateLimit(60);
    initialize();
}

void GameManager::initialize() {
    setupGraphics();
    setupAnimations();
    setupUI();
    setupAudio();

    // Initialize positions
    ballC.setX(250); ballC.setY(150);
    p1C.setX(200); p1C.setY(840);
    p2C.setX(1400); p2C.setY(840);

    // Initialize player states
    p1C.setCurrentState(IDLE); p2C.setCurrentState(IDLE);
    p1C.setIsJumping(false); p1C.setIsBottom(true); p1C.setScore(0);
    p2C.setIsJumping(false); p2C.setIsBottom(true); p2C.setScore(0);
}

void GameManager::setupAnimations() {
    p1IdleAnim.loadFrames("images/p1_idle_", 8); p1IdleAnim.setFrameTime(0.15f);
    p1RunAnim.loadFrames("images/p1_run_", 8); p1RunAnim.setFrameTime(0.1f);
    p1JumpAnim.loadFrames("images/p1_jump_", 1); p1JumpAnim.setLoop(false);

    p2IdleAnim.loadFrames("images/p2_idle_", 8); p2IdleAnim.setFrameTime(0.15f);
    p2RunAnim.loadFrames("images/p2_run_", 8); p2RunAnim.setFrameTime(0.1f);
    p2JumpAnim.loadFrames("images/p2_jump_", 1); p2JumpAnim.setLoop(false);

    p1Sprite.setScale(5.0f, 5.0f); p1Sprite.setOrigin(30, 48);
    p2Sprite.setScale(-5.0f, 5.0f); p2Sprite.setOrigin(30, 48);
}

void GameManager::setupUI() {
    font.loadFromFile("font/arcade.ttf");

    title.setFont(font); title.setCharacterSize(150); title.setFillColor(Color::Black);
    title.setStyle(Text::Bold); title.setString("                 Magdula   Ca   ug \n                      Volleyball!");

    startText.setFont(font); startText.setCharacterSize(50); startText.setFillColor(Color::Yellow);
    startText.setString("\n\n\n\n\n\n\n\n\n\n\n\n					     												              Press Enter to Play!!");

    Score.setFont(font); Score.setCharacterSize(150); Score.setFillColor(Color::Black);
    Score.setStyle(Text::Bold); Score.setPosition(777, 0);

    p1Score.setFont(font); p1Score.setCharacterSize(150); p1Score.setFillColor(Color::Black);
    p1Score.setStyle(Text::Bold); p1Score.setPosition(250, 0);

    p2Score.setFont(font); p2Score.setCharacterSize(150); p2Score.setFillColor(Color::Black);
    p2Score.setStyle(Text::Bold); p2Score.setPosition(1200, 0);

    endText.setFont(font); endText.setCharacterSize(90); endText.setFillColor(Color::Yellow);
    endText.setPosition(500, 100);

    retryText.setFont(font); retryText.setString("Retry"); retryText.setCharacterSize(60);
    retryText.setFillColor(Color::White); retryText.setPosition(720, 310);

    retryButton.setSize(Vector2f(250, 80)); retryButton.setFillColor(Color(255, 0, 0));
    retryButton.setPosition(675, 300);

    instructionText.setFont(font); instructionText.setCharacterSize(40); instructionText.setFillColor(Color::Black);
    instructionText.setString("Player															Controls\n\nPlayer 1\n  Move Left															A\n  Move Right														D\n  Jump																			   W\n  Speed Up																Space \n\nPlayer 2\n  Move Left											     Left Arrow\n  Move Right											Right Arrow\n  Jump																   Up Arrow\n  Speed Up												     Right Shift \n\nPress Enter to continue");
    instructionText.setPosition(200, 100);
}

void GameManager::setupAudio() {
    music.openFromFile("musics/mainMusic.wav");
    bounceb.loadFromFile("musics/Bounce.wav"); bounce.setBuffer(bounceb);
    crowdb.loadFromFile("musics/Crowd.wav"); crowd.setBuffer(crowdb);
    scoreb.loadFromFile("musics/ScoreM.wav"); scoreM.setBuffer(scoreb);
}

void GameManager::setupGraphics() {
    Image icon; icon.loadFromFile("images/ry.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    t1.loadFromFile("images/bg_main.png"); t2.loadFromFile("images/background.png");
    t3.loadFromFile("images/ry.png"); cloud.loadFromFile("images/clouds.png");

    mainBackground.setTexture(t1); background.setTexture(t2); ball.setTexture(t3);
    cloud1.setTexture(cloud); cloud2.setTexture(cloud);

    wall.setSize(Vector2f(20, 330)); wall.setFillColor(Color::Transparent);
    wall.setOrigin(10, 0); wall.setPosition(800, 515);

    ball.setOrigin(96, 96); ball.setScale(0.7f, 0.7f);
    ball.setPosition(ballC.getX(), ballC.getY());

    cloud1.setPosition(10.f, 100.f); cloud2.setPosition(900.f, 70.f);
}

void GameManager::run() {
    music.play();

    while (window.isOpen()) {
        handleEvents();
        float deltaTime = animationClock.restart().asSeconds();

        if (instructionScreen) {
            showInstructions();
        }
        else if (!start) {
            showMainMenu();
        }
        else {
            music.stop();
            updateGame(deltaTime);
            render();
        }

        window.display();
    }
}

void GameManager::handleEvents() {
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) window.close();
    }

    if (instructionScreen && Keyboard::isKeyPressed(Keyboard::Enter)) {
        instructionScreen = false;
        sleep(milliseconds(200));
    }

    if (Keyboard::isKeyPressed(Keyboard::Enter) && !start) {
        start = true;
        window.clear();
    }
}

void GameManager::showInstructions() {
    window.clear(Color::White);
    window.draw(instructionText);
}

void GameManager::showMainMenu() {
    window.clear();
    Time dt = clock.restart();
    float delta = dt.asSeconds();

    cloud1.move(cloudSpeed * delta, 0);
    cloud2.move(cloudSpeed * delta, 0);

    if (cloud1.getPosition().x > 900.f) cloud1.setPosition(-cloud1.getGlobalBounds().width, cloud1.getPosition().y);
    if (cloud2.getPosition().x > 900.f) cloud2.setPosition(-cloud2.getGlobalBounds().width, cloud2.getPosition().y);

    window.draw(mainBackground);
    window.draw(cloud1); window.draw(cloud2);
    window.draw(title); window.draw(startText);
}

void GameManager::updateGame(float deltaTime) {
    // Update animations
    p1IdleAnim.update(deltaTime); p1RunAnim.update(deltaTime); p1JumpAnim.update(deltaTime);
    p2IdleAnim.update(deltaTime); p2RunAnim.update(deltaTime); p2JumpAnim.update(deltaTime);

    updatePlayer1(deltaTime);
    updatePlayer2(deltaTime);
    updateBall();
    checkCollisions();
}

void GameManager::updatePlayer1(float deltaTime) {
    // Store previous position BEFORE updating current position
    float prevX = p1C.getX();
    float prevY = p1C.getY();

    // Determine P1 state
    PlayerState newState = IDLE;
    if (p1C.getIsJumping() || !p1C.getIsBottom()) newState = JUMPING;
    else if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::D)) newState = RUNNING;

    if (p1C.getCurrentState() != newState) {
        p1C.setCurrentState(newState);
        if (newState == RUNNING) p1RunAnim.reset();
        else if (newState == JUMPING) p1JumpAnim.reset();
        else p1IdleAnim.reset();
    }

    // Jump - Check jump FIRST, before movement affects isBottom
    bool jumpPressed = Keyboard::isKeyPressed(Keyboard::W);
    if (jumpPressed && !wasP1JumpPressed && p1C.getIsBottom() && !p1C.getIsJumping()) {
        p1C.setIsJumping(true);
        p1C.setIsBottom(false);
        p1C.setIsFalling(false);
    }
    wasP1JumpPressed = jumpPressed;

    // Movement
    float speed = Keyboard::isKeyPressed(Keyboard::Space) ? 20.0f : 10.0f;
    if (Keyboard::isKeyPressed(Keyboard::A)) {
        p1C.setX(p1C.getX() - speed);
    }
    if (Keyboard::isKeyPressed(Keyboard::D)) {
        p1C.setX(p1C.getX() + speed);
    }

    // Physics
    if (p1C.getIsJumping() && !p1C.getIsFalling()) {
        p1C.setY(p1C.getY() - gravity);
    }
    else {
        p1C.setY(p1C.getY() + gravity);
    }

    // Boundary checks
    if (p1C.getY() >= Y_BOTTOM) {
        p1C.setY(Y_BOTTOM);
        p1C.setIsBottom(true);
        p1C.setIsJumping(false);  // Add this line
        p1C.setIsFalling(false);
    }
    if (p1C.getY() <= Y_BOTTOM - 250) {
        p1C.setIsJumping(false);
        p1C.setIsFalling(true);  // Add this line
    }
    if (p1C.getX() <= 64) p1C.setX(64);
    if (p1C.getX() >= 736) p1C.setX(736);

    // Update previous position AFTER all movement calculations
    p1C.setPrevX(prevX);
    p1C.setPrevY(prevY);
}

void GameManager::updatePlayer2(float deltaTime) {
    // Store previous position BEFORE updating current position
    float prevX = p2C.getX();
    float prevY = p2C.getY();

    // Determine P2 state
    PlayerState newState = IDLE;
    if (p2C.getIsJumping() || !p2C.getIsBottom()) newState = JUMPING;
    else if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::Right) ||
        Keyboard::isKeyPressed(Keyboard::Numpad4) || Keyboard::isKeyPressed(Keyboard::Numpad6)) newState = RUNNING;

    if (p2C.getCurrentState() != newState) {
        p2C.setCurrentState(newState);
        if (newState == RUNNING) p2RunAnim.reset();
        else if (newState == JUMPING) p2JumpAnim.reset();
        else p2IdleAnim.reset();
    }
    // Jump - Check jump BEFORE physics update
    bool jumpPressed = Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Numpad8);
    if (jumpPressed && !wasP2JumpPressed && p2C.getIsBottom()) {
        p2C.setIsJumping(true);
        p2C.setIsBottom(false);
        p2C.setIsFalling(false);
    }
    wasP2JumpPressed = jumpPressed;

    // Movement
    float speed = Keyboard::isKeyPressed(Keyboard::RShift) ? 20.0f : 10.0f;
    if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::Numpad4)) {
        p2C.setX(p2C.getX() - speed);
        if (p2C.getIsBottom() && !p2C.getIsJumping()) p2C.setIsBottom(false);
    }
    if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::Numpad6)) {
        p2C.setX(p2C.getX() + speed);
        if (p2C.getIsBottom() && !p2C.getIsJumping()) p2C.setIsBottom(false);
    }


    // Physics
    if (p2C.getIsJumping() && !p2C.getIsFalling()) {
        p2C.setY(p2C.getY() - gravity);
    }
    else {
        p2C.setY(p2C.getY() + gravity);
    }

    // Boundary checks
    if (p2C.getY() >= Y_BOTTOM) {
        p2C.setY(Y_BOTTOM);
        p2C.setIsBottom(true);
        p2C.setIsJumping(false);  // Add this line
        p2C.setIsFalling(false);
    }
    if (p2C.getY() <= Y_BOTTOM - 250) {
        p2C.setIsJumping(false);
        p2C.setIsFalling(true);  // Add this line
    }
    if (p2C.getX() <= 864) p2C.setX(864);
    if (p2C.getX() >= WIDTH - 64) p2C.setX(WIDTH - 64);

    // Update previous position AFTER all movement calculations
    p2C.setPrevX(prevX);
    p2C.setPrevY(prevY);
}

void GameManager::updateBall() {
    ballC.setX(ballC.getX() + dx);
    ballC.setY(ballC.getY() + dy);

    // Wall collision
    if (FloatRect(ballC.getX() - (96 * 0.7f), ballC.getY() - (96 * 0.7f), 96 * 0.7f * 2, 96 * 0.7f * 2).intersects(wall.getGlobalBounds())) {
        if (ballC.getY() <= 490) {
            dy = -dy;
            ballC.setY(520 - (96 * 0.7f));
        }
        else {
            dx = -dx;
        }
    }

    // Boundary collisions
    if (ballC.getX() + (96 * 0.7) >= WIDTH || ballC.getX() - (96 * 0.7) <= 0) {
        dx = -dx;
    }

    // Apply gravity when ball is in air
    if (ballC.getY() + (96 * 0.7f) < Y_BOTTOM) {
        dy += gravityAcceleration;
        if (dy > maxFallSpeed) dy = maxFallSpeed;
    }

    // Limit velocities
    if (abs(dx) >= 21) dx = (dx < 0) ? -21 : 21;
    if (abs(dy) >= 21) dy = (dy < 0) ? -21 : 21;

    // Scoring
    if (ballC.getY() + (96 * 0.7) >= Y_BOTTOM) {
        scoreM.play();
        ballC.setY(80);
        p1C.setX(200);
        p1C.setY(Y_BOTTOM);
        p2C.setX(1400);
        p2C.setY(Y_BOTTOM);

        if (ballC.getX() <= 790) {
            ballC.setX(200 + 64);
            p2C.setScore(p2C.getScore() + 1);
        }
        else if (ballC.getX() >= 810) {
            ballC.setX(1400 - 64);
            p1C.setScore(p1C.getScore() + 1);
        }

        Sleep(1000);
        dx = 0;
        dy = gravity;
    }

    // Win conditions
    if (p1C.getScore() == 5) {
        over = true;
        endText.setString("Player 1 Win!");
    }
    if (p2C.getScore() == 5) {
        over = true;
        endText.setString("Player 2 Win!");
    }
}

void GameManager::checkCollisions() {
    FloatRect p1Bounds(p1C.getX() - 64, p1C.getY() - 128, 128, 128);
    FloatRect p2Bounds(p2C.getX() - 64, p2C.getY() - 128, 128, 128);
    FloatRect ballBounds(ballC.getX() - (96 * 0.7f), ballC.getY() - (96 * 0.7f), 96 * 0.7f * 2, 96 * 0.7f * 2);

    if (ballBounds.intersects(p1Bounds)) {
        bounce.play();
        if (p1C.getIsJumping()) {
            p1C.setIsFalling(true);
            p1C.setIsJumping(false);
        }

        // Calculate velocity based on the difference between current and previous positions
        float playerDx = p1C.getX() - p1C.getPrevX();
        float playerDy = p1C.getY() - p1C.getPrevY();

        // Set ball velocity with a minimum horizontal speed and upward force
        dx = playerDx * 1.5f;
        dy = -abs(playerDy) - 5.0f;

        // Ensure minimum horizontal velocity if player isn't moving much
        if (abs(dx) < 3.0f) {
            dx = (ballC.getX() < p1C.getX()) ? -5.0f : 5.0f;
        }

        // Ensure upward velocity
        if (dy > -3.0f) dy = -8.0f;
    }

    if (ballBounds.intersects(p2Bounds)) {
        bounce.play();
        if (p2C.getIsJumping()) {
            p2C.setIsFalling(true);
            p2C.setIsJumping(false);
        }

        // Calculate velocity based on the difference between current and previous positions
        float playerDx = p2C.getX() - p2C.getPrevX();
        float playerDy = p2C.getY() - p2C.getPrevY();

        // Set ball velocity with a minimum horizontal speed and upward force
        dx = playerDx * 1.5f;
        dy = -abs(playerDy) - 5.0f;

        // Ensure minimum horizontal velocity if player isn't moving much
        if (abs(dx) < 3.0f) {
            dx = (ballC.getX() < p2C.getX()) ? -5.0f : 5.0f;
        }

        // Ensure upward velocity
        if (dy > -3.0f) dy = -8.0f;
    }
}

void GameManager::render() {
    window.clear();
    window.draw(background); window.draw(wall); window.draw(ball);

    // Set sprite textures based on current state
    Texture* p1Texture = nullptr; Texture* p2Texture = nullptr;

    switch (p1C.getCurrentState()) {
    case IDLE: p1Texture = p1IdleAnim.getCurrentTexture(); break;
    case RUNNING: p1Texture = p1RunAnim.getCurrentTexture(); break;
    case JUMPING: p1Texture = p1JumpAnim.getCurrentTexture(); break;
    }

    switch (p2C.getCurrentState()) {
    case IDLE: p2Texture = p2IdleAnim.getCurrentTexture(); break;
    case RUNNING: p2Texture = p2RunAnim.getCurrentTexture(); break;
    case JUMPING: p2Texture = p2JumpAnim.getCurrentTexture(); break;
    }

    if (p1Texture) p1Sprite.setTexture(*p1Texture);
    if (p2Texture) p2Sprite.setTexture(*p2Texture);

    p1Sprite.setPosition(p1C.getX(), p1C.getY());
    p2Sprite.setPosition(p2C.getX(), p2C.getY());

    window.draw(p1Sprite); window.draw(p2Sprite);
    window.draw(Score); window.draw(p1Score); window.draw(p2Score);

    p1Score.setString(to_string(p1C.getScore()));
    p2Score.setString(to_string(p2C.getScore()));

    if (over) {
        dx = 0; dy = 0;
        window.draw(endText); window.draw(retryButton); window.draw(retryText);

        if (Mouse::isButtonPressed(Mouse::Left)) {
            Vector2i mousePos = Mouse::getPosition(window);
            if (retryButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                resetGame();
                Event event;
                while (Mouse::isButtonPressed(Mouse::Left)) { window.pollEvent(event); }
            }
        }
    }
    else {
        ball.setPosition(ballC.getX(), ballC.getY());
        ball.rotate(10);
    }
}

void GameManager::resetGame() {
    over = false; p1C.setScore(0); p2C.setScore(0);
    p1C.setX(200); p1C.setY(840); p2C.setX(1400); p2C.setY(840);
    ballC.setX(250); ballC.setY(150); dx = 0; dy = 10;
    p1C.setIsJumping(false); p1C.setIsBottom(true);
    p2C.setIsJumping(false); p2C.setIsBottom(true);
    p1C.setCurrentState(IDLE); p2C.setCurrentState(IDLE);
}

int main() {
    GameManager game;
    game.run();
    return 0;
}