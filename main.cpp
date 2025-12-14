#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{
    // int a = 800;
    // std::cout << "hello world!, " << argv[0] << std::endl;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Hello World!", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // do something

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}