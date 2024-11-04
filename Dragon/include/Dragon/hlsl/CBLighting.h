// Abient and directional light constants

struct CBLighting
{
    float3 ambient_color;
    float _pad0;
    float3 light_color;
    float _pad1;
    float3 light_direction;
    float _pad2;
};
