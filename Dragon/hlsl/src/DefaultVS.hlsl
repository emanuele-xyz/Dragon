#include <Dragon/VSInput.hlsli>
#include <Dragon/VSOutput.hlsli>
#include <Dragon/CBBindings.hlsli>

VSOutput main(VSInput input)
{
    VSOutput output;
    output.position = mul(cb_camera.projection, mul(cb_camera.view, mul(cb_object.model, float4(input.position, 1.0))));
    output.normal = mul(cb_object.normal, float4(input.normal, 0.0)).xyz;
    output.uv = input.uv;
    return output;
}
