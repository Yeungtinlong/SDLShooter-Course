#include "SceneMain.h"
#include "Game.h"
#include "SceneEnd.h"
#include "SceneTitle.h"
#include <SDL.h>
#include <SDL_image.h>
#include <random>

SceneMain::SceneMain()
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::init()
{
    // 加载背景音乐
    bgm = Mix_LoadMUS("assets/music/03_Racing_Through_Asteroids_Loop.ogg");
    if (bgm == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Mix_LoadMUS Error: %s\n", SDL_GetError());
    }
    Mix_PlayMusic(bgm, -1);

    // 读取ui Health
    uiHealth = IMG_LoadTexture(game.getRenderer(), "assets/image/Health UI Black.png");

    // 读取音效资源
    sounds["player_shoot"] = Mix_LoadWAV("assets/sound/laser_shoot4.wav");
    sounds["enemy_shoot"] = Mix_LoadWAV("assets/sound/xs_laser.wav");
    sounds["player_explode"] = Mix_LoadWAV("assets/sound/explosion1.wav");
    sounds["enemy_explode"] = Mix_LoadWAV("assets/sound/explosion3.wav");
    sounds["hit"] = Mix_LoadWAV("assets/sound/eff11.wav");
    sounds["get_item"] = Mix_LoadWAV("assets/sound/eff5.wav");

    // 载入字体
    scoreFont = TTF_OpenFont("assets/font/VonwaonBitmap-12px.ttf", 24);
    if (scoreFont == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_OpenFont Error: %s\n", SDL_GetError());
        return;
    }

    // 初始化随机数生成器
    std::random_device rd;
    gen = std::mt19937 { rd() };
    dis = std::uniform_real_distribution<float> { 0.0f, 1.0f };
    // 初始化玩家
    player.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/SpaceShip.png");
    if (player.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "IMG_LoadTexture Error: %s\n", SDL_GetError());
        return;
    }
    if (SDL_QueryTexture(player.texture, nullptr, nullptr, &player.width, &player.height) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_QueryTexture Error: %s\n", SDL_GetError());
        return;
    }
    player.width /= 4;
    player.height /= 4;
    player.position.x = game.getWindowWidth() / 2 - player.width / 2;
    player.position.y = game.getWindowHeight() - player.height;

    // 初始化玩家子弹模板
    projectilePlayerTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/laser-1.png");
    if (SDL_QueryTexture(projectilePlayerTemplate.texture, nullptr, nullptr, &projectilePlayerTemplate.width, &projectilePlayerTemplate.height) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_QueryTexture Error: %s\n", SDL_GetError());
        return;
    }
    projectilePlayerTemplate.width /= 4;
    projectilePlayerTemplate.height /= 4;

    // 初始化敌机子弹模板
    projectileEnemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/bullet-1.png");
    if (SDL_QueryTexture(projectileEnemyTemplate.texture, nullptr, nullptr, &projectileEnemyTemplate.width, &projectileEnemyTemplate.height) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_QueryTexture Error: %s\n", SDL_GetError());
        return;
    }
    projectileEnemyTemplate.width /= 4;
    projectileEnemyTemplate.height /= 4;

    // 初始化敌机模板
    enemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/insect-1.png");
    if (SDL_QueryTexture(enemyTemplate.texture, nullptr, nullptr, &enemyTemplate.width, &enemyTemplate.height) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_QueryTexture Error: %s\n", SDL_GetError());
        return;
    }
    enemyTemplate.width /= 4;
    enemyTemplate.height /= 4;

    // 初始化爆炸模板
    explosionTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/effect/explosion.png");
    if (SDL_QueryTexture(explosionTemplate.texture, nullptr, nullptr, &explosionTemplate.width, &explosionTemplate.height) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_QueryTexture Error: %s\n", SDL_GetError());
        return;
    }
    explosionTemplate.totalFrame = explosionTemplate.width / explosionTemplate.height;
    explosionTemplate.width = explosionTemplate.height;
    // SDL_Log("total frame: %d, width: %d, height: %d\n", explosionTemplate.totalFrame, explosionTemplate.width, explosionTemplate.height);

    itemLifeTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/bonus_life.png");
    if (SDL_QueryTexture(itemLifeTemplate.texture, nullptr, nullptr, &itemLifeTemplate.width, &itemLifeTemplate.height) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_QueryTexture Error: %s\n", SDL_GetError());
        return;
    };
    itemLifeTemplate.width /= 4;
    itemLifeTemplate.height /= 4;
}

