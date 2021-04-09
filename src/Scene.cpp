#include "Game.h"
#include "Scene.h"

Scene::Scene(Game* game)
{
    this->game = game;
    this->sceneManager = game->getSceneManager();
}

void Scene::update()
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
