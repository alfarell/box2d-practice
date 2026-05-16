#pragma once

#include <SDL3/SDL.h>

struct Color {
   private:
    uint32_t hexColor;

   public:
    const Uint8 r;
    const Uint8 g;
    const Uint8 b;
    const Uint8 a;

    Color() : hexColor(0xFF000000), r(0), g(0), b(0), a(255) {}

    Color(uint32_t hexColor)
        : hexColor(hexColor),
          r((hexColor >> 16) & 0xFF),
          g((hexColor >> 8) & 0xFF),
          b(hexColor & 0xFF),
          a((hexColor >> 24) & 0xFF) {}

    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
        : hexColor((a << 24) | (r << 16) | (g << 8) | b),
          r(r),
          g(g),
          b(b),
          a(a) {}

    Color(const Color& other)
        : hexColor(other.hexColor),
          r(other.r),
          g(other.g),
          b(other.b),
          a(other.a) {}

    Color& operator=(const Color& other) {
        hexColor              = other.hexColor;
        const_cast<Uint8&>(r) = other.r;
        const_cast<Uint8&>(g) = other.g;
        const_cast<Uint8&>(b) = other.b;
        const_cast<Uint8&>(a) = other.a;
        return *this;
    }

    operator uint32_t() const {
        return hexColor;
    }

    operator SDL_Color() const {
        return SDL_Color{r, g, b, a};
    }
};