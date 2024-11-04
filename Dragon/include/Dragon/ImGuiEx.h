#pragma once

#include <imgui.h>
#include <directxtk/SimpleMath.h>

namespace ImGuiEx
{
    bool ColorPicker3(const char* label, DirectX::SimpleMath::Vector3& col, ImGuiColorEditFlags flags = 0);
    bool DragFloat3(const char* label, DirectX::SimpleMath::Vector3& v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
}
