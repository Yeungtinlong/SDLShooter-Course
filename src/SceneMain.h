#ifndef SCENEMAIN_H
#define SCENEMAIN_H

#include "Scene.h"
#include "Object.h"
#include "Game.h"
#include <list>
#include <random>

class SceneMain : public Scene
{
public:
    SceneMain();
    ~SceneMain();

    void init() override;
    void handleEvent(SDL_Event *event) override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;

    void keyboardControl(float deltaTime);
    void shootPlayer();
    void shootEnemy(Enemy *enemy);
    void updatePlayerProjectiles(float deltaTime);
    void renderPlayerProjectiles();
    void spawnEnemy();
    void updatePlayer(float deltaTime);
    void updateEnemies(float deltaTime);
    void renderEnemies();
    void updateEnemyProjectiles(float deltaTime);
    void renderEnemyProjectiles();
    void enemyExplode(Enemy *enemy);

    SDL_FPoint getDirection(Enemy *enemy);

private:
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;
    Game &game;
    Player player;
    bool isDead;
    ProjectilePlayer projectilePlayerTemplate;
    ProjectileEnemy projectileEnemyTemplate;
    Enemy enemyTemplate;

    std::list<ProjectilePlayer *> projectilesPlayer;
    std::list<Enemy *> enemies;
    std::list<ProjectileEnemy *> projectilesEnemy;
};

#endif