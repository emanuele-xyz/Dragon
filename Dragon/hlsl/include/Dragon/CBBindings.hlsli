#include <Dragon/CBCamera.hlsli>
#include <Dragon/CBObject.hlsli>
#include <Dragon/CBLighting.hlsli>

cbuffer CBCamera : register(b0)
{
    CBCamera cb_camera;
};

cbuffer CBObject : register(b1)
{
    CBObject cb_object;
};

cbuffer CBLighting : register(b2)
{
    CBLighting cb_lighting;
};
