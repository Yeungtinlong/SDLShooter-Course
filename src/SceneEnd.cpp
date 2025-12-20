#include "SceneEnd.h"
#include "Game.h"
#include "SceneMain.h"
#include <string>

void SceneEnd::init()
{
    bgm = Mix_LoadMUS("assets/music/06_Battle_in_Space_Intro.ogg");
    Mix_PlayMusic(bgm, -1);

    if (!SDL_IsTextInputActive())
        SDL_StartTextInput();
    if (!SDL_IsTextInputActive()) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_StartTextInput Error: %s\n", SDL_GetError());
    }
}

void SceneEnd::handleEvent(SDL_Event* event)
{
    if (isTyping) {
        if (event->type == SDL_TEXTINPUT) {
            name += event->text.text;
        }
        if (event->type == SDL_KEYDOWN) {
            if (event->key.keysym.scancode == SDL_SCANCODE_RETURN) {
                isTyping = false;
                SDL_StopTextInput();
                if (name.empty()) {
                    name = "无名氏";
                }
                game.insertLeaderBoard(game.getFinalScore(), name);
            }
            if (event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
                removeLastUTF8Char(name);
            }
        }
    } else if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.scancode == SDL_SCANCODE_J) {
            game.changeScene(new SceneMain());
        }
    }
}

void SceneEnd::update(float deltaTime)
{
    updateUnderlineTimer(deltaTime);
}

void SceneEnd::render()
{
    if (isTyping) {
        renderPhase1();
    } else {
        renderPhase2();
    }
}

void SceneEnd::clean()
{
    if (bgm != nullptr) {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
    }
}

void SceneEnd::updateUnderlineTimer(float deltaTime)
{
    underlineTimer += deltaTime;
    if (underlineTimer > 1.0f)
        underlineTimer = 0.0f;
}

void SceneEnd::renderPhase1()
{
    int score = game.getFinalScore();
    std::string scoreText = "你的得分是：" + std::to_string(score);
    std::string gameOver = "Game Over";
    std::string instrutionText = "请输入你的名字，按回车键确认";
    game.renderTextCentered(scoreText, 0.1f, false);
    game.renderTextCentered(gameOver, 0.4f, true);
    game.renderTextCentered(instrutionText, 0.6f, false);

    if (!name.empty()) {
        SDL_Point p = game.renderTextCentered(name, 0.8f, false);
        if (underlineTimer > 0.5f)
            game.renderTextPos("_", p.x, p.y);
    } else if (underlineTimer > 0.5f)
        game.renderTextCentered("_", 0.8f, false);
}

void SceneEnd::renderPhase2()
{
    game.renderTextCentered("得分榜", 0.1f, true);
    int i = 1;
    int offsetY = 50;
    int startX = 50;
    int startY = static_cast<int>(game.getWindowHeight() * 0.2f);
    for (auto item : game.getLeaderBoard()) {
        std::string name = std::to_string(i) + ". " + item.second;
        std::string score = std::to_string(item.first);
        int y = startY + offsetY * i;
        // 渲染name
        game.renderTextPos(name, startX, y);
        // 渲染score
        game.renderTextPos(score, startX, y, false);
        i++;
    }
    if (underlineTimer > 0.5f)
        game.renderTextCentered("按 J 键重新开始游戏", 0.8f, false);
}

void SceneEnd::removeLastUTF8Char(std::string& str)
{
    if (name.empty())
        return;

    if ((name.back() & 0b10000000) == 0b10000000) { // 判断是中文
        name.pop_back();
        while (!name.empty() && ((name.back() & 0b11000000) != 0b11000000)) {
            name.pop_back();
        }
    }
    name.pop_back();
}
