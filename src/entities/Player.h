#ifndef PLAYER_H
#define PLAYER_H

#include "Model.h"
#include "../Scene.h"

class Scene;

class Player : public Model
{
public:
    Player(Scene* scene);
    void update();
    ~Player();
};

#endif
