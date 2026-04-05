#ifndef LEVELA_H
#define LEVELA_H

#include "Scene.h"

constexpr int LEVEL_A_WIDTH  = 30;
constexpr int LEVEL_A_HEIGHT = 8;

class LevelA : public Scene
{
private:
    unsigned int mLevelData[LEVEL_A_WIDTH * LEVEL_A_HEIGHT] = {
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
        2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
        2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
        2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
        2,0,0,0,0,3,3,3,0,0,0,0,0,3,3,3,0,0,0,0,0,3,3,3,0,0,0,0,0,2,
        2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    };

    Texture2D mBackgroundTexture;

public:
    static constexpr float TILE_DIMENSION          = 64.0f;
    static constexpr float ACCELERATION_OF_GRAVITY = 981.0f;
    static constexpr float END_GAME_THRESHOLD      = 900.0f;

    LevelA();
    LevelA(Vector2 origin, const char *bgHexCode);
    ~LevelA();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // LEVELA_H
