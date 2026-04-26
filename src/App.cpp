#include "App.hpp"

App::App(Window& window) : window(&window) {}

App::App(Window* window) : window(window) {}

App::~App() {
    this->onDestroy();
}

bool App::init() {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity    = b2Vec2{0.0f, 9.8f};
    this->worldId       = b2CreateWorld(&worldDef);

    SDL_FRect groundRect          = SDL_FRect{0.0f, 600.0f, 1280.0f, 120.0f};
    b2BodyDef bodyDef             = b2DefaultBodyDef();
    bodyDef.position              = b2Vec2{groundRect.x + (groundRect.w / 2),
                                           groundRect.y + (groundRect.h / 2)};
    b2ShapeDef shapeDef           = b2DefaultShapeDef();
    shapeDef.material.customColor = 0x00FF0000;

    SquareProperties groundProperies = {&this->worldId, &bodyDef, groundRect.w,
                                        groundRect.h, shapeDef};
    std::unique_ptr<Square> ground   = std::make_unique<Square>(
        this->window->getSDLRenderer(), groundProperies);
    boxes.push_back(std::move(ground));

    return true;
}

void App::onEvent(SDL_Event* event) {
    // SDL_Event* event = Input::Get().Event();
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        switch (event->button.button) {
            case SDL_BUTTON_LEFT: {
                float boxWidth  = 30.0f;
                float boxHeight = 30.0f;

                b2BodyDef bodyDef    = b2DefaultBodyDef();
                bodyDef.gravityScale = 9.8f;
                bodyDef.type         = b2_dynamicBody;
                bodyDef.position     = b2Vec2{event->button.x, event->button.y};

                b2ShapeDef shapeDef           = b2DefaultShapeDef();
                shapeDef.density              = 100.0f;
                shapeDef.material.friction    = 1.0f;
                shapeDef.material.restitution = 0.3f;
                shapeDef.material.customColor = 0xFF0F0F00;

                SquareProperties squareProperties = {
                    &worldId, &bodyDef, boxWidth, boxHeight, shapeDef};
                std::unique_ptr<Square> newBox = std::make_unique<Square>(
                    this->window->getSDLRenderer(), squareProperties);

                boxes.push_back(std::move(newBox));

                break;
            }
            default:
                break;
        }
    }
}

void App::onUpdate() {
    Frame::Get().calculateDeltaTime();
    const auto simulate = [&]() {
        b2World_Step(worldId, Frame::Get().getDeltaTime(), 8);
    };
    Frame::Get().accumulateTime(simulate);
}

void App::onRender() {
    SDL_SetRenderDrawColor(window->getSDLRenderer(), 255, 255, 255,
                           SDL_ALPHA_OPAQUE);
    SDL_RenderClear(window->getSDLRenderer());

    for (const auto& box : boxes) {
        box->onRender();
    }

    SDL_RenderPresent(window->getSDLRenderer());
}

void App::onDestroy() {
    boxes.clear();

    if (this->worldId.generation == b2_nullWorldId.generation &&
        this->worldId.index1 == b2_nullWorldId.index1) {
        SDL_Log("Box2D world is already null.");
        return;
    }

    if (!b2World_IsValid(this->worldId)) {
        SDL_Log("Box2D world is invalid.");
        return;
    }

    b2DestroyWorld(worldId);
    worldId = b2_nullWorldId;
}