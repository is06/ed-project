#ifndef MAP_ZONE_H
#define MAP_ZONE_H

#include <string>

#include "Model.h"
#include "../Map.h"

using namespace std;

class Map;

class MapZone : public Model
{
public:
    MapZone(Map* map, const string& name);
    ~MapZone();
};

#endif
