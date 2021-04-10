#ifndef PLAYER_H
#define PLAYER_H

#include "Model.h"
#include "../Map.h"

class Map;

class Player : public Model
{
public:
    Player(Map* scene);
    void update(f32 speed);
    ~Player();

    Map* getMap();

    core::aabbox3df hitBox;
};

#endif
