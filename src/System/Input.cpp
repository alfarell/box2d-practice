#include "Input.hpp"

Input& Input::Get() {
    static Input instance;
    return instance;
}

void Input::processEvent(SDL_Renderer* renderer, SDL_Event* event) {
    Input::MouseState& mouseState = Get().mouseState;

    switch (event->type) {
        case SDL_EVENT_MOUSE_MOTION:
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            mouseState.windowEvent = event->button;
            mouseState.renderEvent = event->button;

            float renderX;
            float renderY;

            SDL_RenderCoordinatesFromWindow(renderer, mouseState.windowEvent.x,
                                            mouseState.windowEvent.y, &renderX,
                                            &renderY);

            mouseState.renderEvent.x = renderX;
            mouseState.renderEvent.y = renderY;

            break;
        }
    }
}

const Input::MouseState* Input::getMouseState() {
    return &Get().mouseState;
}