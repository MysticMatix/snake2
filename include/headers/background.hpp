#ifndef background_hpp
#define background_hpp

#include "gameObject.hpp"

class Background : public GameObject{
    public:
        Background(SDL_Texture* texture);
        Background(const char* fileName);
        ~Background();

        void update();
        void render();

    private:
        SDL_Texture *texture;
};

#endif /* background_hpp */