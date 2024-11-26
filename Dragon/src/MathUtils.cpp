#include <Dragon/pch.h>
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

    /*
        Explanation is here: https://en.wikipedia.org/wiki/Slab_method
    */
    RayAABBIntersection IntersectRayAABB(Vector3 ray_origin, Vector3 ray_dir, Vector3 p_min, Vector3 p_max)
    {
        Vector3 t_low_i{ (p_min - ray_origin) / ray_dir };
        Vector3 t_high_i{ (p_max - ray_origin) / ray_dir };
        Vector3 t_close_i{ Vector3::Min(t_low_i, t_high_i) };
        Vector3 t_far_i{ Vector3::Max(t_low_i, t_high_i) };
        float t_close{ (std::max)(t_close_i.x, (std::max)(t_close_i.y, t_close_i.z)) };
        float t_far{ (std::min)(t_far_i.x, (std::min)(t_far_i.y, t_far_i.z)) };
        bool does_intersect{ t_close <= t_far };
        Vector3 p_close{ ray_origin + t_close * ray_dir };
        Vector3 p_far{ ray_origin + t_far * ray_dir };
        return { .does_intersect = does_intersect, .p_close = p_close, .p_far = p_far };
    }
}
