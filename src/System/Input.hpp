#pragma once

#include <SDL3/SDL.h>

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

    Input(const Input&)            = delete;
    Input& operator=(const Input&) = delete;

    static Input& Get();

    static void processEvent(SDL_Renderer* renderer, SDL_Event* event);
    static const MouseState* getMouseState();
};