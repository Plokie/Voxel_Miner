cbuffer alpha_buffer : register(b0)
{
    float alpha = 1.0f;
}

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    //float3 col : COLOR;
    float2 texCoord : TEXCOORD;
};

Texture2D tex : TEXTURE : register(t0);
SamplerState samplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 pixCol = tex.Sample(samplerState, input.texCoord);
    return float4(pixCol.b, pixCol.g, pixCol.r, pixCol.a * alpha);
    
    //return float4(pixCol.rgb, pixCol.a * alpha);
}