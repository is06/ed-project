#ifndef STATIC_MODEL_H
#define STATIC_MODEL_H

#include "../Model.h"
#include "../../Map.h"

class Map;
class Model;

class StaticModel : public Model
{
public:
    StaticModel(Map* map, const string& modelName);
    void update(f32 speed);
    ~StaticModel();
};

#endif
