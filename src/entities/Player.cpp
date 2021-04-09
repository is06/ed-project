#include "Player.h"

Player::Player(Scene* scene) : Model(scene, "content/models/cube.obj")
{
    node = scene->getSceneManager()->addAnimatedMeshSceneNode(static_cast<scene::IAnimatedMesh*>(mesh));
    node->setMaterialFlag(video::EMF_LIGHTING, false);
}

void Player::update()
{
    Model::update();
}

Player::~Player()
{
    node->remove();
}
