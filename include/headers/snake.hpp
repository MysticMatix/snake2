#ifndef snake_hpp
#define snake_hpp

#include "game.hpp"
#include "gameObject.hpp"

#include <list>

extern const int moveTime;

enum class Direction{
    UP = 0,
    LEFT = 270,
    RIGHT = 90,
    DOWN = 180
};

class SnakeTile{
    public:
        SDL_Point* coordinates;
        Direction* direction;
};

class SnakeHead{
    public:
        SDL_Point* coordinates;
        double angle;
        Direction* originalDirection;
        Direction* direction;
        bool directionChanged;
        bool showNeck;
};

class SnakeTail{
    public:
        SDL_Point* coordinates;
        Direction* direction;
};

class Snake: public GameObject{
    public:
        Snake(SDL_Texture* headTexture, SDL_Texture* bodyTexture, SDL_Texture* tailTexture, int x, int y, Direction* dir, int length, TileState* gameboard);
        Snake(const char* headTexture, const char* bodyTexture, const char* tailTexture, int x, int y, Direction* dir, int length, TileState* gameboard);
        ~Snake();

        void update();
        void render();

        void tryChangeHeadDir(Direction* dir);

        void eat();

        SDL_Point* getHeadCoordinates();
    private:
        SDL_Texture *headTexture, *bodyTexture, *tailTexture;

        SnakeHead* head;
        std::list<SnakeTile*> tiles;
        SnakeTail* tail;

        TileState* gameboard;

        int progress;
        
        bool eating;

        void move();

        void renderHead();
        void renderBody();
        void renderTail();
};

#endif