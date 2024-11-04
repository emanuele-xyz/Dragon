#include <Dragon/pch.h>
#include <Dragon/ImGuiEx.h>

namespace ImGuiEx
{
    bool ColorPicker3(const char* label, DirectX::SimpleMath::Vector3& col, ImGuiColorEditFlags flags)
    {
        float tmp[3]{ col.x, col.y, col.z };
        bool res{ ImGui::ColorPicker3(label, tmp, flags) };
        col.x = tmp[0];
        col.y = tmp[1];
        col.z = tmp[2];
        return res;
    }

    bool DragFloat3(const char* label, DirectX::SimpleMath::Vector3& v, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
    {
        float tmp[3]{ v.x, v.y, v.z };
        bool res{ ImGui::DragFloat3(label, tmp, v_speed, v_min, v_max, format, flags) };
        v.x = tmp[0];
        v.y = tmp[1];
        v.z = tmp[2];
        return res;
    }
}