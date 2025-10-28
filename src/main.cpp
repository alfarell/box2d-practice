#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <box2d/box2d.h>

#include <vector>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

/* We will use this renderer to draw into this window every frame. */
static SDL_Window* window     = NULL;
static SDL_Renderer* renderer = NULL;

const int targetFPS         = 120;
const float targetFrameTime = (float)(1.0 / targetFPS);
Uint64 currentTicks;
Uint64 lastTicks;
float deltaTime;
float performanceFrequency;
float accumulatedTime;

SDL_FRect groundRect = {
    0.0f,
    600.0f,
    (float)WINDOW_WIDTH,
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
    SDL_SetAppMetadata("Box2D", "1.0", "com.example.box2d");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", WINDOW_WIDTH,
                                     WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE,
                                     &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);

    currentTicks         = SDL_GetPerformanceCounter();
    lastTicks            = 0;
    deltaTime            = 0.0;
    performanceFrequency = (float)(SDL_GetPerformanceFrequency());
    accumulatedTime      = 0.0;

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
    debugDraw.context          = renderer;
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
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 40, 40, 40, SDL_ALPHA_OPAQUE / 2);
    SDL_RenderFillRect(renderer, &groundRect);

    for (Entity& box : boxes) {
        b2Vec2 entityPosition = b2Body_GetPosition(box.bodyId);
        b2Rot entityRotation  = b2Body_GetRotation(box.bodyId);
        float entityAngle     = b2Rot_GetAngle(entityRotation);
        box.rect.x            = entityPosition.x;
        box.rect.y            = entityPosition.y;
        // SDL_SetRenderDrawColor(renderer, 20, 80, 255, 1);
        // SDL_RenderFillRect(renderer, &box.rect);

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

    SDL_RenderPresent(renderer);

    lastTicks    = currentTicks;
    currentTicks = SDL_GetPerformanceCounter();
    deltaTime    = (currentTicks - lastTicks) / performanceFrequency;
    accumulatedTime += deltaTime;

    while (accumulatedTime >= targetFrameTime) {
        b2World_Step(worldId, targetFrameTime, 8);
        accumulatedTime -= targetFrameTime;
    }

    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    /* SDL will clean up the window/renderer for us. */
    b2DestroyWorld(worldId);
    worldId = b2_nullWorldId;
}
