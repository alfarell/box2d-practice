#pragma once

#include <SDL3/SDL.h>

#include <functional>

#include "../Helpers/ConstructorMacros.hpp"

#define DEFAULT_TARGET_FPS 60

class Frame {
   private:
    int targetFPS   = DEFAULT_TARGET_FPS;
    float deltaTime = (float)(1.0 / DEFAULT_TARGET_FPS);
    Uint64 currentTicks;
    Uint64 lastTicks;
    double targetFrameTime;
    double performanceFrequency;
    double accumulatedTime;

    Frame() = default;

   public:
    ~Frame() = default;

    NON_COPYABLE(Frame)
    NON_MOVABLE(Frame)

    static Frame& Get();

    void init();
    void calculateDeltaTime();
    void accumulateTime(const std::function<void()>& updateFunction);
    void setTargetFPS(int newTargetFPS);
    int getTargetFPS() const;
    float getDeltaTime() const;
};