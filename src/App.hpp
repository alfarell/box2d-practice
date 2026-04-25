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

    SDL_FRect groundRect;
    b2WorldId worldId;
    b2BodyId groundId;

    std::vector<std::unique_ptr<Object>> boxes;

   public:
    App(Window& window);
    App(Window* window);
    ~App();

    App(const App&)            = delete;
    App& operator=(const App&) = delete;

    bool init();
    void run();
    void destroy();
};