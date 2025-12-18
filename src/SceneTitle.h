#ifndef SCENETITLE_H
#define SCENETTILE_H

#include "Game.h"
#include "Scene.h"
#include <SDL_mixer.h>
#include <SDL_ttf.h>

class SceneTitle : public Scene {
public:
    SceneTitle();
    ~SceneTitle();

    // 生命周期函数
    void init() override;
    void handleEvent(SDL_Event* event) override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;

private:
    Mix_Music* bgm;
    float timer = 0.0f;
};

#endif