void SceneMain::handleEvent(SDL_Event* event)
{
    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            game.changeScene(new SceneTitle());
        }
    }
}

void SceneMain::update(float deltaTime)
{
    // 检查输入
    keyboardControl(deltaTime);
    // 生成敌机
    spawnEnemy();
    // 更新玩家
    updatePlayer(deltaTime);
    // 更新敌机
    updateEnemies(deltaTime);
    // 更新玩家子弹
    updatePlayerProjectiles(deltaTime);
    // 更新敌机子弹
    updateEnemyProjectiles(deltaTime);
    // 更新爆炸
    updateExplosions(deltaTime);
    // 更新道具
    updateItems(deltaTime);

    if (isDead)
        changeSceneDelayed(deltaTime, 3.0f);
}

void SceneMain::render()
{
    // 渲染玩家子弹
    renderPlayerProjectiles();
    // 渲染敌机子弹
    renderEnemyProjectiles();
    // 渲染玩家
    renderPlayer();
    // 渲染敌机
    renderEnemies();
    // 渲染道具
    renderItems();
    // 渲染爆炸
    renderExplosions();

    // 渲染UI
    renderUI();
}

void SceneMain::clean()
{
    // 清理玩家子弹
    for (auto projectile : projectilesPlayer) {
        if (projectile != nullptr) {
            delete projectile;
        }
    }
    projectilesPlayer.clear();

    // 清理敌机子弹
    for (auto projectile : projectilesEnemy) {
        if (projectile != nullptr) {
            delete projectile;
        }
    }
    projectilesEnemy.clear();

    // 清理敌机
    for (auto enemy : enemies) {
        if (enemy != nullptr) {
            delete enemy;
        }
    }
    enemies.clear();
    // 清理爆炸
    for (auto explosion : explosions) {
        if (explosion != nullptr) {
            delete explosion;
        }
    }
    explosions.clear();

    // 清理道具
    for (auto item : items) {
        if (item != nullptr) {
            delete item;
        }
    }
    items.clear();

    // 清理UI
    if (uiHealth != nullptr) {
        SDL_DestroyTexture(uiHealth);
    }

    // 清理字体
    if (scoreFont != nullptr) {
        TTF_CloseFont(scoreFont);
    }

    // 清理音效
    for (auto sound : sounds) {
        if (sound.second != nullptr) {
            Mix_FreeChunk(sound.second);
        }
    }
    sounds.clear();

    // 清理玩家
    if (player.texture != nullptr) {
        SDL_DestroyTexture(player.texture);
    }

    // 清理玩家子弹模板
    if (projectilePlayerTemplate.texture != nullptr) {
        SDL_DestroyTexture(projectilePlayerTemplate.texture);
    }

    // 清理敌机模板
    if (enemyTemplate.texture != nullptr) {
        SDL_DestroyTexture(enemyTemplate.texture);
    }

    // 清理敌机子弹模板
    if (projectileEnemyTemplate.texture != nullptr) {
        SDL_DestroyTexture(projectileEnemyTemplate.texture);
    }

    // 清理道具模板
    if (itemLifeTemplate.texture != nullptr) {
        SDL_DestroyTexture(itemLifeTemplate.texture);
    }

    if (bgm != nullptr) {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
    }
}

