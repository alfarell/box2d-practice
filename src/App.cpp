#include "App.hpp"

App::App(Window& window) : window(&window) {}

App::App(Window* window) : window(window) {}

App::~App() {
    this->destroy();
}

bool App::init() {
    this->groundRect = SDL_FRect{0.0f, 600.0f, 1280.0f, 120.0f};

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity    = b2Vec2{0.0f, 9.8f};
    this->worldId       = b2CreateWorld(&worldDef);

    b2BodyDef groundBodyDef = b2DefaultBodyDef();
    groundBodyDef.position =
        b2Vec2{this->groundRect.x + (this->groundRect.w / 2),
               this->groundRect.y + (this->groundRect.h / 2)};
    groundId = b2CreateBody(worldId, &groundBodyDef);
    b2Polygon groundBox =
        b2MakeBox(this->groundRect.w / 2, this->groundRect.h / 2);
    b2ShapeDef groundShapeDef           = b2DefaultShapeDef();
    groundShapeDef.material.customColor = 0x00FF0000;
    b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

    SquareProperties groundProperies = {&worldId, &groundBodyDef,
                                        this->groundRect.w, this->groundRect.h,
                                        groundShapeDef};
    std::unique_ptr<Square> ground   = std::make_unique<Square>(
        this->window->getSDLRenderer(), groundProperies);
    boxes.push_back(std::move(ground));

    return true;
}

void App::run() {
    SDL_Event* event = Input::Get().Event();
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        switch (event->button.button) {
            case SDL_BUTTON_LEFT: {
                float boxWidth  = 30.0f;
                float boxHeight = 30.0f;

                b2BodyDef bodyDef    = b2DefaultBodyDef();
                bodyDef.gravityScale = 9.8f;
                bodyDef.type         = b2_dynamicBody;
                bodyDef.position     = b2Vec2{event->button.x, event->button.y};

                b2ShapeDef entityShapeDef           = b2DefaultShapeDef();
                entityShapeDef.density              = 100.0f;
                entityShapeDef.material.friction    = 1.0f;
                entityShapeDef.material.restitution = 0.3f;
                entityShapeDef.material.customColor = 0xFF0F0F00;

                SquareProperties squareProperties = {
                    &worldId, &bodyDef, boxWidth, boxHeight, entityShapeDef};
                std::unique_ptr<Square> newBox = std::make_unique<Square>(
                    this->window->getSDLRenderer(), squareProperties);

                boxes.push_back(std::move(newBox));

                break;
            }
            default:
                break;
        }
    }

    Frame::Get().calculateDeltaTime();
    const auto simulate = [&]() {
        b2World_Step(worldId, Frame::Get().getDeltaTime(), 8);
    };
    Frame::Get().accumulateTime(simulate);

    SDL_SetRenderDrawColor(window->getSDLRenderer(), 255, 255, 255,
                           SDL_ALPHA_OPAQUE);
    SDL_RenderClear(window->getSDLRenderer());

    for (const auto& box : boxes) {
        box->render();
    }

    SDL_RenderPresent(window->getSDLRenderer());
}

void App::destroy() {
    if (this->worldId.generation == b2_nullWorldId.generation &&
        this->worldId.index1 == b2_nullWorldId.index1) {
        SDL_Log("Attempted to destroy an invalid Box2D world.");
        return;
    }

    if (!b2World_IsValid(this->worldId)) {
        SDL_Log("Attempted to destroy an invalid Box2D world.");
        return;
    }

    b2DestroyWorld(worldId);
    worldId = b2_nullWorldId;
}