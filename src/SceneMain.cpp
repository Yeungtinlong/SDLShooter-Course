#include "SceneMain.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Game.h"

SceneMain::SceneMain() : game(Game::getInstance())
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::init()
{
    player.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/SpaceShip.png");
    if (player.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "IMG_LoadTexture Error: %s\n", SDL_GetError());
        return;
    }
    if (SDL_QueryTexture(player.texture, nullptr, nullptr, &player.width, &player.height) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_QueryTexture Error: %s\n", SDL_GetError());
        return;
    }
    player.width /= 4;
    player.height /= 4;
    player.position.x = game.getWindowWidth() / 2 - player.width / 2;
    player.position.y = game.getWindowHeight() - player.height;
}

void SceneMain::handleEvent(SDL_Event *event)
{
}

void SceneMain::update()
{
    keyboardControl();
}

void SceneMain::render()
{
    SDL_RenderClear(game.getRenderer());
    SDL_Rect rect{
        static_cast<int>(player.position.x),
        static_cast<int>(player.position.y),
        player.width,
        player.height};
    SDL_RenderCopy(game.getRenderer(), player.texture, nullptr, &rect);
}

void SceneMain::clean()
{
    if (player.texture != nullptr)
    {
        SDL_DestroyTexture(player.texture);
    }
}

void SceneMain::keyboardControl()
{
    const u_int8_t *keyboardState = SDL_GetKeyboardState(nullptr);
    SDL_FPoint vec{0, 0};
    float moveSpeed = 1;
    if (keyboardState[SDL_SCANCODE_W])
    {
        vec.y -= moveSpeed;
    }
    if (keyboardState[SDL_SCANCODE_S])
    {
        vec.y += moveSpeed;
    }
    if (keyboardState[SDL_SCANCODE_A])
    {
        vec.x -= moveSpeed;
    }
    if (keyboardState[SDL_SCANCODE_D])
    {
        vec.x += moveSpeed;
    }
    // TODO: 移动应该使用命令，而不是直接改变位置
    player.position.x += vec.x;
    player.position.y += vec.y;

    float xLimit = game.getWindowWidth() - player.width;
    float yLimit = game.getWindowHeight() - player.height;
    if (player.position.x < 0)
        player.position.x = 0;
    if (player.position.x > xLimit)
        player.position.x = xLimit;
    if (player.position.y < 0) 
        player.position.y = 0;
    if (player.position.y > yLimit)
        player.position.y = yLimit;
}
