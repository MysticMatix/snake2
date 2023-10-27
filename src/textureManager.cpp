#include "textureManager.hpp"

SDL_Texture* TextureManager::LoadTexture(const char* fileName){
    SDL_Surface *tempSurface = IMG_Load(fileName);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return texture;
}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect sourceRectangle, SDL_Rect destinationRectangle, SDL_RendererFlip flip, double angle){
    SDL_RenderCopyEx(Game::renderer, texture, &sourceRectangle, &destinationRectangle, angle, NULL, flip);
}