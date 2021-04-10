#include "Game.h"
#include "Scene.h"

Scene::Scene(Game* game)
{
    this->game = game;
    this->controller = game->getController();
    this->sceneManager = game->getSceneManager();
}

void Scene::update(f32 speed)
{
    
}

void Scene::draw()
{

}

scene::ISceneManager* Scene::getSceneManager()
{
    return sceneManager;
}

Scene::~Scene()
{

}
