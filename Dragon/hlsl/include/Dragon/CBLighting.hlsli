// Abient and directional light constants

cbuffer CBLighting : register(b2)
{
    struct
    {
        // TODO: use float3 + padding
        float4 ambient_color;
        float4 light_color;
        float4 light_direction;
    } cb_lighting;
};
