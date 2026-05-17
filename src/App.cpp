#include "App.hpp"

App::App(Window& window) : window(&window) {}

App::App(Window* window) : window(window) {}

App::~App() {
    this->onDestroy();
}

bool App::init() {
    this->setDefaultBackgroundColor(0xFFFFFFFF);

    this->physics.init();

    SDL_FRect groundRect = {};
    groundRect.x         = (this->window->getWidth() / 2) - (900.0f / 2);
    groundRect.y         = (this->window->getHeight()) - (100.0f);
    groundRect.w         = 900.0f;
    groundRect.h         = 100.0f;

    b2BodyDef bodyDef             = b2DefaultBodyDef();
    bodyDef.type                  = b2_staticBody;
    bodyDef.position              = b2Vec2{groundRect.x + (groundRect.w / 2),
                                           groundRect.y + (groundRect.h / 2)};
    b2ShapeDef shapeDef           = b2DefaultShapeDef();
    shapeDef.material.customColor = 0xFF00FF00;

    SquareProperties groundProperies = {};
    groundProperies.name             = "Ground";
    groundProperies.tags             = {"ground", "static"};
    groundProperies.worldId          = this->physics.getWorldId();
    groundProperies.bodyDef          = &bodyDef;
    groundProperies.shapeDef         = &shapeDef;
    groundProperies.width            = groundRect.w;
    groundProperies.height           = groundRect.h;

    objectManager.createObject<Square>(this->window->getSDLRenderer(),
                                       groundProperies);

    return true;
}

void App::onEvent(SDL_Event* event) {
    objectManager.each([event](Object* object) { object->onEvent(event); });
}

void App::onSimulate() {
    const auto simulate = [this](float deltaTime) {
        this->physics.step(deltaTime);
    };
    Frame::Get().accumulateTime(simulate);
}

void App::onUpdate() {
    const MouseClickState* mouseState = Input::getMouseClickEvent();
    if (mouseState->justPressed) {
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
        squareProperties.name             = "Box";
        squareProperties.tags             = {"box", "dynamic"};
        squareProperties.worldId          = this->physics.getWorldId();
        squareProperties.bodyDef          = &bodyDef;
        squareProperties.shapeDef         = &shapeDef;
        squareProperties.width            = boxWidth;
        squareProperties.height           = boxHeight;

        objectManager.createObject<Square>(this->window->getSDLRenderer(),
                                           squareProperties);
    }

    objectManager.each([&window = this->window](Object* object) {
        b2Vec2 position       = object->getPosition();
        bool isFallFromGround = position.x > window->getWidth() ||
                                position.x < 0 ||
                                position.y > window->getHeight();
        if (isFallFromGround) {
            object->deactivate();
        }
    });

    objectManager.each([](Object* object) { object->onUpdate(); });
}

void App::onRender() {
    this->renderDefaultBackground();

    objectManager.each([](Object* object) { object->onRender(); });

    SDL_RenderPresent(window->getSDLRenderer());
}

void App::onDestroy() {
    this->objectManager.onDestroy();
    this->physics.destroy();
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