#include "Game.h"
#include "SceneMain.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

Game::Game()
{
}

Game &Game::getInstance()
{
    static Game instance;
    return instance;
}

Game::~Game()
{
    clean();
}

void Game::run()
{
    while (isRunning)
    {
        auto frameStart = SDL_GetTicks();
        SDL_Event event;
        handleEvent(&event);

        update(deltaTime);

        render();

        auto frameEnd = SDL_GetTicks();
        auto diff = frameEnd - frameStart;
        if (diff < frameTime)
        {
            SDL_Delay(frameTime - diff);
            deltaTime = frameTime / 1000.0f;
        }
        else
        {
            deltaTime = diff / 1000.0f;
        }
    }
}

void Game::init()
{
    frameTime = 1000 / fps;

    // SDL初始化
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    // SDL窗口初始化
    window = SDL_CreateWindow("SDL_Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    // SDL渲染器初始化
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    // SDL_image初始化
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "IMG_Init Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    // SDL_mixer初始化
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != (MIX_INIT_MP3 | MIX_INIT_OGG))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Mix_Init Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Mix_OpenAudio Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    // 设置音效Channel数量
    Mix_AllocateChannels(32);
    // 设置音乐音量
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    Mix_Volume(-1, MIX_MAX_VOLUME / 8);

    // 初始化背景
    nearStars.texture = IMG_LoadTexture(getRenderer(), "assets/image/Stars-A.png");
    if (nearStars.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "IMG_LoadTexture Error: %s\n", SDL_GetError());
    }
    if (SDL_QueryTexture(nearStars.texture, nullptr, nullptr, &nearStars.width, &nearStars.height) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_QueryTexture Error: %s\n", SDL_GetError());
    }
    nearStars.width /= 2;
    nearStars.height /= 2;

    farStars.texture = IMG_LoadTexture(getRenderer(), "assets/image/Stars-B.png");
    if (farStars.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "IMG_LoadTexture Error: %s\n", SDL_GetError());
    }
    if (SDL_QueryTexture(farStars.texture, nullptr, nullptr, &farStars.width, &farStars.height) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_QueryTexture Error: %s\n", SDL_GetError());
    }
    farStars.speed = 20.0f;
    farStars.width /= 2;
    farStars.height /= 2;

    currentScene = new SceneMain();
    currentScene->init();
}

void Game::clean()
{
    if (currentScene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
    }
    if (nearStars.texture != nullptr)
    {
        SDL_DestroyTexture(nearStars.texture);
    }
    if (farStars.texture != nullptr)
    {
        SDL_DestroyTexture(farStars.texture);
    }

    IMG_Quit();

    // 清理SDL_mixer
    Mix_CloseAudio();
    Mix_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::changeScene(Scene *scene)
{
    if (currentScene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
    }
    currentScene = scene;
    currentScene->init();
}

void Game::handleEvent(SDL_Event *event)
{
    // 事件处理
    while (SDL_PollEvent(event))
    {
        if (event->type == SDL_QUIT)
        {
            isRunning = false;
        }
        currentScene->handleEvent(event);
    }
}

void Game::update(float deltaTime)
{
    // 背景更新
    backgroundUpdate(deltaTime);

    // 场景更新
    currentScene->update(deltaTime);
}

void Game::render()
{
    // 清理渲染器
    SDL_RenderClear(getRenderer());

    // 渲染背景
    renderBackground();

    // 渲染场景
    currentScene->render();
    SDL_RenderPresent(renderer);
}

void Game::backgroundUpdate(float deltaTime)
{
    nearStars.offset += nearStars.speed * deltaTime;
    if (nearStars.offset >= 0)
    {
        nearStars.offset -= nearStars.height;
    }

    farStars.offset += farStars.speed * deltaTime;
    if (farStars.offset >= 0)
    {
        farStars.offset -= farStars.height;
    }
}

void Game::renderBackground()
{
    for (int yOffset = static_cast<int>(farStars.offset); yOffset < getWindowHeight(); yOffset += farStars.height)
    {
        for (int xOffset = 0; xOffset < getWindowWidth(); xOffset += farStars.width)
        {
            SDL_Rect dstRect = {xOffset, yOffset, farStars.width, farStars.height};
            if (SDL_RenderCopy(getRenderer(), farStars.texture, nullptr, &dstRect) != 0)
            {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_RenderCopy Error: %s\n", SDL_GetError());
            }
        }
    }
    for (int yOffset = static_cast<int>(nearStars.offset); yOffset < getWindowHeight(); yOffset += nearStars.height)
    {
        for (int xOffset = 0; xOffset < getWindowWidth(); xOffset += nearStars.width)
        {
            SDL_Rect dstRect = {xOffset, yOffset, nearStars.width, nearStars.height};
            if (SDL_RenderCopy(getRenderer(), nearStars.texture, nullptr, &dstRect) != 0)
            {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_RenderCopy Error: %s\n", SDL_GetError());
            }
        }
    }
}
