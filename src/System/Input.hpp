#pragma once

#include <SDL3/SDL.h>

#include "../Helpers/ConstructorMacros.hpp"

class Input {
   public:
    struct MouseState {
        SDL_MouseButtonEvent windowEvent;
        SDL_MouseButtonEvent renderEvent;
    };

   private:
    MouseState mouseState;

    Input() = default;

   public:
    ~Input() = default;

    NON_COPYABLE(Input)
    NON_MOVABLE(Input)

    static Input& Get();

    static void processEvent(SDL_Renderer* renderer, SDL_Event* event);
    static const MouseState* getMouseState();
};