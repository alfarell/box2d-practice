#pragma once

#include <SDL3/SDL.h>

#include "../Helpers/ConstructorMacros.hpp"

struct MouseClickState {
    SDL_MouseButtonEvent windowEvent{};
    SDL_MouseButtonEvent renderEvent{};
    bool justPressed{};
    bool justReleased{};
};

struct MouseMotionState {
    float windowX{};
    float windowY{};
    float windowXRel{};
    float windowYRel{};
    float renderX{};
    float renderY{};
    SDL_MouseButtonFlags buttonState{};
};

class Input {
   private:
    MouseClickState mouseClickState;
    MouseMotionState mouseMotionState;

    Input() = default;

    static void processMouseClickEvent(SDL_Renderer* renderer,
                                       SDL_Event* event);
    static void processMouseMotionEvent(SDL_Renderer* renderer,
                                        SDL_Event* event);

   public:
    ~Input() = default;

    NON_COPYABLE(Input)
    NON_MOVABLE(Input)

    static Input& Get();

    static void processEvent(SDL_Renderer* renderer, SDL_Event* event);

    static const MouseClickState* getMouseClickEvent();
    static const MouseMotionState* getMouseMotionEvent();
    static void resetPerFrameState();
};