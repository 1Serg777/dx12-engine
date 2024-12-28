struct VSInput
{
    float3 position : POSITION;
    float3 color    : COLOR;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color    : COLOR;
};

PSInput VSMain(VSInput input)
{
    PSInput result;

    result.position = float4(input.position, 1.0);
    result.color = float4(input.color, 1.0);

    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return input.color;
}