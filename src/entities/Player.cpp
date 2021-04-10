#include "Player.h"

Player::Player(Map* map)
    : Model(map, "content/models/player/player.obj")
{
    node = scene->getSceneManager()->addAnimatedMeshSceneNode(static_cast<scene::IAnimatedMesh*>(mesh));
    node->setMaterialFlag(video::EMF_LIGHTING, false);
    node->setMaterialFlag(video::EMF_WIREFRAME, true);
    node->setPosition(core::vector3df(0, 5.0f, 0));

    hitBox = core::aabbox3df(
        core::vector3df(0, 0, 0),
        core::vector3df(0, 0, 0)
    );
}

void Player::update(f32 speed)
{
    Model::update(speed);

    if (scene->getController()->isActionPerformed(ACTION_JUMP)) {
        
    }
}

Map* Player::getMap()
{
    return static_cast<Map*>(scene);
}

Player::~Player()
{
    node->remove();
}
