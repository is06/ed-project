#include "Player.h"
#include "../Direction.h"
#include "../animators/PlatformPlayerAnimator.h"

Player::Player(Map* map)
    : Model(map, "content/models/player/player_cube.b3d")
{
    node = scene->getSceneManager()->addAnimatedMeshSceneNode(static_cast<scene::IAnimatedMesh*>(mesh));
    node->setMaterialFlag(video::EMF_LIGHTING, false);
    node->setPosition(core::vector3df(0, 5.0f, 0));
}

void Player::update(f32 speed)
{
    Model::update(speed);

    if (scene->getController()->getPlayerXAxis() > 35) {
        platformPlayerAnimator->goRight(speed * scene->getController()->getPlayerXAxis());
    } else if (scene->getController()->getPlayerXAxis() < -35) {
        platformPlayerAnimator->goLeft(speed * scene->getController()->getPlayerXAxis());
    }
    if (scene->getController()->getPlayerYAxis() > 35) {
        platformPlayerAnimator->goForward(speed * scene->getController()->getPlayerYAxis());
    } else if (scene->getController()->getPlayerYAxis() < -35) {
        platformPlayerAnimator->goBackward(speed * scene->getController()->getPlayerYAxis());
    }

    if (scene->getController()->isActionPerformed(CONTROL_ACTION_JUMP, true)) {
        platformPlayerAnimator->jump();
    }
}

void Player::attachWorldForCollisions(World* world)
{
    // Make a data representation of all world triangles
    auto selector = scene->getSceneManager()->createTriangleSelector(world->getMesh(), world->getNode());
    world->getNode()->setTriangleSelector(selector);

    if (selector != nullptr) {
        platformPlayerAnimator = new PlatformPlayerAnimator(this);
        node->addAnimator(platformPlayerAnimator);
        
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
    }
}

void Player::attachThirdPersonCamera(TpCamera* camera)
{
    attachedCamera = camera;
}

Map* Player::getMap()
{
    return static_cast<Map*>(scene);
}

Camera* Player::getAttachedCamera()
{
    return attachedCamera;
}

Player::~Player()
{
    node->remove();
}
