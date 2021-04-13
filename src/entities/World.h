#ifndef WORLD_H
#define WORLD_H

#include <string>

#include "Model.h"
#include "../Map.h"

using namespace std;

class Map;

class World : public Model
{
public:
    World(Map* map, const string& name);
    ~World();
};

#endif
