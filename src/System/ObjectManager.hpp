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
    ObjectList objects;

   public:
    ObjectManager() = default;
    ~ObjectManager();

    NON_COPYABLE(ObjectManager)
    NON_MOVABLE(ObjectManager)

    const ObjectList* getObjects() const;
    template <typename T>
    std::vector<T*> getObjects() const;
    template <typename T>
    T* getObjectById(const uint32_t& id) const;
    template <typename T>
    T* getObjectByName(const std::string& name) const;
    template <typename T>
    std::vector<T*> getObjectsByTag(const std::string& tag) const;

    template <typename T, typename... Args>
    T* createObject(Args&&... args);
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

#include "ObjectManager.tpp"