#ifndef SCENEMAIN_H
#define SCENEMAIN_H

#include "Scene.h"
#include "Object.h"
#include "Game.h"

class SceneMain : public Scene
{
public:
    SceneMain();
    ~SceneMain();

    void init() override;
    void handleEvent(SDL_Event *event) override;
    void update() override;
    void render() override;
    void clean() override;

    void keyboardControl();

private:
    Game& game;
    Player player;
};

#endif