#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace sf;

class Animation {
private:
    vector<Texture> frames;
    int currentFrame;
    float frameTime;
    float elapsedTime;
    bool loop;

public:
    Animation() : currentFrame(0), frameTime(0.1f), elapsedTime(0), loop(true) {}

    void loadFrames(const string& basePath, int frameCount) 
    {
        frames.clear();
        for (int i = 0; i < frameCount; i++) 
        {
            Texture texture;
            if (texture.loadFromFile(basePath + to_string(i) + ".png")) 
                frames.push_back(texture);
        }
    }

    void setFrameTime(float time) { frameTime = time; }
    void setLoop(bool shouldLoop) { loop = shouldLoop; }

    void update(float deltaTime) 
    {
        if (frames.empty()) return;

        elapsedTime += deltaTime;
        if (elapsedTime >= frameTime) 
        {
            elapsedTime = 0;
            currentFrame++;

            if (currentFrame >= frames.size()) 
            {
                if (loop) currentFrame = 0;
                else currentFrame = frames.size() - 1;
            }
        }
    }

    void reset() 
    {
        currentFrame = 0;
        elapsedTime = 0;
    }

    Texture* getCurrentTexture() 
    {
        if (frames.empty()) return nullptr;
        return &frames[currentFrame];
    }

    bool isFinished() {
        return !loop && currentFrame >= frames.size() - 1;
    }
};
