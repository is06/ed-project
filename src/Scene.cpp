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

Controller* Scene::getController()
{
    return controller;
}

void Scene::setDebugFlags(bool active)
{
    
}

Scene::~Scene()
{

}
