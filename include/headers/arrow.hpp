#ifndef arrow_hpp
#define arrow_hpp

#include "gameObject.hpp"

class Arrow : public GameObject{
    public:
        Arrow(SDL_Texture* texture, int x, int y);
        ~Arrow();

        void update();
        void render();

        void setAngle(double angle);
        double getAngle();
    private:
        SDL_Texture *texture;
        double angle;
        int x;
        int y;
};

#endif /* arrow_hpp */