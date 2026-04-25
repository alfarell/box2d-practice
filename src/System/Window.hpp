#pragma once

#include <SDL3/SDL.h>

#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720
#define DEFAULT_WINDOW_TITLE "SDL App"
#define DEFAULT_APP_NAME "SDL App"
#define DEFAULT_APP_VERSION "1.0"
#define DEFAULT_APP_ORGANIZATION "com.example.sdl"

typedef struct WindowMetadata {
    int width;
    int height;
    const char* title;
    const char* name;
    const char* version;
    const char* organization;
} WindowMetadata;

class Window {
   private:
    SDL_Window* window            = NULL;
    SDL_Renderer* renderer        = NULL;
    WindowMetadata windowMetadata = {
        DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, DEFAULT_WINDOW_TITLE,
        DEFAULT_APP_NAME,     DEFAULT_APP_VERSION,   DEFAULT_APP_ORGANIZATION};

   public:
    Window() = default;
    Window(const WindowMetadata& metadata);
    ~Window();

    Window(const Window&)            = delete;
    Window& operator=(const Window&) = delete;

    bool create();
    void destroy();

    SDL_Window* getSDLWindow() const;
    SDL_Renderer* getSDLRenderer() const;
    void setWindowMetadata(const WindowMetadata& newMetadata);
    void setWidth(int newWidth);
    void setHeight(int newHeight);
    void setSize(int newWidth, int newHeight);
    void setTitle(const char* newTitle);
    int getWidth() const;
    int getHeight() const;
    const char* getTitle() const;
};