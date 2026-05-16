#include "Square.hpp"

Square::Square(SDL_Renderer* renderer, SquareProperties properties)
    : Object(renderer, (ObjectProperties)properties) {
    b2Polygon newPolygon =
        b2MakeBox(properties.width / 2, properties.height / 2);

    this->shapeId = b2CreatePolygonShape(this->getBodyId(),
                                         &properties.shapeDef, &newPolygon);
}

Square::~Square() {
    if (this->shapeId.index1 == b2_nullShapeId.index1 &&
        this->shapeId.world0 == b2_nullShapeId.world0 &&
        this->shapeId.generation == b2_nullShapeId.generation) {
        return;
    }

    if (!b2Shape_IsValid(this->shapeId)) {
        SDL_Log("Box2D shape with ID: %u is invalid.", this->shapeId.index1);
        return;
    }

    b2DestroyShape(this->shapeId, true);
}

void Square::setSize(float newWidth, float newHeight) {
    b2Polygon newPolygon = b2MakeBox(newWidth / 2, newHeight / 2);
    b2Shape_SetPolygon(this->shapeId, &newPolygon);
}

void Square::setWidth(float newWidth) {
    b2Polygon currentPolygon = this->getPolygon();
    float currentHeight      = currentPolygon.vertices[0].y * 2;
    this->setSize(newWidth, currentHeight);
}

void Square::setHeight(float newHeight) {
    b2Polygon currentPolygon = this->getPolygon();
    float currentWidth       = currentPolygon.vertices[0].x * 2;
    this->setSize(currentWidth, newHeight);
}