void SceneMain::keyboardControl(float deltaTime)
{
    if (isDead)
        return;

    const u_int8_t* keyboardState = SDL_GetKeyboardState(nullptr);
    SDL_FPoint vec { 0, 0 };

    if (keyboardState[SDL_SCANCODE_W]) {
        vec.y -= player.moveSpeed;
    }
    if (keyboardState[SDL_SCANCODE_S]) {
        vec.y += player.moveSpeed;
    }
    if (keyboardState[SDL_SCANCODE_A]) {
        vec.x -= player.moveSpeed;
    }
    if (keyboardState[SDL_SCANCODE_D]) {
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
    if (keyboardState[SDL_SCANCODE_J]) {
        auto currentTime = SDL_GetTicks();
        if (currentTime - player.lastShootTime > player.cooldown) {
            player.lastShootTime = currentTime;
            shootPlayer();
        }
    }
}

void SceneMain::updatePlayerProjectiles(float deltaTime)
{
    for (auto it = projectilesPlayer.begin(); it != projectilesPlayer.end();) {
        auto projectile = *it;
        projectile->position.y -= projectile->moveSpeed * deltaTime;
        if (projectile->position.y - projectile->height < 0) {
            delete projectile;
            it = projectilesPlayer.erase(it);
        } else {
            bool hit = false;
            // 子弹碰撞检测
            for (auto& enemy : enemies) {
                SDL_Rect projectileRect { static_cast<int>(projectile->position.x), static_cast<int>(projectile->position.y), projectile->width, projectile->height };
                SDL_Rect enemyRect { static_cast<int>(enemy->position.x), static_cast<int>(enemy->position.y), enemy->width, enemy->height };
                if (SDL_HasIntersection(&projectileRect, &enemyRect)) {
                    enemy->currentHealth -= projectile->damage;
                    delete projectile;
                    it = projectilesPlayer.erase(it);
                    hit = true;
                    Mix_PlayChannel(-1, sounds["hit"], 0);
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
    for (auto* projectile : projectilesPlayer) {
        SDL_Rect rect { static_cast<int>(projectile->position.x), static_cast<int>(projectile->position.y), projectile->width, projectile->height };
        if (SDL_RenderCopy(game.getRenderer(), projectile->texture, nullptr, &rect) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_RenderCopy Error: %s\n", SDL_GetError());
        }
    }
}

void SceneMain::spawnEnemy()
{
    if (dis(gen) > 1 / 60.0f)
        return;
    Enemy* enemy = new Enemy(enemyTemplate);
    enemy->position.x = dis(gen) * (game.getWindowWidth() - enemy->width);
    enemy->position.y = -enemy->height;
    enemies.push_back(enemy);
}

void SceneMain::updatePlayer(float deltaTime)
{
    if (isDead)
        return;

    if (player.currentHealth <= 0) {
        // TODO: Game over
        isDead = true;
        Explosion* explosion = new Explosion(explosionTemplate);
        explosion->position.x = player.position.x + player.width / 2 - explosion->width / 2;
        explosion->position.y = player.position.y + player.height / 2 - explosion->height / 2;
        explosion->startTime = SDL_GetTicks();
        explosions.push_back(explosion);
        Mix_PlayChannel(-1, sounds["player_explode"], 0);
        game.setFinalScore(score);
        return;
    }
    SDL_Rect playerRect = { static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height };
    for (auto it = enemies.begin(); it != enemies.end(); it++) {
        auto* enemy = *it;
        SDL_Rect enemyRect = { static_cast<int>(enemy->position.x), static_cast<int>(enemy->position.y), enemy->width, enemy->height };
        if (SDL_HasIntersection(&playerRect, &enemyRect)) {
            enemy->currentHealth = 0;
            player.currentHealth--;
        }
    }
}

void SceneMain::updateEnemies(float deltaTime)
{
    auto currentTime = SDL_GetTicks();
    for (auto it = enemies.begin(); it != enemies.end();) {
        auto enemy = *it;
        if (enemy->currentHealth <= 0) {
            enemyExplode(enemy);
            it = enemies.erase(it);
            continue;
        }

        enemy->position.y += enemy->moveSpeed * deltaTime;
        if (enemy->position.y > game.getWindowHeight()) {
            delete enemy;
            it = enemies.erase(it);
            // SDL_Log("Player Projectile Removed.");
        } else {
            if (!isDead && currentTime - enemy->lastShootTime > enemy->cooldown) {
                enemy->lastShootTime = currentTime;
                shootEnemy(enemy);
            }

            ++it;
        }
    }
}

void SceneMain::renderPlayer()
{
    if (!isDead) {
        SDL_Rect rect {
            static_cast<int>(player.position.x),
            static_cast<int>(player.position.y),
            player.width,
            player.height
        };
        SDL_RenderCopy(game.getRenderer(), player.texture, nullptr, &rect);
    }
}

void SceneMain::renderEnemies()
{
    for (auto& enemy : enemies) {
        SDL_Rect rect { static_cast<int>(enemy->position.x), static_cast<int>(enemy->position.y), enemy->width, enemy->height };
        if (SDL_RenderCopy(game.getRenderer(), enemy->texture, nullptr, &rect) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_RenderCopy Error: %s\n", SDL_GetError());
        }
    }
}

void SceneMain::updateEnemyProjectiles(float deltaTime)
{
    for (auto it = projectilesEnemy.begin(); it != projectilesEnemy.end();) {
        auto projectile = *it;
        projectile->position.x += projectile->direction.x * projectile->moveSpeed * deltaTime;
        projectile->position.y += projectile->direction.y * projectile->moveSpeed * deltaTime;
        if (projectile->position.y > game.getWindowHeight() || projectile->position.x < -projectile->width || projectile->position.x > game.getWindowWidth() || projectile->position.y < -projectile->height) {
            delete projectile;
            it = projectilesEnemy.erase(it);
        } else {
            // 子弹碰撞检测
            SDL_Rect projectileRect { static_cast<int>(projectile->position.x), static_cast<int>(projectile->position.y), projectile->width, projectile->height };
            SDL_Rect playerRect { static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height };
            if (!isDead && SDL_HasIntersection(&projectileRect, &playerRect)) {
                player.currentHealth -= projectile->damage;
                delete projectile;
                it = projectilesEnemy.erase(it);
                Mix_PlayChannel(-1, sounds["hit"], 0);
                break;
            } else {
                ++it;
            }
        }
    }
}

void SceneMain::renderEnemyProjectiles()
{
    for (auto* projectile : projectilesEnemy) {
        SDL_Rect rect { static_cast<int>(projectile->position.x), static_cast<int>(projectile->position.y), projectile->width, projectile->height };
        SDL_Point center { projectile->width / 2, projectile->height / 2 };
        if (SDL_RenderCopyEx(game.getRenderer(), projectile->texture, nullptr, &rect, atan2(projectile->direction.y, projectile->direction.x) * 180 / M_PI - 90, &center, SDL_RendererFlip::SDL_FLIP_NONE) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_RenderCopy Error: %s\n", SDL_GetError());
        }
    }
}

void SceneMain::enemyExplode(Enemy* enemy)
{
    Explosion* explosion = new Explosion(explosionTemplate);
    explosion->position.x = enemy->position.x + enemy->width / 2 - explosion->width / 2;
    explosion->position.y = enemy->position.y + enemy->height / 2 - explosion->height / 2;
    explosion->startTime = SDL_GetTicks();
    explosions.push_back(explosion);
    if (dis(gen) < 0.5f) {
        dropItem(enemy);
    }
    delete enemy;
    score += 10;
    Mix_PlayChannel(-1, sounds["enemy_explode"], 0);
}

void SceneMain::updateExplosions(float)
{
    auto currentTime = SDL_GetTicks();
    for (auto it = explosions.begin(); it != explosions.end();) {
        Explosion* explosion = *it;
        explosion->currentFrame = (currentTime - explosion->startTime) * explosion->fps / 1000.0f;
        if (explosion->currentFrame > explosion->totalFrame) {
            delete explosion;
            it = explosions.erase(it);
        } else {
            // SDL_Log("currentFrame: %d", explosion->currentFrame);
            it++;
        }
    }
}

void SceneMain::updateItems(float deltaTime)
{
    for (auto it = items.begin(); it != items.end();) {
        auto item = *it;
        // 更新位置
        item->position.x += item->direction.x * item->speed * deltaTime;
        item->position.y += item->direction.y * item->speed * deltaTime;

        // 处理边缘反弹
        if (item->bounceCount > 0) {
            bool hasbounce = false;
            if (item->position.x < 0 || item->position.x + item->width > game.getWindowWidth()) {
                item->direction.x = -item->direction.x;
                hasbounce = true;
            }
            if (item->position.y < 0 || item->position.y + item->height > game.getWindowHeight()) {
                item->direction.y = -item->direction.y;
                hasbounce = true;
            }
            if (hasbounce) {
                item->bounceCount--;
            }
        }

        // 超出屏幕范围则删除
        if (item->position.x < -item->width || item->position.x > game.getWindowWidth() || item->position.y < -item->height || item->position.y > game.getWindowHeight()) {
            it = items.erase(it);
            delete item;
        } else {
            // 判断玩家拾取物品
            SDL_Rect itemRect { static_cast<int>(item->position.x), static_cast<int>(item->position.y), item->width, item->height };
            SDL_Rect playerRect { static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height };
            if (!isDead && SDL_HasIntersection(&itemRect, &playerRect)) {
                playerGetItem(item);
                it = items.erase(it);
                delete item;
            } else {
                it++;
            }
        }
    }
}

void SceneMain::changeSceneDelayed(float deltaTime, float delay)
{
    timerEnd += deltaTime;
    if (timerEnd > delay)
        game.changeScene(new SceneEnd());
}

void SceneMain::renderExplosions()
{
    for (const Explosion* explosion : explosions) {
        SDL_Rect srcRect = { explosion->width * explosion->currentFrame, 0, explosion->width, explosion->height };
        SDL_Rect dstRect = { static_cast<int>(explosion->position.x), static_cast<int>(explosion->position.y), explosion->width, explosion->height };
        if (SDL_RenderCopy(game.getRenderer(), explosion->texture, &srcRect, &dstRect) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_RenderCopy Error: %s\n", SDL_GetError());
        }
    }
}

void SceneMain::renderItems()
{
    for (auto item : items) {
        SDL_Rect rect = { static_cast<int>(item->position.x), static_cast<int>(item->position.y), item->width, item->height };
        if (SDL_RenderCopy(game.getRenderer(), item->texture, nullptr, &rect) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_RenderCopy Error: %s\n", SDL_GetError());
        }
    }
}

void SceneMain::renderUI()
{
    // 渲染血条
    int x = 10;
    int y = 10;
    int size = 32;
    int offset = 0;
    for (int i = 0; i < player.maxHealth; i++) {
        if (i >= player.currentHealth) {
            SDL_SetTextureColorMod(uiHealth, 128, 128, 128);
        } else {
            SDL_SetTextureColorMod(uiHealth, 255, 255, 255);
        }
        SDL_Rect dstrect { x + i * (size + offset), y, size, size };
        SDL_RenderCopy(game.getRenderer(), uiHealth, nullptr, &dstrect);
    }

    // 渲染得分
    auto text = "SCORE:" + std::to_string(score);
    SDL_Color color { 255, 255, 255, 255 };
    SDL_Surface* surface = TTF_RenderUTF8_Solid(scoreFont, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(game.getRenderer(), surface);
    SDL_Rect dstrect { game.getWindowWidth() - surface->w - 10, 10, surface->w, surface->h };
    SDL_RenderCopy(game.getRenderer(), texture, nullptr, &dstrect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void SceneMain::dropItem(Enemy* enemy)
{
    Item* item = new Item(itemLifeTemplate);
    item->position.x = enemy->position.x + enemy->width / 2 + item->width / 2;
    item->position.y = enemy->position.y + enemy->height / 2 + item->height / 2;
    float angle = dis(gen) * 2 * M_PI;
    // length = 1
    item->direction.x = cos(angle);
    item->direction.y = sin(angle);
    items.push_back(item);
}

void SceneMain::playerGetItem(Item* item)
{
    if (item->type == ItemType::Life) {
        player.currentHealth++;
        if (player.currentHealth > player.maxHealth) {
            player.currentHealth = player.maxHealth;
        }
    }
    score += 5;
    Mix_PlayChannel(-1, sounds["get_item"], 0);
}

SDL_FPoint SceneMain::getDirection(Enemy* enemy)
{
    float deltaX = (player.position.x + player.width / 2) - (enemy->position.x + enemy->width / 2);
    float deltaY = (player.position.y + player.height / 2) - (enemy->position.y + enemy->height / 2);

    float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

    return SDL_FPoint { deltaX /= distance, deltaY /= distance };
}

void SceneMain::shootPlayer()
{
    auto* projectile = new ProjectilePlayer(projectilePlayerTemplate);
    projectile->position.x = player.position.x + player.width / 2 - projectile->width / 2;
    projectile->position.y = player.position.y;
    projectilesPlayer.push_back(projectile);
    Mix_PlayChannel(0, sounds["player_shoot"], 0);
    // SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Player shoot!");
}

void SceneMain::shootEnemy(Enemy* enemy)
{
    auto* projectile = new ProjectileEnemy(projectileEnemyTemplate);
    projectile->position.x = enemy->position.x + enemy->width / 2 - projectile->width / 2;
    projectile->position.y = enemy->position.y + enemy->height / 2 - projectile->height / 2;

    projectile->direction = getDirection(enemy);
    projectilesEnemy.push_back(projectile);
    Mix_PlayChannel(-1, sounds["enemy_shoot"], 0);
}
