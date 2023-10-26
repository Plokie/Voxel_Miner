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
    
    float lightDot = dot(input.normal, float3(0.0f, -1.0f, 0.0f));
    lightDot = (lightDot + 1.0f) / 2.0f;
    lightDot = saturate(lightDot);
    
    //todo: make cbuffer input
    float3 ambientLight = (0.2f, 0.2f, 0.2f);
    
    
    float4 col = float4(pixCol.rgb * lightDot, pixCol.a);
    //col = lerp(col, pixCol, 0.3f);
    col = saturate(col);
    //col.a = pixCol.a;
    col.r = lerp(col.r, pixCol.r, ambientLight.r);
    col.g = lerp(col.g, pixCol.g, ambientLight.g);
    col.b = lerp(col.b, pixCol.b, ambientLight.b);
    
    return float4(col.rgb, pixCol.a * alpha);
}