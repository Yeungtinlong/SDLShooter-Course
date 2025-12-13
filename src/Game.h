#ifndef GAME_H
#define GAME_H

#include "Scene.h"
#include <SDL2/SDL.h>

class Game
{
public:
    static Game &getInstance();
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;
    ~Game();
    void run();
    void init();
    void clean();
    void changeScene(Scene *scene);

    void handleEvent(SDL_Event *event);
    void update();
    void render();

    SDL_Window *getWindow() { return window; }
    SDL_Renderer *getRenderer() { return renderer; }
    int getWindowWidth() { return windowWidth; }
    int getWindowHeight() { return windowHeight; }

private:
    Game();
    bool isRunning = true;
    Scene *currentScene = nullptr;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    int windowWidth = 600;
    int windowHeight = 800;
};

#endif