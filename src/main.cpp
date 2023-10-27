#include "game.hpp"
#include <iostream>

Game *game = nullptr;

int frameDelay = 1000 / FPS;

int main(int argc, char *argv[]){
    game = new Game();

    game->init("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, false);
    

    while(game->running()){
        Uint32 frameStart = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if(frameDelay > frameTime){
            SDL_Delay(frameDelay - frameTime);
        }

        //std::cin.get();
    }

    game->clean();

    return 0;
}