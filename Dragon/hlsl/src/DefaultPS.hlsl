#include <Dragon/VSOutput.hlsli>

#include <Dragon/ConstantBuffers.hlsli>

Texture2D albedo_texture : register(t0);
SamplerState zampler : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 out_color;
    float4 albedo = albedo_texture.Sample(zampler, input.uv);
    
    if (cb_object.is_lit)
    {
        float3 ambient = cb_lighting.ambient_color;
        float3 directional;
        {
            float3 normal = normalize(input.normal);
            float3 light_direction = normalize(cb_lighting.light_direction);
            float k = max(0.0, dot(normal, -light_direction));
            directional = k * cb_lighting.light_color;
        }
        out_color = float4(ambient + directional, 1.0) * albedo;
    }
    else
    {
        out_color = albedo;
    }
    
    return out_color;
}
