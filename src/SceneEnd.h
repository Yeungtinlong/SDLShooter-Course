#ifndef SCENE_END_H
#define SCENE_END_H

#include "Scene.h"
#include <string>

class SceneEnd : public Scene {
public:
    SceneEnd() { };
    ~SceneEnd() { };

    void init() override;
    void handleEvent(SDL_Event* event) override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;
    
    void updateUnderlineTimer(float deltaTime);

private:
    void renderPhase1();
    void renderPhase2();
    void removeLastUTF8Char(std::string& str);

private:
    bool isTyping = true;
    std::string name {};
    float underlineTimer = 0.0f;
};

#endif