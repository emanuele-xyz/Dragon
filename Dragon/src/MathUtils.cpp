#include <Dragon/pch.h>
#include <Dragon/MathUtils.h>
#include <Dragon/MathUtils.h>

namespace Dragon::MathUtils
{
    Vector3 GetRayFromMouse(int x, int y, const Viewport& viewport, const Matrix& view, const Matrix& projection)
    {
        Vector3 i{ static_cast<float>(x), static_cast<float>(y), 0.0f };
        Vector3 a{ viewport.Unproject(i, projection, view, Matrix::Identity) };
        Vector3 j{ static_cast<float>(x), static_cast<float>(y), 1.0f };
        Vector3 b{ viewport.Unproject(j, projection, view, Matrix::Identity) };
        Vector3 from_a_to_b{ b - a };
        from_a_to_b.Normalize();
        return from_a_to_b;
    }

    Vector3 IntersectRayPlane(Vector3 r, Vector3 d, Vector3 p, Vector3 n)
    {
        float t{ n.Dot(p - r) / n.Dot(d) };
        return r + t * d;
    }
}
