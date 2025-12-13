#include "Game.h"
#include "SceneMain.h"
#include <SDL2/SDL_image.h>

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
        SDL_Event event;
        handleEvent(&event);

        update();

        render();
    }
}

void Game::init()
{
    // SDL 初始化
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    window = SDL_CreateWindow("SDL_Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    // 初始化SDL_image
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "IMG_Init Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    currentScene = new SceneMain();
    currentScene->init();

    // SDL_Texture *texture = IMG_LoadTexture(renderer, "assets/image/bg.png");

    // // 初始化SDL_mixer
    // if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    // {
    //     std::cerr << "Mix_OpenAudio Error: " << SDL_GetError() << std::endl;
    //     return 1;
    // }

    // // 初始化SDL_ttf
    // if (TTF_Init() != 0)
    // {
    //     std::cerr << "TTF_Init Error: " << SDL_GetError() << std::endl;
    //     return 1;
    // }
}

void Game::clean()
{
    if (currentScene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
    }

    IMG_Quit();

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

void Game::update()
{
    // 逻辑更新
    currentScene->update();
}

void Game::render()
{
    // 渲染更新
    currentScene->render();
    SDL_RenderPresent(renderer);
}
