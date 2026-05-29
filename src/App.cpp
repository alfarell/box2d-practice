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
        this->onCollision();
        this->onContact();
    };
    Frame::Get().accumulateTime(simulate);
}

void App::onCollision() {
    objectManager.each([](Object* object, const ObjectManager* self) {
        if (!object->isActive()) return;

        const Uint32 bodyContactCapacity = b2Body_GetContactCapacity(object->getBodyId());

        std::vector<b2ContactData> contacts = {};
        contacts.resize(bodyContactCapacity);

        b2Body_GetContactData(
            object->getBodyId(), 
            contacts.data(),
            bodyContactCapacity
        );

        const ObjectList* objects = self->getObjects();

        for (const b2ContactData& contact : contacts) {
            if (contact.shapeIdA == b2_nullShapeId ||
                contact.shapeIdB == b2_nullShapeId) {
                continue;
            }
            if (!b2Shape_IsValid(contact.shapeIdA)) {
                SDL_Log("Box2D shape with ID: %u is invalid.", contact.shapeIdA.index1);
                continue;
            }
            if (!b2Shape_IsValid(contact.shapeIdB)) {
                SDL_Log("Box2D shape with ID: %u is invalid.", contact.shapeIdB.index1);
                continue;
            }

            const b2ShapeId currentObjectShapeId = object->getShapeId();
            if (currentObjectShapeId == contact.shapeIdA &&
                currentObjectShapeId == contact.shapeIdB)
                continue;

            Object* otherA = nullptr;
            Object* otherB = nullptr;

            for (const auto& c : *objects) {
                if (c->getShapeId() == contact.shapeIdA) otherA = c.get();
                if (c->getShapeId() == contact.shapeIdB) otherB = c.get();
                if (otherA && otherB) break;
            }

            if (otherA && otherA != object) object->onCollision(otherA);
            if (otherB && otherB != object) object->onCollision(otherB);
        };
    });
}

void App::onContact() {
    objectManager.each([](Object* object, const ObjectManager* self) {
        if (!object->isActive()) return;

        self->each([object](Object* other) {
            if (object == other) return;

            const b2ShapeId objectShapeId = object->getShapeId();
            const b2ShapeId otherShapeId  = other->getShapeId();

            if (!b2Shape_IsValid(objectShapeId)) {
                SDL_Log("Box2D shape with ID: %u is invalid.",
                        objectShapeId.index1);
                return;
            }
            if (!b2Shape_IsValid(otherShapeId)) {
                SDL_Log("Box2D shape with ID: %u is invalid.",
                        otherShapeId.index1);
                return;
            }

            if (b2AABB_Overlaps(b2Shape_GetAABB(objectShapeId),
                                b2Shape_GetAABB(otherShapeId))) {
                object->onContact(other);
                other->onContact(object);
            }
        });
    });
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
            object->destroy();
        }
    });

    this->objectManager.removeDestroyedObjects();

    objectManager.each([](Object* object) { object->onUpdate(); });
}

void App::onRender() {
    this->renderDefaultBackground();

    objectManager.each([](Object* object) { object->onRender(); });

    SDL_RenderPresent(window->getSDLRenderer());
}

void App::onDestroy() {
    this->objectManager.destroy();
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