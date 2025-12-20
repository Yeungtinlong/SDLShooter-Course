#include "SceneTitle.h"
#include "SceneMain.h"

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
    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.scancode == SDL_SCANCODE_J) {
            game.changeScene(new SceneMain());
        }
    }
}

void SceneTitle::update(float deltaTime)
{
    timer += deltaTime;
    if (timer > 1.0f)
        timer = 0.0f;
}

void SceneTitle::render()
{
    // 渲染标题文字
    game.renderTextCentered("太空战机", 0.4f, true);

    // 渲染按键 J 进入游戏
    if (timer < 0.5f)
        game.renderTextCentered("按 J 键进入游戏", 0.8f, false);
}

void SceneTitle::clean()
{
    if (bgm != nullptr) {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
    }
}
