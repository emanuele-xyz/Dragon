#include <Dragon/VSOutput.hlsli>

#include <Dragon/ConstantBuffers.hlsli>

Texture2D albedo : register(t0);
SamplerState zampler : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float3 ambient = cb_lighting.ambient_color;
    float3 directional;
    {
        float3 normal = normalize(input.normal);
        float3 light_direction = normalize(cb_lighting.light_direction);
        float k = max(0.0, dot(normal, -light_direction));
        directional = k * cb_lighting.light_color;
    }
    
    return float4(ambient + directional, 1.0) * albedo.Sample(zampler, input.uv);
}
