#pragma once

#include <SDL3/SDL.h>
#include <box2d/box2d.h>

#include "../Helpers/ConstructorMacros.hpp"

#define DEFAULT_GRAVITY b2Vec2{0.0f, 9.8f}
#define DEFAULT_VELOCITY_ITERATIONS 8

class Physics {
   private:
    b2WorldId worldId      = b2_nullWorldId;
    int velocityIterations = DEFAULT_VELOCITY_ITERATIONS;
    bool enabled           = true;

   public:
    Physics() = default;
    ~Physics();

    NON_COPYABLE(Physics)
    NON_MOVABLE(Physics)

    void init();
    void step(float deltaTime);
    void destroy();

    const b2WorldId* getWorldId() const;
    void setGravity(float x, float y);
    void setStepperIterations(int velocityIterations);
    void enable();
    void disable();
};