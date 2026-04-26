#pragma once

#include <SDL3/SDL.h>
#include <box2d/box2d.h>

#include <string>
#include <vector>

#include "../System/Color.hpp"

struct ObjectProperties {
    std::string id;
    std::string name;
    std::vector<std::string> tags;
    b2WorldId* worldId;
    b2BodyDef* bodyDef;
};

class Object {
   protected:
    SDL_Renderer* renderer = NULL;

    std::string id;
    std::string name;
    std::vector<std::string> tags;

    b2BodyId bodyId;
    b2ShapeId shapeId;
    b2WorldId* worldId;

   public:
    Object(SDL_Renderer* renderer);
    Object(SDL_Renderer* renderer, ObjectProperties properties);
    virtual ~Object() = default;

    virtual void onEvent(SDL_Event* event);
    virtual void onUpdate();
    virtual void onRender();

    std::string getId() const;
    std::string getName() const;
    std::vector<std::string> getTags() const;
    void setId(const std::string& newId);
    void setName(const std::string& newName);
    void addTag(const std::string& tag);
    void removeTag(const std::string& tag);
    bool hasTag(const std::string& tag) const;

    virtual b2BodyId getBodyId() const;
    virtual b2ShapeId getShapeId() const;
    virtual const b2Vec2 getPosition() const;
    virtual const b2Polygon getPolygon() const;
    virtual void setPosition(b2Vec2 position);
    virtual void setXPosition(float x);
    virtual void setYPosition(float y);
};