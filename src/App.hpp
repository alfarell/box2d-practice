#pragma once

#include <SDL3/SDL.h>
#include <box2d/box2d.h>

#include <memory>
#include <vector>

#include "Components/Object.hpp"
#include "Components/Square.hpp"
#include "Helpers/ConstructorMacros.hpp"
#include "System/Color.hpp"
#include "System/Frame.hpp"
#include "System/Input.hpp"
#include "System/ObjectManager.hpp"
#include "System/Window.hpp"

struct AppProperties {
    Color backgroundColor;
};

class App {
   private:
    Window* window;
    AppProperties properties;

    b2WorldId worldId;

    ObjectManager objectManager;

   public:
    App(Window& window);
    App(Window* window);
    ~App();

    NON_COPYABLE(App)
    NON_MOVABLE(App)

    bool init();
    void onEvent(SDL_Event* event);
    void onSimulate();
    void onUpdate();
    void onRender();
    void onDestroy();

   private:
    void setDefaultBackgroundColor(Color color);
    void renderDefaultBackground();
};