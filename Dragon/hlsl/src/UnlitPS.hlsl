#include <Dragon/VSOutput.hlsli>

Texture2D albedo : register(t0);
SamplerState zampler : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    return albedo.Sample(zampler, input.uv);
}
