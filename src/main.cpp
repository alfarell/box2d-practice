#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <box2d/box2d.h>

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
    .x = 0.0f,
    .y = 600.0f,
    .w = (float)WINDOW_WIDTH,
    .h = 50.0f,
};

SDL_FRect entityRect = {
    .x = 100.0f,
    .y = 100.0f,
    .w = 30.0f,
    .h = 30.0f,
};

static b2WorldDef worldDef;
static b2WorldId worldId;
b2BodyId groundId;
b2BodyId entityId;

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
    worldDef.gravity = (b2Vec2){.x = 0.0f, .y = 9.8f};
    worldId          = b2CreateWorld(&worldDef);

    b2BodyDef groundBodyDef   = b2DefaultBodyDef();
    groundBodyDef.position    = (b2Vec2){groundRect.x, groundRect.y};
    groundId                  = b2CreateBody(worldId, &groundBodyDef);
    b2Polygon groundBox       = b2MakeBox(groundRect.w, groundRect.h);
    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

    b2BodyDef entityDef              = b2DefaultBodyDef();
    entityDef.type                   = b2_dynamicBody;
    entityDef.position               = (b2Vec2){entityRect.x, entityRect.y};
    entityId                         = b2CreateBody(worldId, &entityDef);
    b2Polygon entityDynamicBox       = b2MakeBox(entityRect.w, entityRect.h);
    b2ShapeDef entityShapeDef        = b2DefaultShapeDef();
    entityShapeDef.density           = 1.0f;
    entityShapeDef.material.friction = 0.3f;
    b2CreatePolygonShape(entityId, &entityShapeDef, &entityDynamicBox);

    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS; /* end the program, reporting success to the OS.
                                 */
    }
    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 40, 40, 40, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &groundRect);

    b2Vec2 entityPosition = b2Body_GetPosition(entityId);
    b2Rot entityRotation  = b2Body_GetRotation(entityId);
    float entityAngle     = b2Rot_GetAngle(entityRotation);
    entityRect.x          = entityPosition.x;
    entityRect.y          = entityPosition.y;
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &entityRect);

    SDL_RenderPresent(renderer);

    lastTicks    = currentTicks;
    currentTicks = SDL_GetPerformanceCounter();
    deltaTime    = (currentTicks - lastTicks) / performanceFrequency;
    accumulatedTime += deltaTime;

    while (accumulatedTime >= targetFrameTime) {
        b2World_Step(worldId, targetFrameTime, 4);
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
