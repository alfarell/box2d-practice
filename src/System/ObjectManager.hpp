#pragma once

#include <SDL3/SDL.h>
#include <box2d/box2d.h>

#include <memory>
#include <vector>

#include "../Components/Object.hpp"
#include "../Helpers/ConstructorMacros.hpp"

using ObjectList = std::vector<std::unique_ptr<Object>>;

class ObjectManager {
   private:
    uint32_t nextId = 0;
    std::shared_ptr<ObjectList> objects;

   public:
    ObjectManager();
    ~ObjectManager();

    NON_COPYABLE(ObjectManager)
    NON_MOVABLE(ObjectManager)

    const ObjectList& getObjects() const;
    void createObject(std::unique_ptr<Object> object);
    void removeObject(Object* object);
    void removeObject(const uint32_t& id);
    void removeObjectsWithName(const std::string& name);
    void removeObjectsWithTag(const std::string& tag);

    void onEvent(SDL_Event* event);
    void onUpdate();
    void onRender(SDL_Renderer* renderer);
    void onDestroy();

   private:
    void clearDeactivatedObjects();
};