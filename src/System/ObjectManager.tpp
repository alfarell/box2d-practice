#include "ObjectManager.hpp"

template <typename T, typename... Args>
T* ObjectManager::createObject(Args&&... args) {
    static_assert(std::is_base_of<Object, T>::value,
                  "T must be a subclass of Object");

    auto object = std::make_unique<T>(std::forward<Args>(args)...);
    object->setId(this->nextId++);
    this->objects.push_back(std::move(object));

    T* ptr = static_cast<T*>(this->objects.back().get());
    return ptr;
}

template <typename T>
std::vector<T*> ObjectManager::getObjects() const {
    static_assert(std::is_base_of<Object, T>::value,
                  "T must be a subclass of Object");

    std::vector<T*> result;

    for (const auto& object : this->objects) {
        T* castedObject = dynamic_cast<T*>(object.get());

        if (!castedObject) continue;

        result.push_back(castedObject);
    }

    return result;
}

template <typename T>
T* ObjectManager::getObjectById(const uint32_t& id) const {
    static_assert(std::is_base_of<Object, T>::value,
                  "T must be a subclass of Object");

    for (const auto& object : this->objects) {
        if (object->getId() != id) continue;

        T* castedObject = dynamic_cast<T*>(object.get());

        if (!castedObject) return nullptr;

        return castedObject;
    }

    return nullptr;
}

template <typename T>
T* ObjectManager::getObjectByName(const std::string& name) const {
    static_assert(std::is_base_of<Object, T>::value,
                  "T must be a subclass of Object");

    for (const auto& object : this->objects) {
        if (object->getName() != name) continue;

        T* castedObject = dynamic_cast<T*>(object.get());

        if (!castedObject) return nullptr;

        return castedObject;
    }

    return nullptr;
}

template <typename T>
std::vector<T*> ObjectManager::getObjectsByTag(const std::string& tag) const {
    static_assert(std::is_base_of<Object, T>::value,
                  "T must be a subclass of Object");

    std::vector<T*> result;

    for (const auto& object : this->objects) {
        if (!object->hasTag(tag)) continue;

        T* castedObject = dynamic_cast<T*>(object.get());

        if (!castedObject) continue;

        result.push_back(castedObject);
    }

    return result;
}
