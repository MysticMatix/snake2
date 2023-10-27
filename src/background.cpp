#include "background.hpp"

#include "game.hpp"
#include "textureManager.hpp"

Background::Background(SDL_Texture* texture){
    this->texture = texture;
}

Background::Background(const char* fileName){
    this->texture = TextureManager::LoadTexture(fileName);
}

Background::~Background(){
    SDL_DestroyTexture(texture);
}

void Background::update(){
    
}

void Background::render(){
    SDL_Rect srcRect1, srcRect2, destRect;

    srcRect1.x = 0;
    srcRect1.y = 0;

    srcRect2.x = 32;
    srcRect2.y = 0;

    srcRect1.w = srcRect2.w = 32;
    srcRect1.h = srcRect2.h = 32;

    destRect.w = destRect.h = TILE_SIZE;

    for(int x = 0; x < BOARD_WIDTH; x++){
        for(int y = 0; y < BOARD_HEIGHT; y++){
            destRect.x = x * TILE_SIZE;
            destRect.y = y * TILE_SIZE;

            if((x + y) % 2 == 0){
                TextureManager::Draw(this->texture, srcRect1, destRect, SDL_FLIP_NONE, 0);
            }else{
                TextureManager::Draw(this->texture, srcRect2, destRect, SDL_FLIP_NONE, 0);
            }
        }
    }
}

