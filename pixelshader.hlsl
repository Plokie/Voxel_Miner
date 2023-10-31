cbuffer cbuff : register(b0)
{
    float alpha = 1.0f;
}

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    //float3 col : COLOR;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
};

Texture2D tex : TEXTURE : register(t0);
SamplerState samplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 pixCol = tex.Sample(samplerState, input.texCoord);
    
    //todo: make cbuffer input
    float3 tempLightDir = -float3(0.0f, -1.0f, 0.0f);
    
    float lightDot = dot(input.normal, tempLightDir);
    lightDot = (lightDot + 1.0f) / 2.0f;
    lightDot += 0.25f;
    lightDot = saturate(lightDot);
    
    
    float4 col = float4(pixCol.rgb * lightDot, pixCol.a);
    //col = lerp(col, pixCol, 0.3f);
    //col += float4(ambientLight, 0.0f);
    col = saturate(col);

    return float4(col.rgb, pixCol.a * alpha);
}