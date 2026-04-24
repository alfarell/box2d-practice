#include "Frame.hpp"

Frame& Frame::Get() {
    static Frame instance;
    return instance;
}

void Frame::init() {
    currentTicks         = SDL_GetPerformanceCounter();
    lastTicks            = 0;
    targetFrameTime      = 0.0;
    performanceFrequency = (float)(SDL_GetPerformanceFrequency());
    accumulatedTime      = 0.0;
}

void Frame::calculateDeltaTime() {
    lastTicks       = currentTicks;
    currentTicks    = SDL_GetPerformanceCounter();
    targetFrameTime = (currentTicks - lastTicks) / performanceFrequency;
}

void Frame::accumulateTime(const std::function<void()>& updateFunction) {
    accumulatedTime += targetFrameTime;

    while (accumulatedTime >= deltaTime) {
        updateFunction();
        accumulatedTime -= deltaTime;
    }
}

void Frame::setTargetFPS(int newTargetFPS) {
    targetFPS = newTargetFPS;
    deltaTime = (float)(1.0 / targetFPS);
}

int Frame::getTargetFPS() const {
    return targetFPS;
}

float Frame::getDeltaTime() const {
    return deltaTime;
}