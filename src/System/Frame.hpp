#pragma once

#include <SDL3/SDL.h>

#include <functional>

#include "../Helpers/ConstructorMacros.hpp"

#define DEFAULT_TARGET_FPS 60
#define MAX_DELTA_TIME 0.25f

class Frame {
   private:
    bool isVSyncEnabled         = false;
    int targetFPS               = DEFAULT_TARGET_FPS;
    float deltaTime             = (float)(1.0 / DEFAULT_TARGET_FPS);
    Uint64 currentTicks         = 0;
    Uint64 lastTicks            = 0;
    double measuredFrameTime    = 0.0;
    double performanceFrequency = 0.0;
    double accumulatedTime      = 0.0;

    Frame() = default;

   public:
    ~Frame() = default;

    NON_COPYABLE(Frame)
    NON_MOVABLE(Frame)

    static Frame& Get();

    void init();
    void calculateDeltaTime();
    void accumulateTime(
        const std::function<void(float deltaTime)>& updateFunction);
    void renderFrame(SDL_Renderer* renderer);

    int getTargetFPS() const;
    float getDeltaTime() const;
    void setTargetFPS(int newTargetFPS);
    void enableVSync(SDL_Renderer* renderer);
    void disableVSync(SDL_Renderer* renderer);
};