#include "LevelB.h"

extern int gLives;

LevelB::LevelB() : Scene{{0.0f, 0.0f}, "#4A90D9"} {}
LevelB::LevelB(Vector2 origin, const char *bgHexCode) : Scene{origin, bgHexCode} {}
LevelB::~LevelB() { shutdown(); }

void LevelB::initialise()
{
    mGameState.nextSceneID = 0;

    mBackgroundTexture = LoadTexture("assets/levels/bg2.png");

    mGameState.bgm = LoadMusicStream("assets/sound/bg_music.wav");
    SetMusicVolume(mGameState.bgm, 0.4f);
    PlayMusicStream(mGameState.bgm);

    mGameState.jumpSound  = LoadSound("assets/sound/jumpland.wav");
    mGameState.hitSound   = LoadSound("assets/sound/hit01.mp3.wav");
    mGameState.deathSound = LoadSound("assets/sound/die1.wav");

    mGameState.map = new Map(
        LEVEL_B_WIDTH, LEVEL_B_HEIGHT,
        (unsigned int *) mLevelData,
        "assets/tiles/tile_set.png",
        TILE_DIMENSION, 3, 1, mOrigin
    );

    std::map<Direction, std::vector<int>> playerAtlas = {
        {LEFT,  {0, 1, 2, 3, 4, 5}},
        {RIGHT, {0, 1, 2, 3, 4, 5}},
    };

    mGameState.player = new Entity(
        {mOrigin.x - (LEVEL_B_WIDTH * TILE_DIMENSION) / 2.0f + TILE_DIMENSION * 2,
         mOrigin.y - TILE_DIMENSION * 2},
        {60.0f, 60.0f},
        "assets/characters/krabby_run_sprite_sheet.png",
        ATLAS, {6, 1}, playerAtlas, PLAYER
    );
    mGameState.player->setJumpingPower(600.0f);
    mGameState.player->setColliderDimensions({40.0f, 50.0f});
    mGameState.player->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    mGameState.player->setSpeed(200);

    mGameState.numEnemies = 3;
    mGameState.enemies = new Entity[mGameState.numEnemies];

    float mapLeft = mOrigin.x - (LEVEL_B_WIDTH * TILE_DIMENSION) / 2.0f;

    std::map<Direction, std::vector<int>> followerAtlas = {
        {LEFT,  {0, 1, 2, 3, 4, 5}},
        {RIGHT, {0, 1, 2, 3, 4, 5}},
    };

    std::map<Direction, std::vector<int>> wandererAtlas = {
        {LEFT,  {0, 1, 2, 3, 4, 5}},
        {RIGHT, {0, 1, 2, 3, 4, 5}},
    };

    // Two followers (enemy3 = pirate)
    for (int i = 0; i < 2; i++)
    {
        mGameState.enemies[i].setTexture("assets/characters/enemy3_run_sprite_sheet.png");
        mGameState.enemies[i].setTextureType(ATLAS);
        mGameState.enemies[i].setSpriteSheetDimensions({6, 1});
        mGameState.enemies[i].setAnimationAtlas(followerAtlas);
        mGameState.enemies[i].setPosition({mapLeft + TILE_DIMENSION * (10 + i * 10),
                                           mOrigin.y - TILE_DIMENSION * 2});
        mGameState.enemies[i].setScale({55.0f, 55.0f});
        mGameState.enemies[i].setColliderDimensions({38.0f, 45.0f});
        mGameState.enemies[i].setEntityType(NPC);
        mGameState.enemies[i].setAIType(FOLLOWER);
        mGameState.enemies[i].setAIState(IDLE);
        mGameState.enemies[i].setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
        mGameState.enemies[i].setSpeed(100);
        mGameState.enemies[i].setDirection(LEFT);
    }

    // One wanderer (enemy2 = purple blob)
    mGameState.enemies[2].setTexture("assets/characters/enemy2_run_sprite_sheet.png");
    mGameState.enemies[2].setTextureType(ATLAS);
    mGameState.enemies[2].setSpriteSheetDimensions({6, 1});
    mGameState.enemies[2].setAnimationAtlas(wandererAtlas);
    mGameState.enemies[2].setPosition({mapLeft + TILE_DIMENSION * 20,
                                       mOrigin.y - TILE_DIMENSION * 2});
    mGameState.enemies[2].setScale({50.0f, 50.0f});
    mGameState.enemies[2].setColliderDimensions({35.0f, 40.0f});
    mGameState.enemies[2].setEntityType(NPC);
    mGameState.enemies[2].setAIType(WANDERER);
    mGameState.enemies[2].setAIState(WALKING);
    mGameState.enemies[2].setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    mGameState.enemies[2].setSpeed(80);
    mGameState.enemies[2].setDirection(LEFT);
}

void LevelB::update(float deltaTime)
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
            mGameState.nextSceneID = (gLives <= 0) ? 5 : 2;
            return;
        }
    }

    if (mGameState.player->getPosition().y > END_GAME_THRESHOLD)
    {
        PlaySound(mGameState.deathSound);
        gLives--;
        mGameState.nextSceneID = (gLives <= 0) ? 5 : 2;
        return;
    }

    float mapRight = mOrigin.x + (LEVEL_B_WIDTH * TILE_DIMENSION) / 2.0f - TILE_DIMENSION * 3;
    if (mGameState.player->getPosition().x >= mapRight)
        mGameState.nextSceneID = 3;
}

void LevelB::render()
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

void LevelB::shutdown()
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