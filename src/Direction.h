#ifndef DIRECTION_H
#define DIRECTION_H

#include <irrlicht/irrlicht.h>

using namespace irr;

namespace Direction
{
    static const f32 right = 0.0f;
    static const f32 upRight = core::PI / 4.0f;
    static const f32 up = core::PI / 2.0f;
    static const f32 upLeft = 3.0f * (core::PI / 4.0f);
    static const f32 left = core::PI;
    static const f32 downLeft = 5.0f * (core::PI / 4.0f);
    static const f32 down = 3.0f * (core::PI / 2.0f);
    static const f32 downRight = 7.0f * (core::PI / 4.0f);
}

#endif
