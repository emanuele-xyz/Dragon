#include <Dragon/VSOutput.hlsli>
#include <Dragon/CBBindings.hlsli>

Texture2D albedo_texture : register(t0);
SamplerState zampler : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 albedo;
    if (cb_object.blend_factor == 1.0) // full color
    {
        albedo = float4(cb_object.color, 1.0);
    }
    else // mix of albedo texture and full color
    {
        albedo = lerp(albedo_texture.Sample(zampler, input.uv), float4(cb_object.color, 1.0), cb_object.blend_factor);
    }
    
    float4 out_color;
    if (cb_object.is_lit)
    {
        float k_ambient = cb_lighting.ambient_strength;
        float k_directional = 0;
        {
            float3 normal = normalize(input.normal);
            float3 light_direction = normalize(cb_lighting.light_direction);
            k_directional = max(0.0, dot(normal, -light_direction));
        }
        // NOTE: Instead of doing k_ambient + k_directional, we take the maximum value between the two
        out_color = max(k_ambient, k_directional) * float4(cb_lighting.light_color, 1) * albedo;
    }
    else
    {
        out_color = albedo;
    }
    
    return out_color;
}
