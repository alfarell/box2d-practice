#pragma once

#include <SDL3/SDL.h>
#include <box2d/box2d.h>

#include <string>
#include <vector>

#include "../System/Color.hpp"

class ObjectManager;

struct ObjectProperties {
    std::string name;
    std::vector<std::string> tags;
    b2WorldId* worldId;
    b2BodyDef* bodyDef;
};

class Object {
   protected:
    SDL_Renderer* renderer = NULL;

    uint32_t id;
    std::string name;
    std::vector<std::string> tags;
    bool active;

    b2BodyId bodyId;
    b2ShapeId shapeId;
    b2WorldId* worldId;

   protected:
    friend class ObjectManager;
    void setId(uint32_t newId);

   public:
    Object(SDL_Renderer* renderer);
    Object(SDL_Renderer* renderer, ObjectProperties properties);
    virtual ~Object() = default;

    virtual void onEvent(SDL_Event* event);
    virtual void onUpdate();
    virtual void onRender();

    uint32_t getId() const;
    std::string getName() const;
    std::vector<std::string> getTags() const;
    bool isActive() const;
    void setName(const std::string& newName);
    void addTag(const std::string& tag);
    void removeTag(const std::string& tag);
    bool hasTag(const std::string& tag) const;
    void activate();
    void deactivate();

    virtual b2BodyId getBodyId() const;
    virtual b2ShapeId getShapeId() const;
    virtual const b2Vec2 getPosition() const;
    virtual const b2Polygon getPolygon() const;
    virtual void setPosition(b2Vec2 position);
    virtual void setXPosition(float x);
    virtual void setYPosition(float y);
};