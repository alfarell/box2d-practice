#include "Input.hpp"

Input& Input::Get() {
    static Input instance;
    return instance;
}

void Input::Update(SDL_Event* event) {
    Get().currentEvent = event;
}

SDL_Event* Input::Event() {
    return Get().currentEvent;
}