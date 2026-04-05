#include "LevelC.h"

extern int gLives;

LevelC::LevelC() : Scene{{0.0f, 0.0f}, "#1A3A5C"} {}
LevelC::LevelC(Vector2 origin, const char *bgHexCode) : Scene{origin, bgHexCode} {}
LevelC::~LevelC() { shutdown(); }

void LevelC::initialise()
{
    mGameState.nextSceneID = 0;

    mBackgroundTexture = LoadTexture("assets/levels/bg3.png");

    mGameState.bgm = LoadMusicStream("assets/sound/bg_music.wav");
    SetMusicVolume(mGameState.bgm, 0.4f);
    PlayMusicStream(mGameState.bgm);

    mGameState.jumpSound  = LoadSound("assets/sound/jumpland.wav");
    mGameState.hitSound   = LoadSound("assets/sound/hit01.mp3.wav");
    mGameState.deathSound = LoadSound("assets/sound/die1.wav");

    mGameState.map = new Map(
        LEVEL_C_WIDTH, LEVEL_C_HEIGHT,
        (unsigned int *) mLevelData,
        "assets/tiles/tile_set.png",
        TILE_DIMENSION, 3, 1, mOrigin
    );

    std::map<Direction, std::vector<int>> playerAtlas = {
        {LEFT,  {0, 1, 2, 3, 4, 5}},
        {RIGHT, {0, 1, 2, 3, 4, 5}},
    };

    mGameState.player = new Entity(
        {mOrigin.x - (LEVEL_C_WIDTH * TILE_DIMENSION) / 2.0f + TILE_DIMENSION * 2,
         mOrigin.y - TILE_DIMENSION * 2},
        {60.0f, 60.0f},
        "assets/characters/krabby_run_sprite_sheet.png",
        ATLAS, {6, 1}, playerAtlas, PLAYER
    );
    mGameState.player->setJumpingPower(600.0f);
    mGameState.player->setColliderDimensions({40.0f, 50.0f});
    mGameState.player->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    mGameState.player->setSpeed(200);

    mGameState.numEnemies = 4;
    mGameState.enemies = new Entity[mGameState.numEnemies];

    float mapLeft = mOrigin.x - (LEVEL_C_WIDTH * TILE_DIMENSION) / 2.0f;

    std::map<Direction, std::vector<int>> wandererAtlas = {
        {LEFT,  {0, 1, 2, 3, 4, 5}},
        {RIGHT, {0, 1, 2, 3, 4, 5}},
    };

    std::map<Direction, std::vector<int>> followerAtlas = {
        {LEFT,  {0, 1, 2, 3, 4, 5}},
        {RIGHT, {0, 1, 2, 3, 4, 5}},
    };

    std::map<Direction, std::vector<int>> lerperAtlas = {
        {LEFT,  {0, 1, 2, 3, 4, 5}},
        {RIGHT, {0, 1, 2, 3, 4, 5}},
    };

    // WANDERER (enemy2)
    mGameState.enemies[0].setTexture("assets/characters/enemy2_run_sprite_sheet.png");
    mGameState.enemies[0].setTextureType(ATLAS);
    mGameState.enemies[0].setSpriteSheetDimensions({6, 1});
    mGameState.enemies[0].setAnimationAtlas(wandererAtlas);
    mGameState.enemies[0].setPosition({mapLeft + TILE_DIMENSION * 6,
                                       mOrigin.y - TILE_DIMENSION * 2});
    mGameState.enemies[0].setScale({50.0f, 50.0f});
    mGameState.enemies[0].setColliderDimensions({35.0f, 40.0f});
    mGameState.enemies[0].setEntityType(NPC);
    mGameState.enemies[0].setAIType(WANDERER);
    mGameState.enemies[0].setAIState(WALKING);
    mGameState.enemies[0].setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    mGameState.enemies[0].setSpeed(100);
    mGameState.enemies[0].setDirection(LEFT);

    // FOLLOWER x2 (enemy3)
    for (int i = 1; i <= 2; i++)
    {
        mGameState.enemies[i].setTexture("assets/characters/enemy3_run_sprite_sheet.png");
        mGameState.enemies[i].setTextureType(ATLAS);
        mGameState.enemies[i].setSpriteSheetDimensions({6, 1});
        mGameState.enemies[i].setAnimationAtlas(followerAtlas);
        mGameState.enemies[i].setPosition({mapLeft + TILE_DIMENSION * (12 + i * 8),
                                           mOrigin.y - TILE_DIMENSION * 2});
        mGameState.enemies[i].setScale({55.0f, 55.0f});
        mGameState.enemies[i].setColliderDimensions({38.0f, 45.0f});
        mGameState.enemies[i].setEntityType(NPC);
        mGameState.enemies[i].setAIType(FOLLOWER);
        mGameState.enemies[i].setAIState(IDLE);
        mGameState.enemies[i].setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
        mGameState.enemies[i].setSpeed(120);
        mGameState.enemies[i].setDirection(LEFT);
    }

    // LERPER (enemy1)
    mGameState.enemies[3].setTexture("assets/characters/enemy1_run_sprite_sheet.png");
    mGameState.enemies[3].setTextureType(ATLAS);
    mGameState.enemies[3].setSpriteSheetDimensions({6, 1});
    mGameState.enemies[3].setAnimationAtlas(lerperAtlas);
    mGameState.enemies[3].setPosition({mapLeft + TILE_DIMENSION * 25,
                                       mOrigin.y - TILE_DIMENSION * 3});
    mGameState.enemies[3].setScale({55.0f, 55.0f});
    mGameState.enemies[3].setColliderDimensions({38.0f, 45.0f});
    mGameState.enemies[3].setEntityType(NPC);
    mGameState.enemies[3].setAIType(LERPER);
    mGameState.enemies[3].setAIState(IDLE);
    mGameState.enemies[3].setLerpFactor(2.0f);
    mGameState.enemies[3].setDirection(LEFT);
}

void LevelC::update(float deltaTime)
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
            mGameState.nextSceneID = (gLives <= 0) ? 5 : 3;
            return;
        }
    }

    if (mGameState.player->getPosition().y > END_GAME_THRESHOLD)
    {
        PlaySound(mGameState.deathSound);
        gLives--;
        mGameState.nextSceneID = (gLives <= 0) ? 5 : 3;
        return;
    }

    float mapRight = mOrigin.x + (LEVEL_C_WIDTH * TILE_DIMENSION) / 2.0f - TILE_DIMENSION * 3;
    if (mGameState.player->getPosition().x >= mapRight)
        mGameState.nextSceneID = 4;
}

void LevelC::render()
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

void LevelC::shutdown()
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