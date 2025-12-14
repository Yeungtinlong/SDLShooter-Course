#include "SceneMain.h"
#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <random>

SceneMain::SceneMain() : game(Game::getInstance())
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::init()
{
    std::random_device rd;
    gen = std::mt19937{rd()};
    dis = std::uniform_real_distribution<float>{0.0f, 1.0f};

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

    // 初始化玩家子弹模板
    projectilePlayerTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/laser-1.png");
    if (SDL_QueryTexture(projectilePlayerTemplate.texture, nullptr, nullptr, &projectilePlayerTemplate.width, &projectilePlayerTemplate.height) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_QueryTexture Error: %s\n", SDL_GetError());
        return;
    }
    projectilePlayerTemplate.width /= 4;
    projectilePlayerTemplate.height /= 4;

    // 初始化敌机子弹模板
    projectileEnemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/bullet-1.png");
    if (SDL_QueryTexture(projectileEnemyTemplate.texture, nullptr, nullptr, &projectileEnemyTemplate.width, &projectileEnemyTemplate.height) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_QueryTexture Error: %s\n", SDL_GetError());
        return;
    }
    projectileEnemyTemplate.width /= 4;
    projectileEnemyTemplate.height /= 4;

    // 初始化敌机模板
    enemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/insect-1.png");
    if (SDL_QueryTexture(enemyTemplate.texture, nullptr, nullptr, &enemyTemplate.width, &enemyTemplate.height) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_QueryTexture Error: %s\n", SDL_GetError());
        return;
    }
    enemyTemplate.width /= 4;
    enemyTemplate.height /= 4;
}

void SceneMain::handleEvent(SDL_Event *event)
{
}

void SceneMain::update(float deltaTime)
{
    keyboardControl(deltaTime);
    updatePlayer(deltaTime);
    updateEnemies(deltaTime);
    updatePlayerProjectiles(deltaTime);
    updateEnemyProjectiles(deltaTime);
    spawnEnemy();
}

void SceneMain::render()
{
    SDL_RenderClear(game.getRenderer());

    // 渲染玩家子弹
    renderPlayerProjectiles();
    // 渲染敌机子弹
    renderEnemyProjectiles();
    // 渲染玩家
    if (!isDead)
    {
        SDL_Rect rect{
            static_cast<int>(player.position.x),
            static_cast<int>(player.position.y),
            player.width,
            player.height};
        SDL_RenderCopy(game.getRenderer(), player.texture, nullptr, &rect);
    }
    // 渲染敌机
    renderEnemies();
}

void SceneMain::clean()
{
    // 清理玩家子弹
    for (auto projectile : projectilesPlayer)
    {
        if (projectile != nullptr)
        {
            delete projectile;
        }
    }
    projectilesPlayer.clear();

    // 清理敌机子弹
    for (auto projectile : projectilesEnemy)
    {
        if (projectile != nullptr)
        {
            delete projectile;
        }
    }
    projectilesEnemy.clear();

    // 清理敌机
    for (auto enemy : enemies)
    {
        if (enemy != nullptr)
        {
            delete enemy;
        }
    }
    enemies.clear();

    // 清理玩家
    if (player.texture != nullptr)
    {
        SDL_DestroyTexture(player.texture);
    }

    // 清理玩家子弹模板
    if (projectilePlayerTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(projectilePlayerTemplate.texture);
    }

    // 清理敌机模板
    if (enemyTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(enemyTemplate.texture);
    }

    // 清理敌机子弹模板
    if (projectileEnemyTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(projectileEnemyTemplate.texture);
    }
}

void SceneMain::keyboardControl(float deltaTime)
{
    if (isDead)
        return;

    const u_int8_t *keyboardState = SDL_GetKeyboardState(nullptr);
    SDL_FPoint vec{0, 0};

    if (keyboardState[SDL_SCANCODE_W])
    {
        vec.y -= player.moveSpeed;
    }
    if (keyboardState[SDL_SCANCODE_S])
    {
        vec.y += player.moveSpeed;
    }
    if (keyboardState[SDL_SCANCODE_A])
    {
        vec.x -= player.moveSpeed;
    }
    if (keyboardState[SDL_SCANCODE_D])
    {
        vec.x += player.moveSpeed;
    }
    // TODO: 移动应该使用命令，而不是直接改变位置
    player.position.x += vec.x * deltaTime;
    player.position.y += vec.y * deltaTime;

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

    // 控制子弹发射
    if (keyboardState[SDL_SCANCODE_J])
    {
        auto currentTime = SDL_GetTicks();
        if (currentTime - player.lastShootTime > player.cooldown)
        {
            player.lastShootTime = currentTime;
            shootPlayer();
        }
    }
}

void SceneMain::updatePlayerProjectiles(float deltaTime)
{
    for (auto it = projectilesPlayer.begin(); it != projectilesPlayer.end();)
    {
        auto projectile = *it;
        projectile->position.y -= projectile->moveSpeed * deltaTime;
        if (projectile->position.y - projectile->height < 0)
        {
            delete projectile;
            it = projectilesPlayer.erase(it);
        }
        else
        {
            bool hit = false;
            // 子弹碰撞检测
            for (auto &enemy : enemies)
            {
                SDL_Rect projectileRect{static_cast<int>(projectile->position.x), static_cast<int>(projectile->position.y), projectile->width, projectile->height};
                SDL_Rect enemyRect{static_cast<int>(enemy->position.x), static_cast<int>(enemy->position.y), enemy->width, enemy->height};
                if (SDL_HasIntersection(&projectileRect, &enemyRect))
                {
                    enemy->currentHealth -= projectile->damage;
                    delete projectile;
                    it = projectilesPlayer.erase(it);
                    hit = true;
                    break;
                }
            }
            if (!hit)
                ++it;
        }
    }
}

