#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <box2d/box2d.h>

#include <vector>

#include "App.hpp"
#include "System/Frame.hpp"
#include "System/Input.hpp"
#include "System/Window.hpp"

WindowMetadata windowMetadata = {
    DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, DEFAULT_WINDOW_TITLE,
    DEFAULT_APP_NAME,     DEFAULT_APP_VERSION,   DEFAULT_APP_ORGANIZATION};

Window window = Window(windowMetadata);
App app       = App(window);

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    if (!window.create()) {
        return SDL_APP_FAILURE;
    }

    Frame::Get().init();

    if (!app.init()) {
        return SDL_APP_FAILURE;
    }

    /* carry on with the program! */
    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    if (event->type == SDL_EVENT_QUIT) {
        /* end the program, reporting success to the OS. */
        return SDL_APP_SUCCESS;
    }

    Input::Get().Update(event);

    /* carry on with the program! */
    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate) {
    app.run();

    /* carry on with the program! */
    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    /* SDL will clean up the window/renderer for us. */
    app.destroy();
    window.destroy();
}
