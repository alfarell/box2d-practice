#pragma once

#include <SDL3/SDL.h>
#include <box2d/box2d.h>

#include "Object.hpp"

struct SquareProperties : public ObjectProperties {
    float width;
    float height;
    b2ShapeDef& shapeDef;
};

class Square : public Object {
   private:
    b2ShapeDef shapeDef;

   public:
    Square(SDL_Renderer* renderer, SquareProperties properties);
    ~Square() = default;

    Square(const Square&)            = delete;
    Square& operator=(const Square&) = delete;

    void setSize(float newWidth, float newHeight);
    void setWidth(float newWidth);
    void setHeight(float newHeight);
};