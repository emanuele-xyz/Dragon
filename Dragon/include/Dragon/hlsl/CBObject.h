struct CBObject
{
    matrix model;
    matrix normal;
    float3 color;
    float blend_factor; // 0 for albedo texture, 1 for full color
    int is_lit;
    int _pad0[3];
};
