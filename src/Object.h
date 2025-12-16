#ifndef OBJECT_H
#define OBJECT_H

// #include "Scene.h"
#include <SDL.h>

enum class ItemType
{
    Life,
    Shield,
    Time,
};

struct Player
{
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int moveSpeed = 300;
    int currentHealth = 3;
    int maxHealth = 3;
    Uint32 cooldown = 300;
    Uint32 lastShootTime = 0;
};

struct ProjectilePlayer
{
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int moveSpeed = 600;
    int damage = 1;
};

struct Enemy
{
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int moveSpeed = 150;
    int currentHealth = 2;
    Uint32 cooldown = 2000;
    Uint32 lastShootTime = 0;
};

struct ProjectileEnemy
{
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    SDL_FPoint direction = {0, 0};
    int width = 0;
    int height = 0;
    int moveSpeed = 400;
    int damage = 1;
};

struct Explosion
{
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int currentFrame = 0;
    int totalFrame = 0;
    Uint32 startTime = 0;
    Uint32 fps = 6;
};

struct Item
{
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    SDL_FPoint direction = {0, 0};
    int width = 0;
    int height = 0;
    float speed = 200;
    int bounceCount = 3;
    ItemType type = ItemType::Life;
};

struct Background
{
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    float offset = 0;
    float speed = 30.0f;
};

#endif