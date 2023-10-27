#include "food.hpp"

#include "game.hpp"
#include "textureManager.hpp"

#include <iostream>

Food::Food(SDL_Texture* textureSheet, int x, int y, TileState* gameboard){
    texture = textureSheet;
    this->x = x;
    this->y = y;
    this->gameboard = gameboard;

    active = false;

    srcRect.x = 0;
    srcRect.y = 0;

    srcRect.w = srcRect.h = 32;
}

Food::Food(const char* textureSheet, int x, int y, TileState* gameboard){
    texture = TextureManager::LoadTexture(textureSheet);
    this->x = x;
    this->y = y;
    this->gameboard = gameboard;

    active = false;

    srcRect.x = 0;
    srcRect.y = 0;

    srcRect.w = srcRect.h = 32;
}

Food::~Food(){
    SDL_DestroyTexture(texture);
}

void Food::update(){
    if(active){
        destRect.x = x * TILE_SIZE;
        destRect.y = y * TILE_SIZE;
        destRect.w = destRect.h = TILE_SIZE;
    }
}

void Food::render(){
    if(active){
        TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE, 0);
    }
}

bool Food::isActive(){
    return active;
}

void Food::spawn(){
    active = true;
    int empty = 0;
    for(int x = 0; x < BOARD_WIDTH; x++){
        for(int y = 0; y < BOARD_HEIGHT; y++){
            if(gameboard[y * BOARD_WIDTH +  x] == TileState::EMPTY){
                empty++;
            }
        }
    }
    int random = rand() % empty;
    int count = 0;
    for(int x = 0; x < BOARD_WIDTH; x++){
        for(int y = 0; y < BOARD_HEIGHT; y++){
            if(gameboard[y * BOARD_WIDTH + x] == TileState::EMPTY){
                if(count == random){
                    this->x = x;
                    this->y = y;
                    gameboard[y * BOARD_WIDTH + x] = TileState::FOOD;
                    return;
                }
                count++;
            }
        }
    }
    std::cout << "nowhere to spawn food" << std::endl;
    active = false;
}

void Food::despawn(){
    active = false;
    if(gameboard[y * BOARD_WIDTH + x] == TileState::FOOD) gameboard[y * BOARD_WIDTH + x] = TileState::EMPTY;
}

int Food::getX(){
    return x;
}

int Food::getY(){
    return y;
}