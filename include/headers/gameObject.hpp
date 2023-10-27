#ifndef gameObject_hpp
#define gameObject_hpp

#include "game.hpp"

class GameObject{
    public:
        virtual void update();
        virtual void render();
};

#endif /* gameObject_hpp */