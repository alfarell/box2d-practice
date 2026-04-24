#include "Window.hpp"

Window& Window::Get() {
    static Window instance;
    return instance;
}

Window::Window(const WindowMetadata& metadata) : windowMetadata(metadata) {}

bool Window::create() {
    SDL_SetAppMetadata(this->windowMetadata.name, this->windowMetadata.version,
                       this->windowMetadata.organization);

    if (this->window != NULL || this->renderer != NULL) {
        SDL_Log("Window and renderer already created!");
        return false;
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return false;
    }

    if (!SDL_CreateWindowAndRenderer(
            this->windowMetadata.title, this->windowMetadata.width,
            this->windowMetadata.height, SDL_WINDOW_RESIZABLE, &this->window,
            &this->renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return false;
    }
    SDL_SetRenderLogicalPresentation(this->renderer, this->windowMetadata.width,
                                     this->windowMetadata.height,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);

    return true;
}

SDL_Window* Window::getSDLWindow() const {
    return this->window;
}

SDL_Renderer* Window::getSDLRenderer() const {
    return this->renderer;
}

void Window::setWidth(int newWidth) {
    this->windowMetadata.width = newWidth;
    SDL_SetRenderLogicalPresentation(this->renderer, this->windowMetadata.width,
                                     this->windowMetadata.height,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);
}

void Window::setHeight(int newHeight) {
    this->windowMetadata.height = newHeight;
    SDL_SetRenderLogicalPresentation(this->renderer, this->windowMetadata.width,
                                     this->windowMetadata.height,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);
}

void Window::setSize(int newWidth, int newHeight) {
    this->windowMetadata.width  = newWidth;
    this->windowMetadata.height = newHeight;
    SDL_SetRenderLogicalPresentation(this->renderer, this->windowMetadata.width,
                                     this->windowMetadata.height,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);
}

int Window::getWidth() const {
    return this->windowMetadata.width;
}

int Window::getHeight() const {
    return this->windowMetadata.height;
}

void Window::setTitle(const char* newTitle) {
    this->windowMetadata.title = newTitle;
    SDL_SetWindowTitle(this->window, this->windowMetadata.title);
}

const char* Window::getTitle() const {
    return this->windowMetadata.title;
}