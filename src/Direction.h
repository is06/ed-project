#ifndef DIRECTION_H
#define DIRECTION_H

#include <irrlicht/irrlicht.h>

using namespace irr;

namespace Direction
{
    static const f32 right = 0.0f;
    static const f32 topRight = core::PI / 4.0f;
    static const f32 top = core::PI / 2.0f;
    static const f32 topLeft = 3.0f * (core::PI / 4.0f);
    static const f32 left = core::PI;
    static const f32 bottomLeft = 5.0f * (core::PI / 4.0f);
    static const f32 bottom = 3.0f * (core::PI / 2.0f);
    static const f32 bottomRight = 7.0f * (core::PI / 4.0f);
}

#endif
