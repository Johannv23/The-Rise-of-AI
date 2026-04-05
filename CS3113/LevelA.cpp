#include "LevelA.h"

extern int gLives;

LevelA::LevelA() : Scene{{0.0f, 0.0f}, "#87CEEB"} {}
LevelA::LevelA(Vector2 origin, const char *bgHexCode) : Scene{origin, bgHexCode} {}
LevelA::~LevelA() { shutdown(); }

void LevelA::initialise()
{
    mGameState.nextSceneID = 0;

    mBackgroundTexture = LoadTexture("assets/levels/bg1.png");

    mGameState.bgm = LoadMusicStream("assets/sound/bg_music.wav");
    SetMusicVolume(mGameState.bgm, 0.4f);
    PlayMusicStream(mGameState.bgm);

    mGameState.jumpSound  = LoadSound("assets/sound/jumpland.wav");
    mGameState.hitSound   = LoadSound("assets/sound/hit01.mp3.wav");
    mGameState.deathSound = LoadSound("assets/sound/die1.wav");

    mGameState.map = new Map(
        LEVEL_A_WIDTH, LEVEL_A_HEIGHT,
        (unsigned int *) mLevelData,
        "assets/tiles/tile_set.png",
        TILE_DIMENSION, 3, 1, mOrigin
    );

    std::map<Direction, std::vector<int>> playerAtlas = {
        {LEFT,  {0, 1, 2, 3, 4, 5}},
        {RIGHT, {0, 1, 2, 3, 4, 5}},
    };

    mGameState.player = new Entity(
        {mOrigin.x - (LEVEL_A_WIDTH * TILE_DIMENSION) / 2.0f + TILE_DIMENSION * 2,
         mOrigin.y - TILE_DIMENSION * 2},
        {60.0f, 60.0f},
        "assets/characters/krabby_run_sprite_sheet.png",
        ATLAS, {6, 1}, playerAtlas, PLAYER
    );
    mGameState.player->setJumpingPower(600.0f);
    mGameState.player->setColliderDimensions({40.0f, 50.0f});
    mGameState.player->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    mGameState.player->setSpeed(200);

    mGameState.numEnemies = 2;
    mGameState.enemies = new Entity[mGameState.numEnemies];

    std::map<Direction, std::vector<int>> wandererAtlas = {
        {LEFT,  {0, 1, 2, 3, 4, 5}},
        {RIGHT, {0, 1, 2, 3, 4, 5}},
    };

    float mapLeft = mOrigin.x - (LEVEL_A_WIDTH * TILE_DIMENSION) / 2.0f;

    for (int i = 0; i < mGameState.numEnemies; i++)
    {
        mGameState.enemies[i].setTexture("assets/characters/enemy2_run_sprite_sheet.png");
        mGameState.enemies[i].setTextureType(ATLAS);
        mGameState.enemies[i].setSpriteSheetDimensions({6, 1});
        mGameState.enemies[i].setAnimationAtlas(wandererAtlas);
        mGameState.enemies[i].setPosition({mapLeft + TILE_DIMENSION * (8 + i * 8),
                                           mOrigin.y - TILE_DIMENSION * 2});
        mGameState.enemies[i].setScale({50.0f, 50.0f});
        mGameState.enemies[i].setColliderDimensions({35.0f, 40.0f});
        mGameState.enemies[i].setEntityType(NPC);
        mGameState.enemies[i].setAIType(WANDERER);
        mGameState.enemies[i].setAIState(WALKING);
        mGameState.enemies[i].setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
        mGameState.enemies[i].setSpeed(80);
        mGameState.enemies[i].setDirection(LEFT);
    }
}

void LevelA::update(float deltaTime)
{
    UpdateMusicStream(mGameState.bgm);

    mGameState.player->update(deltaTime, nullptr, mGameState.map,
        mGameState.enemies, mGameState.numEnemies);

    for (int i = 0; i < mGameState.numEnemies; i++)
        mGameState.enemies[i].update(deltaTime, mGameState.player,
            mGameState.map, nullptr, 0);

    for (int i = 0; i < mGameState.numEnemies; i++)
    {
        if (mGameState.enemies[i].isActive() &&
            mGameState.player->isCollidingWith(&mGameState.enemies[i]))
        {
            PlaySound(mGameState.hitSound);
            gLives--;
            mGameState.nextSceneID = (gLives <= 0) ? 5 : 1;
            return;
        }
    }

    if (mGameState.player->getPosition().y > END_GAME_THRESHOLD)
    {
        PlaySound(mGameState.deathSound);
        gLives--;
        mGameState.nextSceneID = (gLives <= 0) ? 5 : 1;
        return;
    }

    float mapRight = mOrigin.x + (LEVEL_A_WIDTH * TILE_DIMENSION) / 2.0f - TILE_DIMENSION * 3;
    if (mGameState.player->getPosition().x >= mapRight)
        mGameState.nextSceneID = 2;
}

void LevelA::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    DrawTexturePro(mBackgroundTexture,
        {0, 0, (float)mBackgroundTexture.width, (float)mBackgroundTexture.height},
        {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        {0, 0}, 0.0f, WHITE);
    mGameState.map->render();
    for (int i = 0; i < mGameState.numEnemies; i++) mGameState.enemies[i].render();
    mGameState.player->render();
}

void LevelA::shutdown()
{
    if (mGameState.player)  { delete mGameState.player;   mGameState.player  = nullptr; }
    if (mGameState.enemies) { delete[] mGameState.enemies; mGameState.enemies = nullptr; }
    if (mGameState.map)     { delete mGameState.map;      mGameState.map     = nullptr; }
    if (mBackgroundTexture.id != 0) UnloadTexture(mBackgroundTexture);
    if (mGameState.bgm.frameCount > 0)        UnloadMusicStream(mGameState.bgm);
    if (mGameState.jumpSound.frameCount > 0)  UnloadSound(mGameState.jumpSound);
    if (mGameState.hitSound.frameCount > 0)   UnloadSound(mGameState.hitSound);
    if (mGameState.deathSound.frameCount > 0) UnloadSound(mGameState.deathSound);
}