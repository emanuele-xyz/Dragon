#pragma once

#include <directxtk/SimpleMath.h>
using namespace DirectX::SimpleMath;

namespace Dragon::MathUtils
{
    /*
        x, y: mouse coordinates (screen space)
        returns: the ray direction
    */
    Vector3 GetRayFromMouse(int x, int y, const Viewport& viewport, const Matrix& view, const Matrix& projection);

    /*
        This function supposes that at least an intersection point exists!
        If it doesn't exist, the result is bogus.

        r: ray origin
        d: ray direction
        p: plane point
        n: plane normal
    */
    Vector3 IntersectRayPlane(Vector3 r, Vector3 d, Vector3 p, Vector3 n);
}
