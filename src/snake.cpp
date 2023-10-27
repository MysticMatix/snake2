#include "snake.hpp"

#include "textureManager.hpp"

#include <iostream>

const int moveTime = FPS/2;

Snake::Snake(SDL_Texture* headTexture, SDL_Texture* bodyTexture, SDL_Texture* tailTexture, int x, int y, Direction* dir, int length, TileState* gameboard){
    this->headTexture = headTexture;
    this->bodyTexture = bodyTexture;
    this->tailTexture = tailTexture;

    this->progress = 0;

    head = new SnakeHead();
    head->coordinates = new SDL_Point{x, y};
    head->angle = (int)*dir;
    head->originalDirection = dir;
    head->direction = dir;
    head->directionChanged = false;

    int xdif, ydif;
    switch (*dir){
        case Direction::UP:
            xdif = 0;
            ydif = 1;
            break;
        case Direction::DOWN:
            xdif = 0;
            ydif = -1;
            break;
        case Direction::LEFT:
            xdif = 1;
            ydif = 0;
            break;
        case Direction::RIGHT:
            xdif = -1;
            ydif = 0;
            break;
    }

    length++;

    for(int i = 1; i <= length; i++){
        SnakeTile* tile = new SnakeTile();
        tile->coordinates = new SDL_Point{x + i * xdif, y + i * ydif};
        tile->direction = dir;
        tiles.push_front(tile);
        gameboard[(y + i * ydif) * BOARD_WIDTH + (x + i * xdif)] = TileState::SNAKE;
    }

    tail = new SnakeTail();
    tail->coordinates = new SDL_Point{x + (length+1) * xdif, y + (length+1) * ydif};
    tail->direction = dir;

    gameboard[(y + (length+1) * ydif) * BOARD_WIDTH + (x + (length+1) * xdif)] = TileState::SNAKE;

    this->gameboard = gameboard;
}

Snake::Snake(const char* headTexture, const char* bodyTexture, const char* tailTexture, int x, int y, Direction* dir, int length, TileState* gameboard):
Snake(TextureManager::LoadTexture(headTexture), TextureManager::LoadTexture(bodyTexture), TextureManager::LoadTexture(tailTexture), x, y, dir, length, gameboard){

}

Snake::~Snake(){
    delete head;
    delete tail;
    for(auto tile : tiles){
        delete tile;
    }
}

void Snake::update(){

    progress++;
    if(progress >= moveTime){
        move();
        progress = 0;
    }
    else{
        //manage head angle
        
        //clockwise
        double angleDif = (int)*head->direction - head->angle;
        if(angleDif > 180) angleDif -= 360;
        if(angleDif < -180) angleDif += 360;

        head->angle += angleDif / (moveTime - progress);

        while(head->angle < 0) head->angle += 360;
        while(head->angle >= 360) head->angle -= 360;
    }
    if(progress > moveTime / 2){
        head->showNeck = true;
    }
    else{
        head->showNeck = false;
    }
}

void Snake::render(){
    if(eating){
        if((int)progress%6<4) return;
    }
    renderTail();
    renderBody();
    renderHead();
}

void Snake::tryChangeHeadDir(Direction* dir){
    if(head->directionChanged) return;
    if(*head->direction == Direction::UP && *dir == Direction::DOWN) return;
    if(*head->direction == Direction::DOWN && *dir == Direction::UP) return;
    if(*head->direction == Direction::LEFT && *dir == Direction::RIGHT) return;
    if(*head->direction == Direction::RIGHT && *dir == Direction::LEFT) return;
    if(*head->direction == *dir) return;
    
    head->direction = dir;
    head->directionChanged = true;
}

void Snake::eat(){
    eating = true;
}

void Snake::move(){
    SnakeTile* newTile = new SnakeTile();
    newTile->coordinates = new SDL_Point{head->coordinates->x, head->coordinates->y};
    newTile->direction = head->direction;
    tiles.push_back(newTile);

    gameboard[head->coordinates->y * BOARD_WIDTH + head->coordinates->x] = TileState::SNAKE;

    head->coordinates->x += *head->direction == Direction::LEFT ? -1 : *head->direction == Direction::RIGHT ? 1 : 0;
    head->coordinates->y += *head->direction == Direction::UP ? -1 : *head->direction == Direction::DOWN ? 1 : 0;

    head->originalDirection = head->direction;
    head->directionChanged = false;

    if(eating){
        eating = false;
        return;
    }

    gameboard[tail->coordinates->y * BOARD_WIDTH + tail->coordinates->x] = TileState::EMPTY;

    tail->coordinates->x = tiles.front()->coordinates->x;
    tail->coordinates->y = tiles.front()->coordinates->y;
    tail->direction = tiles.front()->direction;

    tiles.pop_front();
}

