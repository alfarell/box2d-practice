#include "Object.hpp"

Object::Object(SDL_Renderer* renderer) : renderer(renderer) {}

Object::Object(SDL_Renderer* renderer, ObjectProperties properties)
    : renderer(renderer), worldId(properties.worldId) {
    b2BodyId newBodyId = b2CreateBody(*worldId, properties.bodyDef);
    this->bodyId       = newBodyId;
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

void Object::update() {
    // Default implementation does nothing
}

void Object::render() {
    b2Polygon boxPolygon = b2Shape_GetPolygon(this->shapeId);

    if (!boxPolygon.count) return;

    b2Vec2 entityPosition      = b2Body_GetPosition(this->bodyId);
    b2Rot entityRotation       = b2Body_GetRotation(this->bodyId);
    b2SurfaceMaterial material = b2Shape_GetSurfaceMaterial(this->shapeId);
    uint32_t color = material.customColor ? material.customColor : 0xFF0000;

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

    SDL_SetRenderDrawColor(this->renderer, (color >> 24) & 0xFF,
                           (color >> 16) & 0xFF, (color >> 8) & 0xFF,
                           color & 0xFF);
    SDL_RenderLines(this->renderer, sdlVertices.data(),
                    (int)sdlVertices.size());
}