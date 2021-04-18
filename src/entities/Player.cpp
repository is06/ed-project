#include "Player.h"
#include "../Direction.h"
#include "../animators/PlatformPlayerAnimator.h"

Player::Player(Map* map)
    : Model(map, "content/models/player/kakashi_anim.b3d")
{
    node = scene->getSceneManager()->addAnimatedMeshSceneNode(static_cast<scene::IAnimatedMesh*>(mesh));
    node->setMaterialFlag(video::EMF_LIGHTING, false);
    node->setPosition(core::vector3df(0, 5.0f, 0));
}

void Player::update(f32 speed)
{
    Model::update(speed);

    if (scene->getController()->getPlayerXAxis() > 35) {
        goRight(speed * scene->getController()->getPlayerXAxis());
    } else if (scene->getController()->getPlayerXAxis() < -35) {
        goLeft(speed * scene->getController()->getPlayerXAxis());
    }
    if (scene->getController()->getPlayerYAxis() > 35) {
        goForward(speed * scene->getController()->getPlayerYAxis());
    } else if (scene->getController()->getPlayerYAxis() < -35) {
        goBackward(speed * scene->getController()->getPlayerYAxis());
    }

    if (scene->getController()->isActionPerformed(CONTROL_ACTION_JUMP, true)) {
        platformPlayerAnimator->jump();
    }
}

void Player::goLeft(f32 speed)
{
    updateCoords(Direction::left, speed);
}

void Player::goRight(f32 speed)
{
    updateCoords(Direction::right, speed);
}

void Player::goForward(f32 speed)
{
    updateCoords(Direction::top, speed);
}

void Player::goBackward(f32 speed)
{
    updateCoords(Direction::bottom, speed);
}

void Player::updateCoords(f32 angle, f32 speed)
{
    if (attachedCamera != nullptr) {
        f32 x = cos(core::degToRad(attachedCamera->getNode()->getRotation().Y) + angle);
        f32 z = sin(core::degToRad(attachedCamera->getNode()->getRotation().Y) + angle);

        node->setPosition(core::vector3df(
            node->getPosition().X + ((-x) * (speed / 32.0f)),
            node->getPosition().Y,
            node->getPosition().Z + (z * (speed / 32.0f))
        ));
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
            core::vector3df(0, 0, 0),
            core::vector3df(0, -0.6f, 0) // offset of origin point of sensor
        );
        node->addAnimator(collisionAnimator);
        
        selector->drop();
        collisionAnimator->drop();

        // Get animators from node and keep a pointer to make actions with it in update
        /*
        core::list<scene::ISceneNodeAnimator*>::ConstIterator it = node->getAnimators().begin();
        for (; it != node->getAnimators().end(); ++it) {
            animator = static_cast<scene::ISceneNodeAnimatorCollisionResponse*>((*it));
        }
        */
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

Player::~Player()
{
    node->remove();
}