void Snake::renderHead(){
    SDL_Rect sourceRectangle;
    SDL_Rect destinationRectangle;

    if(head->showNeck){
        sourceRectangle.x = 96;
        sourceRectangle.y = 0;
        sourceRectangle.w = 32;
        sourceRectangle.h = 32;

        destinationRectangle.x = head->coordinates->x * TILE_SIZE;
        destinationRectangle.y = head->coordinates->y * TILE_SIZE;
        destinationRectangle.w = TILE_SIZE;
        destinationRectangle.h = TILE_SIZE;

        TextureManager::Draw(this->headTexture, sourceRectangle, destinationRectangle, SDL_FLIP_NONE, (int)*head->originalDirection);
    }

    sourceRectangle.x = 0;
    sourceRectangle.y = 0;
    sourceRectangle.w = 96;
    sourceRectangle.h = 96;

    destinationRectangle.x = head->coordinates->x * TILE_SIZE - TILE_SIZE;
    destinationRectangle.y = head->coordinates->y * TILE_SIZE - TILE_SIZE;
    destinationRectangle.w = 3 * TILE_SIZE;
    destinationRectangle.h = 3 * TILE_SIZE;

    switch(*head->originalDirection){
        case Direction::UP:
            destinationRectangle.y -= progress * TILE_SIZE / moveTime - TILE_SIZE;
            break;
        case Direction::DOWN:
            destinationRectangle.y += progress * TILE_SIZE / moveTime - TILE_SIZE;
            break;
        case Direction::LEFT:
            destinationRectangle.x -= progress * TILE_SIZE / moveTime - TILE_SIZE;
            break;
        case Direction::RIGHT:
            destinationRectangle.x += progress * TILE_SIZE / moveTime - TILE_SIZE;
            break;
    }

    TextureManager::Draw(this->headTexture, sourceRectangle, destinationRectangle, SDL_FLIP_NONE, head->angle);
}

void Snake::renderBody(){

    SDL_Rect sourceRectangle;
    sourceRectangle.y = 0;
    sourceRectangle.w = 32;
    sourceRectangle.h = 32;

    SDL_Rect destinationRectangle;
    destinationRectangle.w = TILE_SIZE;
    destinationRectangle.h = TILE_SIZE;

    //straigh (up/down) - 0, 0
    //.#.
    //.#.
    //.#.

    //turned (up/right) - 32, 0
    //...
    //.##
    //.#.

    //manage the angle accordingly
    
    SDL_Point* prev = tail->coordinates;
    double angle = 0;

    for(auto tile : tiles){
        SDL_Point* current = tile->coordinates;
        Direction* dir = tile->direction;

        destinationRectangle.x = current->x * TILE_SIZE;
        destinationRectangle.y = current->y * TILE_SIZE;

        if(prev->x == current->x){ // # | #
            if(prev->y < current->y){ // # /> #
                switch(*dir){
                    case Direction::UP:
                        sourceRectangle.x = 0;
                        angle = 0;
                        break;
                    case Direction::DOWN:
                        sourceRectangle.x = 0;
                        angle = 0;
                        break;
                    case Direction::LEFT:
                        sourceRectangle.x = 32;
                        angle = 180;
                        break;
                    case Direction::RIGHT:
                        sourceRectangle.x = 32;
                        angle = 270;
                        break;
                }
            }
            else{ // # </ #
                switch(*dir){ 
                    case Direction::UP:
                        sourceRectangle.x = 0;
                        angle = 0;
                        break;
                    case Direction::DOWN:
                        sourceRectangle.x = 0;
                        angle = 0;
                        break;
                    case Direction::LEFT:
                        sourceRectangle.x = 32;
                        angle = 90;
                        break;
                    case Direction::RIGHT:
                        sourceRectangle.x = 32;
                        angle = 0;
                        break;
                }
            }
        }
        else{ // # - #
            if(prev->x < current->x){ // # -> #
                switch(*dir){
                    case Direction::UP:
                        sourceRectangle.x = 32;
                        angle = 180;
                        break;
                    case Direction::DOWN:
                        sourceRectangle.x = 32;
                        angle = 90;
                        break;
                    case Direction::LEFT:
                        sourceRectangle.x = 0;
                        angle = 270;
                        break;
                    case Direction::RIGHT:
                        sourceRectangle.x = 0;
                        angle = 90;
                        break;
                }
            }
            else{ // # <- #
                switch(*dir){
                    case Direction::UP:
                        sourceRectangle.x = 32;
                        angle = 270;
                        break;
                    case Direction::DOWN:
                        sourceRectangle.x = 32;
                        angle = 0;
                        break;
                    case Direction::LEFT:
                        sourceRectangle.x = 0;
                        angle = 90;
                        break;
                    case Direction::RIGHT:
                        sourceRectangle.x = 0;
                        angle = 270;
                        break;
                }
            }
        }

        TextureManager::Draw(this->bodyTexture, sourceRectangle, destinationRectangle, SDL_FLIP_NONE, angle);

        prev = current;
    }
}

void Snake::renderTail(){
    SDL_Rect sourceRectangle;
    sourceRectangle.x = 0;
    sourceRectangle.y = 0;
    sourceRectangle.w = 96;
    sourceRectangle.h = 96;

    SDL_Rect destinationRectangle;
    destinationRectangle.x = tail->coordinates->x * TILE_SIZE - TILE_SIZE;
    destinationRectangle.y = tail->coordinates->y * TILE_SIZE - TILE_SIZE;
    destinationRectangle.w = 3 * TILE_SIZE;
    destinationRectangle.h = 3 * TILE_SIZE;

    double angle = 0;

    switch(*tail->direction){
        case Direction::UP:
            angle = 0;
            destinationRectangle.y -= progress * TILE_SIZE / moveTime;
            break;
        case Direction::DOWN:
            angle = 180;
            destinationRectangle.y += progress * TILE_SIZE / moveTime;
            break;
        case Direction::LEFT:
            angle = 270;
            destinationRectangle.x -= progress * TILE_SIZE / moveTime;
            break;
        case Direction::RIGHT:
            angle = 90;
            destinationRectangle.x += progress * TILE_SIZE / moveTime;
            break;
    }

    TextureManager::Draw(this->tailTexture, sourceRectangle, destinationRectangle, SDL_FLIP_NONE, angle);
}

SDL_Point *Snake::getHeadCoordinates(){
    return head->coordinates;
}