void SceneMain::renderPlayerProjectiles()
{
    for (auto *projectile : projectilesPlayer)
    {
        SDL_Rect rect{static_cast<int>(projectile->position.x), static_cast<int>(projectile->position.y), projectile->width, projectile->height};
        if (SDL_RenderCopy(game.getRenderer(), projectile->texture, nullptr, &rect) != 0)
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_RenderCopy Error: %s\n", SDL_GetError());
        }
    }
}

void SceneMain::spawnEnemy()
{
    if (dis(gen) > 1 / 60.0f)
        return;
    Enemy *enemy = new Enemy(enemyTemplate);
    enemy->position.x = dis(gen) * (game.getWindowWidth() - enemy->width);
    enemy->position.y = -enemy->height;
    enemies.push_back(enemy);
}

void SceneMain::updatePlayer(float deltaTime)
{
    if (isDead)
        return;

    if (player.currentHealth <= 0)
    {
        // TODO: Game over
        isDead = true;
    }
}

void SceneMain::updateEnemies(float deltaTime)
{
    auto currentTime = SDL_GetTicks();
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        auto enemy = *it;
        if (enemy->currentHealth <= 0)
        {
            enemyExplode(enemy);
            it = enemies.erase(it);
            continue;
        }

        enemy->position.y += enemy->moveSpeed * deltaTime;
        if (enemy->position.y > game.getWindowHeight())
        {
            delete enemy;
            it = enemies.erase(it);
            // SDL_Log("Player Projectile Removed.");
        }
        else
        {
            if (!isDead && currentTime - enemy->lastShootTime > enemy->cooldown)
            {
                enemy->lastShootTime = currentTime;
                shootEnemy(enemy);
            }

            ++it;
        }
    }
}

void SceneMain::renderEnemies()
{
    for (auto &enemy : enemies)
    {
        SDL_Rect rect{static_cast<int>(enemy->position.x), static_cast<int>(enemy->position.y), enemy->width, enemy->height};
        if (SDL_RenderCopy(game.getRenderer(), enemy->texture, nullptr, &rect) != 0)
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_RenderCopy Error: %s\n", SDL_GetError());
        }
    }
}

void SceneMain::updateEnemyProjectiles(float deltaTime)
{
    for (auto it = projectilesEnemy.begin(); it != projectilesEnemy.end();)
    {
        auto projectile = *it;
        projectile->position.x += projectile->direction.x * projectile->moveSpeed * deltaTime;
        projectile->position.y += projectile->direction.y * projectile->moveSpeed * deltaTime;
        if (projectile->position.y > game.getWindowHeight() || projectile->position.x < -projectile->width || projectile->position.x > game.getWindowWidth() || projectile->position.y < -projectile->height)
        {
            delete projectile;
            it = projectilesEnemy.erase(it);
        }
        else
        {
            // 子弹碰撞检测
            SDL_Rect projectileRect{static_cast<int>(projectile->position.x), static_cast<int>(projectile->position.y), projectile->width, projectile->height};
            SDL_Rect playerRect{static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height};
            if (!isDead && SDL_HasIntersection(&projectileRect, &playerRect))
            {
                player.currentHealth -= projectile->damage;
                delete projectile;
                it = projectilesEnemy.erase(it);
                break;
            }
            else
            {
                ++it;
            }
        }
    }
}

void SceneMain::renderEnemyProjectiles()
{
    for (auto *projectile : projectilesEnemy)
    {
        SDL_Rect rect{static_cast<int>(projectile->position.x), static_cast<int>(projectile->position.y), projectile->width, projectile->height};
        SDL_Point center{projectile->width / 2, projectile->height / 2};
        if (SDL_RenderCopyEx(game.getRenderer(), projectile->texture, nullptr, &rect, atan2(projectile->direction.y, projectile->direction.x) * 180 / M_PI - 90, &center, SDL_RendererFlip::SDL_FLIP_NONE) != 0)
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_RenderCopy Error: %s\n", SDL_GetError());
        }
    }
}

void SceneMain::enemyExplode(Enemy *enemy)
{
    delete enemy;
}

SDL_FPoint SceneMain::getDirection(Enemy *enemy)
{
    float deltaX = (player.position.x + player.width / 2) - (enemy->position.x + enemy->width / 2);
    float deltaY = (player.position.y + player.height / 2) - (enemy->position.y + enemy->height / 2);

    float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

    return SDL_FPoint{deltaX /= distance, deltaY /= distance};
}

void SceneMain::shootPlayer()
{
    auto *projectile = new ProjectilePlayer(projectilePlayerTemplate);
    projectile->position.x = player.position.x + player.width / 2 - projectile->width / 2;
    projectile->position.y = player.position.y;
    projectilesPlayer.push_back(projectile);

    // SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Player shoot!");
}

void SceneMain::shootEnemy(Enemy *enemy)
{
    auto *projectile = new ProjectileEnemy(projectileEnemyTemplate);
    projectile->position.x = enemy->position.x + enemy->width / 2 - projectile->width / 2;
    projectile->position.y = enemy->position.y + enemy->height / 2 - projectile->height / 2;

    projectile->direction = getDirection(enemy);
    projectilesEnemy.push_back(projectile);
}
