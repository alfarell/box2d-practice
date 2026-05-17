#include "Input.hpp"

Input& Input::Get() {
    static Input instance;
    return instance;
}

void Input::processEvent(SDL_Renderer* renderer, SDL_Event* event) {
    Input::processMouseClickEvent(renderer, event);
    Input::processMouseMotionEvent(renderer, event);
}

void Input::processMouseClickEvent(SDL_Renderer* renderer, SDL_Event* event) {
    if (event->type != SDL_EVENT_MOUSE_BUTTON_DOWN &&
        event->type != SDL_EVENT_MOUSE_BUTTON_UP) {
        return;
    }

    MouseClickState& state = Get().mouseClickState;
    state.windowEvent      = event->button;

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) state.justPressed = true;
    if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) state.justReleased = true;

    float renderX = state.windowEvent.x;
    float renderY = state.windowEvent.y;

    SDL_RenderCoordinatesFromWindow(renderer, state.windowEvent.x,
                                    state.windowEvent.y, &renderX, &renderY);

    state.renderEvent   = state.windowEvent;
    state.renderEvent.x = renderX;
    state.renderEvent.y = renderY;
}

void Input::processMouseMotionEvent(SDL_Renderer* renderer, SDL_Event* event) {
    if (event->type != SDL_EVENT_MOUSE_MOTION) return;

    MouseMotionState& state = Get().mouseMotionState;

    state.windowX     = event->motion.x;
    state.windowY     = event->motion.y;
    state.windowXRel  = event->motion.xrel;
    state.windowYRel  = event->motion.yrel;
    state.buttonState = event->motion.state;

    float renderX = state.windowX;
    float renderY = state.windowY;

    SDL_RenderCoordinatesFromWindow(renderer, state.windowX, state.windowY,
                                    &renderX, &renderY);

    state.renderX = renderX;
    state.renderY = renderY;
}

const MouseClickState* Input::getMouseClickEvent() {
    return &Get().mouseClickState;
}

const MouseMotionState* Input::getMouseMotionEvent() {
    return &Get().mouseMotionState;
}

void Input::resetPerFrameState() {
    MouseClickState& state = Get().mouseClickState;
    state.justPressed      = false;
    state.justReleased     = false;
}