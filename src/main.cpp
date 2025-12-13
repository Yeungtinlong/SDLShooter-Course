#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "Game.h"
// #include <unistd.h>
// void printcwd()
// {
//     char cwd[PATH_MAX];
//     if (getcwd(cwd, sizeof(cwd)) != NULL)
//     {
//         std::cout << cwd << std::endl;
//     }
// }

int main(int argc, char *argv[])
{
    Game::getInstance().init();
    Game::getInstance().run();

    // if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    // {
    //     std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    //     return 1;
    // }

    // SDL_Window *window = SDL_CreateWindow("Hello World!", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
    // SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // // 初始化SDL_image
    // if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG))
    // {
    //     std::cerr << "IMG_Init Error" << SDL_GetError() << std::endl;
    //     return 1;
    // }

    // SDL_Texture *texture = IMG_LoadTexture(renderer, "assets/image/bg.png");

    // // 初始化SDL_mixer
    // if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    // {
    //     std::cerr << "Mix_OpenAudio Error: " << SDL_GetError() << std::endl;
    //     return 1;
    // }

    // // 初始化SDL_ttf
    // if (TTF_Init() != 0)
    // {
    //     std::cerr << "TTF_Init Error: " << SDL_GetError() << std::endl;
    //     return 1;
    // }

    // TTF_Font *font = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 24);
    // // 创建文本纹理
    // SDL_Color color{255, 255, 255, 255};
    // SDL_Surface *surface = TTF_RenderUTF8_Solid(font, "Hello中文也可以", color);
    // SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, surface);

    // Mix_Music *music = Mix_LoadMUS("assets/music/06_Battle_in_Space_Intro.ogg");
    // Mix_PlayMusic(music, -1);

    // // do something
    // while (true)
    // {
    //     SDL_Event event;
    //     if (SDL_PollEvent(&event))
    //     {
    //         if (event.type == SDL_QUIT)
    //         {
    //             break;
    //         }
    //     }
    //     // 清屏
    //     SDL_RenderClear(renderer);

    //     // 画矩形
    //     SDL_Rect rect{100, 100, 200, 200};
    //     SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    //     SDL_RenderFillRect(renderer, &rect);
        
    //     // 画图片
    //     SDL_Rect dstrect{200, 200, 300, 400};
    //     SDL_RenderCopy(renderer, texture, NULL, &dstrect);

    //     // 画文本纹理
    //     SDL_Rect textrect{200, 200, surface->w, surface->h};
    //     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //     SDL_RenderCopy(renderer, textTexture, NULL, &textrect);

    //     // Renderer颜色清为黑色
    //     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    //     // 切换backbuffer
    //     SDL_RenderPresent(renderer);
    // }

    // // 清理图片资源
    // SDL_DestroyTexture(texture);
    // IMG_Quit();

    // // 清理音乐
    // Mix_FreeMusic(music);
    // Mix_CloseAudio();
    // Mix_Quit();

    // // 清理字体
    // SDL_FreeSurface(surface);
    // SDL_DestroyTexture(textTexture);
    // TTF_CloseFont(font);
    // TTF_Quit();

    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    // SDL_Quit();

    return 0;
}