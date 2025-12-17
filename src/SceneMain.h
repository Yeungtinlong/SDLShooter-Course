#ifndef SCENEMAIN_H
#define SCENEMAIN_H

#include "Game.h"
#include "Object.h"
#include "Scene.h"
#include <SDL_mixer.h>
#include <list>
#include <map>
#include <random>

class SceneMain : public Scene {
public:
    SceneMain();
    ~SceneMain();

    // 生命周期函数
    void init() override;
    void handleEvent(SDL_Event* event) override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;

private:
    void keyboardControl(float deltaTime);

    // gameplay
    void shootPlayer();
    void spawnEnemy();
    void shootEnemy(Enemy* enemy);
    void enemyExplode(Enemy* enemy);
    void dropItem(Enemy* enemy);
    void playerGetItem(Item* item);

    // update
    void updatePlayerProjectiles(float deltaTime);
    void updatePlayer(float deltaTime);
    void updateEnemies(float deltaTime);
    void updateEnemyProjectiles(float deltaTime);
    void updateExplosions(float deltaTime);
    void updateItems(float deltaTime);

    // render
    void renderPlayer();
    void renderEnemies();
    void renderPlayerProjectiles();
    void renderEnemyProjectiles();
    void renderExplosions();
    void renderItems();
    void renderUI();

    SDL_FPoint getDirection(Enemy* enemy);

private:
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;
    Player player {};
    bool isDead = false;
    ProjectilePlayer projectilePlayerTemplate {};
    ProjectileEnemy projectileEnemyTemplate {};
    Enemy enemyTemplate {};
    Explosion explosionTemplate {};
    Item itemLifeTemplate {};
    Mix_Music* bgm = nullptr;
    SDL_Texture *uiHealth = nullptr;
    int score = 0;
    TTF_Font *scoreFont;

    std::list<ProjectilePlayer*> projectilesPlayer;
    std::list<Enemy*> enemies;
    std::list<ProjectileEnemy*> projectilesEnemy;
    std::list<Explosion*> explosions;
    std::list<Item*> items;
    std::map<std::string, Mix_Chunk*> sounds;
};

#endif