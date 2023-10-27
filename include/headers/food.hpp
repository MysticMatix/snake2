#ifndef food_hpp
#define food_hpp

#include "game.hpp"
#include "gameObject.hpp"

class Food: public GameObject{
    public:
        Food(SDL_Texture* textureSheet, int x, int y, TileState* gameboard);
        Food(const char* textureSheet, int x, int y, TileState* gameboard);
        ~Food();

        void update();
        void render();

        bool isActive();
        void spawn();
        void despawn();

        int getX();
        int getY();
    private:
        SDL_Texture* texture;
        SDL_Rect srcRect, destRect;

        TileState* gameboard;

        bool active;

        int x, y;
};

#endif