#include "App.hpp"

App::App(Window& window) : window(&window) {}

App::App(Window* window) : window(window) {}

App::~App() {
    this->onDestroy();
}

bool App::init() {
    this->setDefaultBackgroundColor(0xFFFFFFFF);

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity    = b2Vec2{0.0f, 9.8f};
    this->worldId       = b2CreateWorld(&worldDef);

    SDL_FRect groundRect = {};
    groundRect.x         = (this->window->getWidth() / 2) - (900.0f / 2);
    groundRect.y         = (this->window->getHeight()) - (100.0f);
    groundRect.w         = 900.0f;
    groundRect.h         = 100.0f;

    b2BodyDef bodyDef             = b2DefaultBodyDef();
    bodyDef.position              = b2Vec2{groundRect.x + (groundRect.w / 2),
                                           groundRect.y + (groundRect.h / 2)};
    b2ShapeDef shapeDef           = b2DefaultShapeDef();
    shapeDef.material.customColor = 0xFF00FF00;

    SquareProperties groundProperies = {};
    groundProperies.id               = "ground";
    groundProperies.name             = "Ground";
    groundProperies.tags             = {"ground", "static"};
    groundProperies.worldId          = &this->worldId;
    groundProperies.bodyDef          = &bodyDef;
    groundProperies.shapeDef         = shapeDef;
    groundProperies.width            = groundRect.w;
    groundProperies.height           = groundRect.h;

    std::unique_ptr<Square> ground = std::make_unique<Square>(
        this->window->getSDLRenderer(), groundProperies);

    objectManager.addObject(std::move(ground));

    return true;
}

void App::onEvent(SDL_Event* event) {
    const Input::MouseState* mouseState = Input::getMouseState();
    if (mouseState->renderEvent.down) {
        float boxWidth  = 30.0f;
        float boxHeight = 30.0f;

        b2BodyDef bodyDef    = b2DefaultBodyDef();
        bodyDef.gravityScale = 9.8f;
        bodyDef.type         = b2_dynamicBody;
        bodyDef.position =
            b2Vec2{mouseState->renderEvent.x, mouseState->renderEvent.y};

        b2ShapeDef shapeDef           = b2DefaultShapeDef();
        shapeDef.density              = 100.0f;
        shapeDef.material.friction    = 1.0f;
        shapeDef.material.restitution = 0.3f;
        shapeDef.material.customColor = 0xFFFF0F00;

        SquareProperties squareProperties = {};
        squareProperties.id               = "box";
        squareProperties.name             = "Box";
        squareProperties.tags             = {"box", "dynamic"};
        squareProperties.worldId          = &this->worldId;
        squareProperties.bodyDef          = &bodyDef;
        squareProperties.shapeDef         = shapeDef;
        squareProperties.width            = boxWidth;
        squareProperties.height           = boxHeight;

        std::unique_ptr<Square> newBox = std::make_unique<Square>(
            this->window->getSDLRenderer(), squareProperties);

        objectManager.addObject(std::move(newBox));
    }

    for (const auto& object : *this->objectManager.getObjects()) {
        object->onEvent(event);
    }
}

void App::onUpdate() {
    Frame::Get().calculateDeltaTime();
    const auto simulate = [&]() {
        b2World_Step(worldId, Frame::Get().getDeltaTime(), 8);
    };
    for (const auto& object : *this->objectManager.getObjects()) {
        object->onUpdate();
    }
    Frame::Get().accumulateTime(simulate);
}

void App::onRender() {
    this->renderDefaultBackground();

    for (const auto& object : *this->objectManager.getObjects()) {
        b2Vec2 position       = object->getPosition();
        bool isFallFromGround = position.x > window->getWidth() ||
                                position.x < 0 ||
                                position.y > window->getHeight();
        if (isFallFromGround) {
            object->setPosition({(float)this->window->getWidth() / 2, 0});
            continue;
        }
        object->onRender();
    }

    SDL_RenderPresent(window->getSDLRenderer());
}

void App::onDestroy() {
    objectManager.onDestroy();

    if (this->worldId.generation == b2_nullWorldId.generation &&
        this->worldId.index1 == b2_nullWorldId.index1) {
        return;
    }

    if (!b2World_IsValid(this->worldId)) {
        SDL_Log("Box2D world is invalid.");
        return;
    }

    b2DestroyWorld(worldId);
    worldId = b2_nullWorldId;
}

void App::setDefaultBackgroundColor(Color color) {
    this->properties.backgroundColor = color;
}

void App::renderDefaultBackground() {
    SDL_SetRenderDrawColor(
        this->window->getSDLRenderer(), properties.backgroundColor.r,
        properties.backgroundColor.g, properties.backgroundColor.b,
        properties.backgroundColor.a);
    SDL_RenderClear(this->window->getSDLRenderer());
}