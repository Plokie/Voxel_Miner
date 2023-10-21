struct VS_INPUT
{
    float3 pos : POSITION;
    float3 col : COLOR;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 col : COLOR;
};

VS_OUTPUT main(VS_INPUT input) {
    VS_OUTPUT output;
    output.pos = float4(input.pos, 1.0f);
    output.col = input.col;
    
    return output;
}