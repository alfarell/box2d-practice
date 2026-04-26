#pragma once

#include <SDL3/SDL.h>
#include <box2d/box2d.h>

#include <vector>

struct ObjectProperties {
    b2WorldId* worldId;
    b2BodyDef* bodyDef;
};

class Object {
   protected:
    SDL_Renderer* renderer = NULL;
    b2BodyId bodyId;
    b2ShapeId shapeId;
    b2WorldId* worldId;

   public:
    Object(SDL_Renderer* renderer);
    Object(SDL_Renderer* renderer, ObjectProperties properties);
    virtual ~Object() = default;

    Object(const Object&)            = delete;
    Object& operator=(const Object&) = delete;

    virtual void onEvent();
    virtual void onUpdate();
    virtual void onRender();

    virtual b2BodyId getBodyId() const;
    virtual b2ShapeId getShapeId() const;
    virtual const b2Vec2 getPosition() const;
    virtual const b2Polygon getPolygon() const;
    virtual void setPosition(b2Vec2 position);
    virtual void setXPosition(float x);
    virtual void setYPosition(float y);
};