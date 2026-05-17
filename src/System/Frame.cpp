#include "Frame.hpp"

Frame& Frame::Get() {
    static Frame instance;
    return instance;
}

void Frame::init() {
    this->currentTicks         = SDL_GetPerformanceCounter();
    this->lastTicks            = this->currentTicks;
    this->measuredFrameTime    = 0.0;
    this->performanceFrequency = (double)(SDL_GetPerformanceFrequency());
    this->accumulatedTime      = 0.0;
}

void Frame::calculateDeltaTime() {
    this->lastTicks    = this->currentTicks;
    this->currentTicks = SDL_GetPerformanceCounter();
    this->measuredFrameTime =
        (this->currentTicks - this->lastTicks) / this->performanceFrequency;
    if (this->measuredFrameTime > MAX_DELTA_TIME)
        this->measuredFrameTime = MAX_DELTA_TIME;
}

void Frame::accumulateTime(
    const std::function<void(float deltaTime)>& updateFunction) {
    this->accumulatedTime += this->measuredFrameTime;

    while (this->accumulatedTime >= this->deltaTime) {
        updateFunction(this->deltaTime);
        this->accumulatedTime -= this->deltaTime;
    }
}

void Frame::renderFrame(SDL_Renderer* renderer) {
    if (this->isVSyncEnabled) return;

    Uint64 now = SDL_GetPerformanceCounter();
    double elapsedThisFrame =
        (double)(now - this->currentTicks) / this->performanceFrequency;
    double targetFrameTime = 1.0 / (double)this->targetFPS;
    double remainingTime   = targetFrameTime - elapsedThisFrame;

    if (remainingTime <= 0.0) return;

    SDL_DelayNS((Uint64)(remainingTime * 1e9));
}

void Frame::enableVSync(SDL_Renderer* renderer) {
    SDL_SetRenderVSync(renderer, 1);
    this->isVSyncEnabled = true;
}

void Frame::disableVSync(SDL_Renderer* renderer) {
    SDL_SetRenderVSync(renderer, 0);
    this->isVSyncEnabled = false;
}

void Frame::setTargetFPS(int newTargetFPS) {
    if (newTargetFPS <= 0) return;
    this->targetFPS = newTargetFPS;
    this->deltaTime = (float)(1.0 / this->targetFPS);
}

int Frame::getTargetFPS() const {
    return this->targetFPS;
}

float Frame::getDeltaTime() const {
    return this->deltaTime;
}