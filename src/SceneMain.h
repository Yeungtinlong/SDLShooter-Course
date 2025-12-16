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
    void spawnEnemy();
    void shootEnemy(Enemy *enemy);
    void enemyExplode(Enemy *enemy);
    void dropItem(Enemy *enemy);
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
    Explosion explosionTemplate;
    Item itemLifeTemplate;

    std::list<ProjectilePlayer *> projectilesPlayer;
    std::list<Enemy *> enemies;
    std::list<ProjectileEnemy *> projectilesEnemy;
    std::list<Explosion *> explosions;
    std::list<Item *> items;
};

#endif