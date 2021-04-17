#include "Player.h"

Player::Player(Map* map)
    : Model(map, "content/models/player/kakashi_anim.b3d")
{
    node = scene->getSceneManager()->addAnimatedMeshSceneNode(static_cast<scene::IAnimatedMesh*>(mesh));
    node->setMaterialFlag(video::EMF_LIGHTING, false);
    node->setMaterialFlag(video::EMF_WIREFRAME, true);
    node->setPosition(core::vector3df(0, 5.0f, 0));
}

void Player::update(f32 speed)
{
    Model::update(speed);

    if (scene->getController()->isActionPerformed(ACTION_JUMP, true)) {
        if (animator != nullptr) {
            animator->jump(0.35f);
        }
    }
}

void Player::attachWorldForCollisions(World* world)
{
    // Make a data representation of all world triangles
    auto selector = scene->getSceneManager()->createTriangleSelector(world->getMesh(), world->getNode());
    world->getNode()->setTriangleSelector(selector);

    if (selector != nullptr) {
        // This animator make collision detection and response possible between the player and the world
        auto collisionAnimator = scene->getSceneManager()->createCollisionResponseAnimator(
            selector,
            getNode(),
            core::vector3df(1, 1.2f, 1), // radius of ellipsoid collision sensor around the player
            getMap()->gravity,
            core::vector3df(0, -0.6f, 0) // offset of origin point of sensor
        );
        node->addAnimator(collisionAnimator);
        
        selector->drop();
        collisionAnimator->drop();

        // Get animators from node and keep a pointer to make actions with it in update
        core::list<scene::ISceneNodeAnimator*>::ConstIterator it = node->getAnimators().begin();
        for (; it != node->getAnimators().end(); ++it) {
            animator = static_cast<scene::ISceneNodeAnimatorCollisionResponse*>((*it));
        }
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
