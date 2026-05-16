#include "Object.hpp"

Object::Object(SDL_Renderer* renderer) : renderer(renderer) {}

Object::Object(SDL_Renderer* renderer, ObjectProperties properties)
    : renderer(renderer),
      worldId(properties.worldId),
      name(properties.name),
      tags(properties.tags),
      active(true) {
    this->bodyId = b2CreateBody(*worldId, properties.bodyDef);
}

Object::~Object() {
    if (this->bodyId.index1 == b2_nullBodyId.index1 &&
        this->bodyId.world0 == b2_nullBodyId.world0 &&
        this->bodyId.generation == b2_nullBodyId.generation) {
        return;
    }

    if (!b2Body_IsValid(this->bodyId)) {
        SDL_Log("Box2D body with ID: %u is invalid.", this->bodyId.index1);
        return;
    }

    b2DestroyBody(this->bodyId);
}

uint32_t Object::getId() const {
    return this->id;
}

std::string Object::getName() const {
    return this->name;
}

std::vector<std::string> Object::getTags() const {
    return this->tags;
}

void Object::setId(uint32_t newId) {
    this->id = newId;
}

void Object::setName(const std::string& newName) {
    this->name = newName;
}

void Object::addTag(const std::string& tag) {
    if (!hasTag(tag)) {
        this->tags.push_back(tag);
    }
}

void Object::removeTag(const std::string& tag) {
    this->tags.erase(std::remove(this->tags.begin(), this->tags.end(), tag),
                     this->tags.end());
}

bool Object::hasTag(const std::string& tag) const {
    return std::find(this->tags.begin(), this->tags.end(), tag) !=
           this->tags.end();
}

bool Object::isActive() const {
    return this->active;
}

void Object::activate() {
    this->active = true;
}

void Object::deactivate() {
    this->active = false;
}

b2BodyId Object::getBodyId() const {
    return this->bodyId;
}

b2ShapeId Object::getShapeId() const {
    return this->shapeId;
}

const b2Vec2 Object::getPosition() const {
    return b2Body_GetPosition(this->bodyId);
}

void Object::setPosition(b2Vec2 position) {
    b2Rot rotation = b2Body_GetRotation(this->bodyId);
    b2Body_SetTransform(this->bodyId, position, rotation);
}

void Object::setXPosition(float x) {
    b2Vec2 currentPosition = b2Body_GetPosition(this->bodyId);
    b2Vec2 newPosition     = {x, currentPosition.y};
    b2Rot rotation         = b2Body_GetRotation(this->bodyId);
    b2Body_SetTransform(this->bodyId, newPosition, rotation);
}

void Object::setYPosition(float y) {
    b2Vec2 currentPosition = b2Body_GetPosition(this->bodyId);
    b2Vec2 newPosition     = {currentPosition.x, y};
    b2Rot rotation         = b2Body_GetRotation(this->bodyId);
    b2Body_SetTransform(this->bodyId, newPosition, rotation);
}

const b2Polygon Object::getPolygon() const {
    return b2Shape_GetPolygon(this->shapeId);
}

void Object::onEvent(SDL_Event* event) {
    // Default implementation does nothing
}

void Object::onUpdate() {
    // Default implementation does nothing
}

void Object::onRender() {
    b2Polygon boxPolygon = b2Shape_GetPolygon(this->shapeId);

    if (!boxPolygon.count) return;

    b2Vec2 entityPosition      = b2Body_GetPosition(this->bodyId);
    b2Rot entityRotation       = b2Body_GetRotation(this->bodyId);
    b2SurfaceMaterial material = b2Shape_GetSurfaceMaterial(this->shapeId);
    Color materialColor =
        material.customColor ? material.customColor : 0xFF00FF00;

    for (int i = 0; i < boxPolygon.count; ++i) {
        // Rotate the vertex
        float rotatedX = boxPolygon.vertices[i].x * entityRotation.c -
                         boxPolygon.vertices[i].y * entityRotation.s;
        float rotatedY = boxPolygon.vertices[i].x * entityRotation.s +
                         boxPolygon.vertices[i].y * entityRotation.c;

        // Translate to world position
        boxPolygon.vertices[i].x = rotatedX + entityPosition.x;
        boxPolygon.vertices[i].y = rotatedY + entityPosition.y;
    }

    std::vector<SDL_FPoint> sdlVertices;
    for (int i = 0; i < boxPolygon.count; ++i) {
        sdlVertices.push_back(
            SDL_FPoint{boxPolygon.vertices[i].x, boxPolygon.vertices[i].y});
    }
    sdlVertices.push_back(sdlVertices[0]);

    SDL_SetRenderDrawColor(this->renderer, materialColor.r, materialColor.g,
                           materialColor.b, materialColor.a);
    SDL_RenderLines(this->renderer, sdlVertices.data(),
                    static_cast<unsigned int>(sdlVertices.size()));
}