#ifndef MODEL_H
#define MODEL_H

#include <irrlicht/irrlicht.h>
#include <string>

#include "../Entity.h"
#include "../Scene.h"

using namespace irr;
using namespace std;

class Scene;

class Model : public Entity
{
public:
    Model(Scene* scene, const string& name);
    virtual void update();
    virtual ~Model();

protected:
    scene::IMesh* mesh;
};

#endif