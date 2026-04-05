#include "MenuScene.h"

MenuScene::MenuScene() : Scene{{0.0f, 0.0f}, "#000000"} {}
MenuScene::MenuScene(Vector2 origin, const char *bgHexCode) : Scene{origin, bgHexCode} {}
MenuScene::~MenuScene() { shutdown(); }

void MenuScene::initialise()
{
    mGameState.nextSceneID = 0;
    mGameState.player      = nullptr;
    mGameState.enemies     = nullptr;
    mGameState.map         = nullptr;
}

void MenuScene::update(float deltaTime) {}

void MenuScene::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    // Title
    const char *title    = "KRABBY'S BEACH QUEST";
    int         titleSize = 48;
    int         titleW    = MeasureText(title, titleSize);
    DrawText(title,
             (int)(mOrigin.x - titleW / 2),
             (int)(mOrigin.y - 80),
             titleSize, YELLOW);

    // Subtitle
    const char *sub  = "Press ENTER to Start";
    int         subW = MeasureText(sub, 24);
    DrawText(sub,
             (int)(mOrigin.x - subW / 2),
             (int)(mOrigin.y + 20),
             24, WHITE);

    // Controls hint
    const char *ctrl  = "A / D to move    W to jump";
    int         ctrlW = MeasureText(ctrl, 18);
    DrawText(ctrl,
             (int)(mOrigin.x - ctrlW / 2),
             (int)(mOrigin.y + 80),
             18, LIGHTGRAY);
}

void MenuScene::shutdown() {}
