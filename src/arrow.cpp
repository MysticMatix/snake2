#include "arrow.hpp"
#include "textureManager.hpp"

Arrow::Arrow(SDL_Texture* texture, int x, int y){
    this->texture = texture;
    this->x = x;
    this->y = y;
    this->angle = 0;
}

Arrow::~Arrow(){
    SDL_DestroyTexture(texture);
}

void Arrow::update(){
    this->angle += abs((this->angle - 150) * 0.07) + 0.01;
    if(this->angle > 360) this->angle = 0;
}

void Arrow::render(){
    SDL_Rect sourceRectangle;
    SDL_Rect destinationRectangle;

    sourceRectangle.x = 0;
    sourceRectangle.y = 0;
    sourceRectangle.w = 32;
    sourceRectangle.h = 32;

    destinationRectangle.x = this->x;
    destinationRectangle.y = this->y;
    destinationRectangle.w = WINDOW_WIDTH;
    destinationRectangle.h = WINDOW_HEIGHT;

    TextureManager::Draw(this->texture, sourceRectangle, destinationRectangle, SDL_FLIP_NONE, this->angle);
}

void Arrow::setAngle(double angle){
    this->angle = angle;
}

double Arrow::getAngle(){
    return this->angle;
}
