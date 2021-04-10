#include "../Entity.h"
#include "../GameException.h"
#include "../Scene.h"
#include "Model.h"

Model::Model(Scene* scene, const string& name) : Entity(scene)
{
    mesh = scene->getSceneManager()->getMesh(name.c_str());

    if (mesh == nullptr) {
        throw GameException("Unable to load mesh '" + name + "'");
    }
}

void Model::update(f32 speed)
{
    Entity::update(speed);
}

Model::~Model()
{

}
