#ifndef OBJECT_H
#define OBJECT_H

// #include "Scene.h"
#include <SDL.h>

struct Player
{
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int moveSpeed = 200;
    int currentHealth = 3;
    Uint32 cooldown = 500;
    Uint32 lastShootTime = 0;
};

struct ProjectilePlayer
{
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int moveSpeed = 400;
    int damage = 1;
};

struct Enemy
{
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int moveSpeed = 200;
    int currentHealth = 2;
    Uint32 cooldown = 1000;
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

#endif