#ifndef textureManager_hpp
#define textureManager_hpp

#include "game.hpp"

class TextureManager{
    public:
        static SDL_Texture* LoadTexture(const char* fileName);
        static void Draw(SDL_Texture* texture, SDL_Rect sourceRectangle, SDL_Rect destinationRectangle, SDL_RendererFlip flip, double angle);
};

#endif /* textureManager_hpp */