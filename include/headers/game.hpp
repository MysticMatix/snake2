#ifndef game_hpp
#define game_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
extern const int BOARD_WIDTH;
extern const int BOARD_HEIGHT;
extern const int TILE_SIZE;
extern const int FPS;

enum class GameState {
    PAUSE,
    PLAY,
    FINISH,
    EXIT
};

enum class TileState {
    EMPTY,
    SNAKE,
    FOOD
};

class Game{
    public:
        Game();
        ~Game();

        void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

        void handleEvents();
        void update();
        void render();
        void clean();

        bool running();

        void restart();

        inline static SDL_Renderer *renderer;
    private:
        bool isRunning;
        SDL_Window *window;
        GameState state;
        TileState* gameBoard;

        bool foodTurn;
        int score;

};


#endif /* game_hpp */