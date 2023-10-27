#include "game.hpp"

#include <iostream>
#include <list>

#include "textureManager.hpp"
#include "arrow.hpp"
#include "background.hpp"
#include "food.hpp"
#include "snake.hpp"

const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 768;
const int BOARD_WIDTH = 15;
const int BOARD_HEIGHT = 12;
const int TILE_SIZE = 64;
const int FPS = 30;

std::list<GameObject*> gameObjects;

Snake* snake1 = nullptr;
Food* food1 = nullptr;

Snake* snake2 = nullptr;
Food* food2 = nullptr;

Game::Game(){
    this->isRunning = false;
    this->state = GameState::PAUSE;
    window = nullptr;
    renderer = nullptr;
    gameBoard = nullptr;
    foodTurn = false;
    score = 0;
}

Game::~Game(){
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen){
    srand(time(NULL));

    int flags = 0;
    if(fullscreen){
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if(SDL_Init(SDL_INIT_EVERYTHING) == 0){
        std::cout << "Subsystems initialized..." << std::endl;

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if(window){
            std::cout << "Window created..." << std::endl;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if(renderer){
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout << "Renderer created..." << std::endl;
        }

        gameBoard = new TileState[BOARD_WIDTH * BOARD_HEIGHT];
        for(int i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; i++){
            gameBoard[i] = TileState::EMPTY;
        }

        gameObjects.push_back(new Background(TextureManager::LoadTexture("assets/background.png")));

        food1 = new Food(TextureManager::LoadTexture("assets/food1.png"), 0, 0, gameBoard);
        gameObjects.push_back(food1);

        food2 = new Food(TextureManager::LoadTexture("assets/food2.png"), 0, 0, gameBoard);
        gameObjects.push_back(food2);

        snake1 = new Snake("assets/head1.png", "assets/body1.png", "assets/tail1.png", 3, 3, new Direction(Direction::DOWN), 0, gameBoard);
        gameObjects.push_back(snake1);

        snake2 = new Snake("assets/head2.png", "assets/body2.png", "assets/tail2.png", 11, 3, new Direction(Direction::DOWN), 0, gameBoard);
        gameObjects.push_back(snake2);

        score = 0;
        foodTurn = false;
        isRunning = true;
    }else{
        isRunning = false;
    }
}

void Game::handleEvents(){
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type){
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym){
                case SDLK_UP:
                    snake2->tryChangeHeadDir(new Direction(Direction::UP));
                    break;
                case SDLK_DOWN:
                    snake2->tryChangeHeadDir(new Direction(Direction::DOWN));
                    break;
                case SDLK_LEFT:
                    snake2->tryChangeHeadDir(new Direction(Direction::LEFT));
                    break;
                case SDLK_RIGHT:
                    snake2->tryChangeHeadDir(new Direction(Direction::RIGHT));
                    break;
                case SDLK_w:
                    snake1->tryChangeHeadDir(new Direction(Direction::UP));
                    break;
                case SDLK_s:
                    snake1->tryChangeHeadDir(new Direction(Direction::DOWN));
                    break;
                case SDLK_a:
                    snake1->tryChangeHeadDir(new Direction(Direction::LEFT));
                    break;
                case SDLK_d:
                    snake1->tryChangeHeadDir(new Direction(Direction::RIGHT));
                    break;
                case SDLK_SPACE:
                    if(state == GameState::FINISH){
                        restart();
                    }
                    else if(state == GameState::PLAY){
                        state = GameState::PAUSE;
                    }
                    else if(state == GameState::PAUSE){
                        state = GameState::PLAY;
                    }
                    break;
                }
                break;
        default:
            break;
    }
}

void Game::update(){
    SDL_SetWindowTitle(window, ("Score: " + std::to_string(score)).c_str());

    if(state != GameState::PLAY) return;
    for(auto gameObject : gameObjects){
        gameObject->update();
    }

    SDL_Point* headCoordinates = snake1->getHeadCoordinates();
    
    if(headCoordinates->x < 0 || headCoordinates->x >= BOARD_WIDTH || headCoordinates->y < 0 || headCoordinates->y >= BOARD_HEIGHT){
        state = GameState::FINISH;
        SDL_SetWindowTitle(window, "Game Over");
        return;
    }

    if(gameBoard[headCoordinates->y * BOARD_WIDTH + headCoordinates->x] == TileState::SNAKE){
        state = GameState::FINISH;
        SDL_SetWindowTitle(window, "Game Over (hit snake)");
        return;
    }

    if(headCoordinates->x == food1->getX() && headCoordinates->y == food1->getY() && food1->isActive()){
        snake1->eat();
        food1->despawn();
        score++;
        foodTurn = !foodTurn;
    }

    if(headCoordinates->x == snake2->getHeadCoordinates()->x && headCoordinates->y == snake2->getHeadCoordinates()->y){
        state = GameState::FINISH;
        SDL_SetWindowTitle(window, "Game Over (hit snake)");
        return;
    }

    headCoordinates = snake2->getHeadCoordinates();

    if(headCoordinates->x < 0 || headCoordinates->x >= BOARD_WIDTH || headCoordinates->y < 0 || headCoordinates->y >= BOARD_HEIGHT){
        state = GameState::FINISH;
        SDL_SetWindowTitle(window, "Game Over");
        return;
    }

    if(gameBoard[headCoordinates->y * BOARD_WIDTH + headCoordinates->x] == TileState::SNAKE){
        state = GameState::FINISH;
        SDL_SetWindowTitle(window, "Game Over (hit snake)");
        return;
    }

    if(headCoordinates->x == food2->getX() && headCoordinates->y == food2->getY() && food2->isActive()){
        snake2->eat();
        food2->despawn();
        score++;
        foodTurn = !foodTurn;
    }

    

    if(foodTurn){
        if(!food2->isActive()){
            food2->spawn();
        }
    }
    else{
        if(!food1->isActive()){
            food1->spawn();
        }
    }
}

void Game::render(){
    SDL_RenderClear(renderer);
    for(auto gameObject : gameObjects){
        gameObject->render();
    }
    SDL_RenderPresent(renderer);
}

void Game::clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned..." << std::endl;
}

bool Game::running(){
    return isRunning;
}

void Game::restart(){
    for(int i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; i++){
        gameBoard[i] = TileState::EMPTY;
    }

    for(auto object : gameObjects){
        delete object;
    }

    gameObjects.clear();

    gameObjects.push_back(new Background(TextureManager::LoadTexture("assets/background.png")));

    food1 = new Food(TextureManager::LoadTexture("assets/food1.png"), 0, 0, gameBoard);
    gameObjects.push_back(food1);

    food2 = new Food(TextureManager::LoadTexture("assets/food2.png"), 0, 0, gameBoard);
    gameObjects.push_back(food2);

    snake1 = new Snake("assets/head1.png", "assets/body1.png", "assets/tail1.png", 3, 3, new Direction(Direction::DOWN), 0, gameBoard);
    gameObjects.push_back(snake1);

    snake2 = new Snake("assets/head2.png", "assets/body2.png", "assets/tail2.png", 11, 3, new Direction(Direction::DOWN), 0, gameBoard);
    gameObjects.push_back(snake2);

    score = 0;
    foodTurn = false;
    isRunning = true;

    state = GameState::PAUSE;
}