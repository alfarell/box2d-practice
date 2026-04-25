#include "Square.hpp"

Square::Square(SDL_Renderer* renderer, SquareProperties properties)
    : Object(renderer, (ObjectProperties)properties),
      shapeDef(properties.shapeDef) {
    b2Polygon newPolygon =
        b2MakeBox(properties.width / 2, properties.height / 2);

    this->shapeId = b2CreatePolygonShape(this->getBodyId(),
                                         &properties.shapeDef, &newPolygon);
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