#include "ObjectManager.hpp"

const ObjectList* ObjectManager::getObjects() const {
    return &this->objects;
}

void ObjectManager::removeObject(Object* object) {
    auto cb = [object](const std::unique_ptr<Object>& obj) {
        return obj.get() == object;
    };
    auto it = std::remove_if(this->objects.begin(), this->objects.end(), cb);
    this->objects.erase(it, this->objects.end());
}

void ObjectManager::removeObject(const uint32_t& id) {
    auto cb = [&id](const std::unique_ptr<Object>& obj) {
        return obj->getId() == id;
    };
    auto it = std::remove_if(this->objects.begin(), this->objects.end(), cb);
    this->objects.erase(it, this->objects.end());
}

void ObjectManager::removeObjectsWithName(const std::string& name) {
    auto cb = [&name](const std::unique_ptr<Object>& obj) {
        return obj->getName() == name;
    };
    auto it = std::remove_if(this->objects.begin(), this->objects.end(), cb);
    this->objects.erase(it, this->objects.end());
}

void ObjectManager::removeObjectsWithTag(const std::string& tag) {
    auto cb = [&tag](const std::unique_ptr<Object>& obj) {
        return obj->hasTag(tag);
    };
    auto it = std::remove_if(this->objects.begin(), this->objects.end(), cb);
    this->objects.erase(it, this->objects.end());
}

void ObjectManager::each(std::function<void(Object*)> callback) const {
    for (const auto& object : this->objects) {
        callback(object.get());
    }
}

void ObjectManager::each(std::function<void(const Object*)> callback) const {
    for (const auto& object : this->objects) {
        callback(object.get());
    }
}

void ObjectManager::onDestroy() {
    this->objects.clear();
}

void ObjectManager::removeInactiveObjects() {
    auto cb = [](const std::unique_ptr<Object>& obj) {
        return !obj->isActive();
    };
    auto it = std::remove_if(this->objects.begin(), this->objects.end(), cb);
    this->objects.erase(it, this->objects.end());
}