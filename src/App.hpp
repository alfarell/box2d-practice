#pragma once

#include <SDL3/SDL.h>
#include <box2d/box2d.h>

#include <memory>
#include <vector>

#include "Components/Object.hpp"
#include "Components/Square.hpp"
#include "System/Frame.hpp"
#include "System/Input.hpp"
#include "System/Window.hpp"

class App {
   private:
    Window* window;

    b2WorldId worldId;

    std::vector<std::unique_ptr<Object>> boxes;

   public:
    App(Window& window);
    App(Window* window);
    ~App();

    App(const App&)            = delete;
    App& operator=(const App&) = delete;

    bool init();
    void onEvent(SDL_Event* event);
    void onUpdate();
    void onRender();
    void onDestroy();
};