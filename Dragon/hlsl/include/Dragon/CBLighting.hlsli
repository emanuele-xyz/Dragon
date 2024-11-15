// Abient and directional light constants

struct CBLighting
{
    float3 light_color;
    float ambient_strength; // [0, 1]
    float3 light_direction;
    float _pad2;
};
