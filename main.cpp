/**
* Author: Johann Varghese
* Assignment: Rise of the AI
* Date due: 2026-04-04, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/MenuScene.h"
#include "CS3113/WinScene.h"
#include "CS3113/LoseScene.h"
#include "CS3113/LevelA.h"
#include "CS3113/LevelB.h"
#include "CS3113/LevelC.h"
#include <vector>
#include "raymath.h"

// -------------------------------------------------------
// Constants
// -------------------------------------------------------

constexpr int   SCREEN_WIDTH  = 800;
constexpr int   SCREEN_HEIGHT = 600;
constexpr int   FPS           = 60;
constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

constexpr Vector2 ORIGIN = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};

// Scene IDs:
// 0 = MenuScene
// 1 = LevelA
// 2 = LevelB
// 3 = LevelC
// 4 = WinScene
// 5 = LoseScene

// -------------------------------------------------------
// Globals
// -------------------------------------------------------

AppStatus gAppStatus = RUNNING;

int   gLives           = 3;
float gPreviousTicks   = 0.0f;
float gTimeAccumulator = 0.0f;

Camera2D gCamera = {0};

Scene *gCurrentScene = nullptr;
std::vector<Scene*> gLevels;

MenuScene *gMenuScene = nullptr;
LevelA    *gLevelA    = nullptr;
LevelB    *gLevelB    = nullptr;
LevelC    *gLevelC    = nullptr;
WinScene  *gWinScene  = nullptr;
LoseScene *gLoseScene = nullptr;

// -------------------------------------------------------
// switchToScene
// -------------------------------------------------------

void switchToScene(Scene *scene)
{
    if (gCurrentScene != nullptr)
        gCurrentScene->shutdown();

    gCurrentScene = scene;
    gCurrentScene->initialise();

    // Only point camera at player if this scene has one
    if (gCurrentScene->getState().player != nullptr)
        gCamera.target = gCurrentScene->getState().player->getPosition();
    else
        gCamera.target = ORIGIN;
}

// -------------------------------------------------------
// initialise
// -------------------------------------------------------

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Krabby's Beach Quest");
    SetTargetFPS(FPS);
    InitAudioDevice();

    gMenuScene = new MenuScene(ORIGIN, "#000000");
    gLevelA    = new LevelA(ORIGIN,    "#87CEEB");
    gLevelB    = new LevelB(ORIGIN,    "#4A90D9");
    gLevelC    = new LevelC(ORIGIN,    "#1A3A5C");
    gWinScene  = new WinScene(ORIGIN,  "#000000");
    gLoseScene = new LoseScene(ORIGIN, "#000000");

    gLevels.push_back(gMenuScene);  // 0
    gLevels.push_back(gLevelA);     // 1
    gLevels.push_back(gLevelB);     // 2
    gLevels.push_back(gLevelC);     // 3
    gLevels.push_back(gWinScene);   // 4
    gLevels.push_back(gLoseScene);  // 5

    gCamera.offset   = ORIGIN;
    gCamera.rotation = 0.0f;
    gCamera.zoom     = 1.0f;

    switchToScene(gLevels[0]);

    gPreviousTicks = (float)GetTime();
}

// -------------------------------------------------------
// processInput
// -------------------------------------------------------

void processInput()
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;

    // Debug level switching
    if (IsKeyPressed(KEY_ONE))   { gLives = 3; switchToScene(gLevels[1]); }
    if (IsKeyPressed(KEY_TWO))   { gLives = 3; switchToScene(gLevels[2]); }
    if (IsKeyPressed(KEY_THREE)) { gLives = 3; switchToScene(gLevels[3]); }

    // Menu/Win/Lose: press ENTER
    if (IsKeyPressed(KEY_ENTER))
    {
        if (gCurrentScene == gMenuScene)
        {
            gLives = 3;
            switchToScene(gLevels[1]);
            return;
        }
        if (gCurrentScene == gWinScene || gCurrentScene == gLoseScene)
        {
            gLives = 3;
            switchToScene(gLevels[0]);
            return;
        }
    }

    // Player input — only when in a level
    Entity *player = gCurrentScene->getState().player;
    if (player == nullptr) return;

    player->resetMovement();

    if      (IsKeyDown(KEY_A)) player->moveLeft();
    else if (IsKeyDown(KEY_D)) player->moveRight();

    if (IsKeyPressed(KEY_W) && player->isCollidingBottom())
    {
        player->jump();
        PlaySound(gCurrentScene->getState().jumpSound);
    }

    if (GetLength(player->getMovement()) > 1.0f)
        player->normaliseMovement();
}

// -------------------------------------------------------
// update
// -------------------------------------------------------

void update()
{
    float ticks     = (float)GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;

        // Pan camera horizontally following player
        Entity *player = gCurrentScene->getState().player;
        if (player != nullptr)
        {
            Vector2 camTarget = {player->getPosition().x, ORIGIN.y};
            panCamera(&gCamera, &camTarget);
        }
    }

    gTimeAccumulator = deltaTime;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------

void render()
{
    BeginDrawing();

    BeginMode2D(gCamera);
        gCurrentScene->render();
    EndMode2D();

    // HUD — drawn in screen space, outside camera
    if (gCurrentScene != gMenuScene &&
        gCurrentScene != gWinScene  &&
        gCurrentScene != gLoseScene)
    {
        DrawText(TextFormat("LIVES: %d", gLives), 15, 15, 24, WHITE);
    }

    EndDrawing();
}

// -------------------------------------------------------
// shutdown
// -------------------------------------------------------

void shutdown()
{
    if (gCurrentScene != nullptr)
        gCurrentScene->shutdown();

    // Don't double-delete — just clear pointers
    for (Scene *s : gLevels) { delete s; }
    gLevels.clear();

    CloseAudioDevice();
    CloseWindow();
}

// -------------------------------------------------------
// main
// -------------------------------------------------------

int main()
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();

        // Scene switching
        int nextID = gCurrentScene->getState().nextSceneID;
        if (nextID > 0)
            switchToScene(gLevels[nextID]);

        render();
    }

    shutdown();
    return 0;
}
