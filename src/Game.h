#ifndef GAME_H
#define GAME_H

#include "Object.h"
#include "Scene.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <map>
#include <string>

class Game {
public:
    static Game& getInstance();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    ~Game();

    void run();

    // 生命周期函数
    void init();
    void handleEvent(SDL_Event* event);
    void update(float deltaTime);
    void render();
    void clean();

    void changeScene(Scene* scene);

    // 渲染工具函数
    SDL_Point renderTextCentered(std::string text, float posY, bool isTitle);
    void renderTextPos(std::string text, int posX, int posY);

    // getters
    SDL_Window* getWindow() { return window; }
    SDL_Renderer* getRenderer() { return renderer; }
    int getWindowWidth() { return windowWidth; }
    int getWindowHeight() { return windowHeight; }
    TTF_Font* getTitleFont() { return titleFont; }
    TTF_Font* getTextFont() { return textFont; }
    int getFinalScore() { return finalScore; }

    // setters
    void setFinalScore(int score) { finalScore = score; }

    // others
private:
    void backgroundUpdate(float deltaTime);
    void renderBackground();
    void insertLeaderBoard(int score, std::string name);

private:
    Game();
    bool isRunning = true;
    Scene* currentScene = nullptr;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    int windowWidth = 600;
    int windowHeight = 800;
    int fps = 60;
    Uint32 frameTime;
    float deltaTime;
    Background nearStars {};
    Background farStars {};
    TTF_Font* titleFont;
    TTF_Font* textFont;
    int finalScore = 0;
    std::multimap<int, std::string, std::greater<int>> leaderBoard;
};

#endif