#pragma once

#include <SDL3/SDL.h>
#include <box2d/box2d.h>

#include <memory>
#include <vector>

#include "../Components/Object.hpp"

class ObjectManager {
   private:
    std::shared_ptr<std::vector<std::unique_ptr<Object>>> objects;

   public:
    ObjectManager();
    ~ObjectManager();

    ObjectManager(const ObjectManager&)            = delete;
    ObjectManager& operator=(const ObjectManager&) = delete;

    std::shared_ptr<std::vector<std::unique_ptr<Object>>> getObjects() const;
    void addObject(std::unique_ptr<Object> object);
    void addObject(Object object);
    void removeObject(Object* object);
    void removeObject(const std::string& id);
    void removeObjectsWithName(const std::string& name);
    void removeObjectsWithTag(const std::string& tag);
    void onDestroy();
};