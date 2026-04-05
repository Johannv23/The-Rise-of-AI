#ifndef LEVELB_H
#define LEVELB_H

#include "Scene.h"

constexpr int LEVEL_B_WIDTH  = 35;
constexpr int LEVEL_B_HEIGHT = 8;

class LevelB : public Scene
{
private:
    unsigned int mLevelData[LEVEL_B_WIDTH * LEVEL_B_HEIGHT] = {
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
        2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
        2,0,0,0,3,3,0,0,0,0,0,3,3,0,0,0,0,0,3,3,0,0,0,0,0,3,3,0,0,0,0,0,0,0,2,
        2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
        2,0,0,0,0,0,0,0,3,3,0,0,0,0,0,3,3,0,0,0,0,0,3,3,0,0,0,0,0,0,0,0,0,0,2,
        2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    };

    Texture2D mBackgroundTexture;

public:
    static constexpr float TILE_DIMENSION          = 64.0f;
    static constexpr float ACCELERATION_OF_GRAVITY = 981.0f;
    static constexpr float END_GAME_THRESHOLD      = 900.0f;

    LevelB();
    LevelB(Vector2 origin, const char *bgHexCode);
    ~LevelB();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // LEVELB_H