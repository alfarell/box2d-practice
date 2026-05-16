#pragma once

#include <SDL3/SDL.h>
#include <box2d/box2d.h>

#include "Object.hpp"

struct SquareProperties : public ObjectProperties {
    float width;
    float height;
    b2ShapeDef shapeDef;
};

class Square : public Object {
   public:
    Square(SDL_Renderer* renderer, SquareProperties properties);
    virtual ~Square();

    void setSize(float newWidth, float newHeight);
    void setWidth(float newWidth);
    void setHeight(float newHeight);
};