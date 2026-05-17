#include "Physics.hpp"

Physics::~Physics() {
    this->destroy();
}

void Physics::init() {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity    = DEFAULT_GRAVITY;
    this->worldId       = b2CreateWorld(&worldDef);
}

void Physics::step(float deltaTime) {
    if (!b2World_IsValid(this->worldId)) {
        SDL_Log("Box2D world is invalid.");
        return;
    }

    if (!this->enabled) return;

    b2World_Step(worldId, deltaTime, this->velocityIterations);
}

void Physics::destroy() {
    if (this->worldId.generation == b2_nullWorldId.generation &&
        this->worldId.index1 == b2_nullWorldId.index1) {
        return;
    }

    if (!b2World_IsValid(this->worldId)) {
        SDL_Log("Box2D world is invalid.");
        return;
    }

    b2DestroyWorld(worldId);
    worldId = b2_nullWorldId;
}

const b2WorldId* Physics::getWorldId() const {
    return &this->worldId;
}

void Physics::setGravity(float x, float y) {
    if (!b2World_IsValid(this->worldId)) {
        SDL_Log("Box2D world is invalid.");
        return;
    }

    b2Vec2 gravity = b2Vec2{x, y};
    b2World_SetGravity(this->worldId, gravity);
}

void Physics::setStepperIterations(int velocityIterations) {
    if (!b2World_IsValid(this->worldId)) {
        SDL_Log("Box2D world is invalid.");
        return;
    }

    if (velocityIterations <= 0) {
        SDL_Log("Velocity iterations must be greater than 0.");
        return;
    }

    this->velocityIterations = velocityIterations;
}

void Physics::enable() {
    this->enabled = true;
}

void Physics::disable() {
    this->enabled = false;
}