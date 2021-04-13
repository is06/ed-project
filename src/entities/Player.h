#ifndef PLAYER_H
#define PLAYER_H

#include "Model.h"
#include "World.h"
#include "../Map.h"

class Map;
class World;

class Player : public Model
{
public:
    Player(Map* scene);
    void update(f32 speed);
    ~Player();

    void attachWorldForCollisions(World* world);
    Map* getMap();

private:
    scene::ISceneNodeAnimatorCollisionResponse* animator;
};

#endif
