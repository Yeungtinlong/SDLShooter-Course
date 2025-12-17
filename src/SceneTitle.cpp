#include "SceneTitle.h"

SceneTitle::SceneTitle()
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::init()
{
    // 加载bgm
    bgm = Mix_LoadMUS("assets/music/06_Battle_in_Space_Intro.ogg");
    if (bgm == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Mix_LoadMUS Error: %s\n", SDL_GetError());
    }
    Mix_PlayMusic(bgm, -1);
}

void SceneTitle::handleEvent(SDL_Event* event)
{
}

void SceneTitle::update(float deltaTime)
{
}

void SceneTitle::render()
{
    // 渲染标题文字
    game.renderTextCentered("太空战机", 0.4f, true);
    // 渲染
    game.renderTextCentered("按 J 键进入游戏", 0.8f, false);
}

void SceneTitle::clean()
{
}
