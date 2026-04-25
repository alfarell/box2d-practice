#pragma once

#include <SDL3/SDL.h>

class Input {
   private:
    SDL_Event* currentEvent;

    Input() = default;

   public:
    ~Input() = default;

    Input(const Input&)            = delete;
    Input& operator=(const Input&) = delete;

    static Input& Get();

    static void Update(SDL_Event* event);
    static SDL_Event* Event();
};