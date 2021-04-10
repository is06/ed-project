#include "Player.h"

Player::Player(Scene* scene)
    : Model(scene, "content/models/cube.obj")
{
    node = scene->getSceneManager()->addAnimatedMeshSceneNode(static_cast<scene::IAnimatedMesh*>(mesh));
    node->setMaterialFlag(video::EMF_LIGHTING, false);
}

void Player::update(f32 speed)
{
    Model::update(speed);
}

Scene* Player::getScene()
{
    return scene;
}

Player::~Player()
{
    node->remove();
}
