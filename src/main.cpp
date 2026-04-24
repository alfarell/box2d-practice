#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <box2d/box2d.h>

#include <vector>

#include "System/Frame.hpp"
#include "System/Window.hpp"

WindowMetadata windowMetadata = {
    DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, DEFAULT_WINDOW_TITLE,
    DEFAULT_APP_NAME,     DEFAULT_APP_VERSION,   DEFAULT_APP_ORGANIZATION};

Window window = Window(windowMetadata);
Frame& frame  = Frame::Get();

SDL_FRect groundRect = {
    0.0f,
    600.0f,
    (float)window.getWidth(),
    50.0f,
};

static b2DebugDraw debugDraw;
static b2WorldDef worldDef;
static b2WorldId worldId;
static b2BodyId groundId;

struct Entity {
    SDL_FRect rect;
    b2BodyId bodyId;
    b2ShapeId shapeId;
};

std::vector<Entity> boxes;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    if (!window.create()) {
        return SDL_APP_FAILURE;
    }

    frame.init();

    worldDef         = b2DefaultWorldDef();
    worldDef.gravity = b2Vec2{0.0f, 9.8f};
    worldId          = b2CreateWorld(&worldDef);

    b2BodyDef groundBodyDef   = b2DefaultBodyDef();
    groundBodyDef.position    = b2Vec2{groundRect.x + (groundRect.w / 2),
                                       groundRect.y + (groundRect.h / 2)};
    groundId                  = b2CreateBody(worldId, &groundBodyDef);
    b2Polygon groundBox       = b2MakeBox(groundRect.w / 2, groundRect.h / 2);
    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    groundShapeDef.material.customColor = 0x00FF0000;
    b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

    debugDraw                  = b2DefaultDebugDraw();
    debugDraw.context          = window.getSDLRenderer();
    debugDraw.drawBounds       = true;
    debugDraw.drawShapes       = true;
    debugDraw.useDrawingBounds = true;
    debugDraw.DrawPolygonFcn   = [](const b2Vec2* vertices, int vertexCount,
                                    b2HexColor color, void* context) {
        SDL_Renderer* renderer = (SDL_Renderer*)context;
        SDL_SetRenderDrawColor(renderer, (color >> 24) & 0xFF,
                               (color >> 16) & 0xFF, (color >> 8) & 0xFF,
                               color & 0xFF);

        std::vector<SDL_FPoint> sdlVertices;
        for (int i = 0; i < vertexCount; ++i) {
            sdlVertices.push_back(SDL_FPoint{vertices[i].x, vertices[i].y});
        }
        sdlVertices.push_back(sdlVertices[0]);

        SDL_RenderLines(renderer, sdlVertices.data(), sdlVertices.size());
    };

    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS; /* end the program, reporting success to the OS.
                                 */
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        switch (event->button.button) {
            case SDL_BUTTON_LEFT: {
                SDL_FRect entityRect = {
                    event->button.x - 15.0f,
                    event->button.y - 15.0f,
                    30.0f,
                    30.0f,
                };

                b2BodyDef entityDef    = b2DefaultBodyDef();
                entityDef.gravityScale = 9.8f;
                entityDef.type         = b2_dynamicBody;
                entityDef.position     = b2Vec2{entityRect.x, entityRect.y};

                b2BodyId entityId = b2CreateBody(worldId, &entityDef);

                b2Polygon entityDynamicBox =
                    b2MakeBox(entityRect.w / 2, entityRect.h / 2);

                b2ShapeDef entityShapeDef           = b2DefaultShapeDef();
                entityShapeDef.density              = 100.0f;
                entityShapeDef.material.friction    = 1.0f;
                entityShapeDef.material.restitution = 0.3f;
                entityShapeDef.material.customColor = 0xFF0F0F00;

                b2ShapeId entityShapeId = b2CreatePolygonShape(
                    entityId, &entityShapeDef, &entityDynamicBox);

                Entity newEntity = {entityRect, entityId, entityShapeId};
                boxes.push_back(newEntity);

                break;
            }
            default:
                break;
        }
    }

    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate) {
    frame.calculateDeltaTime();
    const auto simulate = [&]() {
        b2World_Step(worldId, frame.getDeltaTime(), 8);
    };
    frame.accumulateTime(simulate);

    SDL_SetRenderDrawColor(window.getSDLRenderer(), 255, 255, 255,
                           SDL_ALPHA_OPAQUE);
    SDL_RenderClear(window.getSDLRenderer());

    SDL_SetRenderDrawColor(window.getSDLRenderer(), 40, 40, 40,
                           SDL_ALPHA_OPAQUE / 2);
    SDL_RenderFillRect(window.getSDLRenderer(), &groundRect);

    for (Entity& box : boxes) {
        b2Vec2 entityPosition = b2Body_GetPosition(box.bodyId);
        b2Rot entityRotation  = b2Body_GetRotation(box.bodyId);
        float entityAngle     = b2Rot_GetAngle(entityRotation);
        box.rect.x            = entityPosition.x;
        box.rect.y            = entityPosition.y;
        // SDL_SetRenderDrawColor(window.getSDLRenderer(), 20, 80, 255, 1);
        // SDL_RenderFillRect(window.getSDLRenderer(), &box.rect);

        b2Polygon boxPolygon = b2Shape_GetPolygon(box.shapeId);

        for (int i = 0; i < boxPolygon.count; ++i) {
            // Rotate the vertex
            float rotatedX = boxPolygon.vertices[i].x * entityRotation.c -
                             boxPolygon.vertices[i].y * entityRotation.s;
            float rotatedY = boxPolygon.vertices[i].x * entityRotation.s +
                             boxPolygon.vertices[i].y * entityRotation.c;

            // Translate to world position
            boxPolygon.vertices[i].x = rotatedX + entityPosition.x;
            boxPolygon.vertices[i].y = rotatedY + entityPosition.y;
        }

        debugDraw.DrawPolygonFcn(boxPolygon.vertices, boxPolygon.count,
                                 b2HexColor::b2_colorDarkRed,
                                 debugDraw.context);
    }

    b2World_Draw(worldId, &debugDraw);

    SDL_RenderPresent(window.getSDLRenderer());

    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    /* SDL will clean up the window/renderer for us. */
    b2DestroyWorld(worldId);
    worldId = b2_nullWorldId;
}
