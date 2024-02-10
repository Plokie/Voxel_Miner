cbuffer cbuff : register(b0)
{
    float alpha = 1.0f;
    float2 uvOffset = float2(0.f, 0.f);
}

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float2 texOffset : TEXOFFSET;
    float3 worldPos : WORLDPOS;
    
    float4 lightSpacePos : LIGHTSPACEPOS;
    float3 lightRay : LIGHTRAY;
    float3 view : VIEW;
};

Texture2D tex : TEXTURE : register(t0);
Texture2D lightTex : LIGHTTEXTURE : register(t1);
SamplerState samplerState : SAMPLER : register(s0);
SamplerState lightSamplerState : LIGHTSAMPLER : register(s1);

float4 main(PS_INPUT input) : SV_TARGET
{
    //debug
    
    float4 sunTexSample = lightTex.Sample(lightSamplerState, input.texCoord * 16.0f);
    return float4(sunTexSample.rgb, 1.0f);
    //
    
    
    float4 pixCol = tex.Sample(samplerState, input.texCoord + input.texOffset);
    if (pixCol.a < 0.5f)
        discard;
    
    //todo: make cbuffer input
    //float3 tempLightDir = normalize(-float3(-1.0f, -1.0f, 0.0f));
    float3 tempLightDir = normalize(-float3(0.0f, -1.0f, 0.0f));
    
    float lightDot = dot(normalize(input.normal), tempLightDir);
    lightDot = (lightDot + 1.0f) / 2.0f;
    lightDot += 0.25f;
    lightDot = saturate(lightDot);
    
    
    float4 col = float4(pixCol.rgb * lightDot, pixCol.a);
    //col = lerp(col, pixCol, 0.3f);
    //col += float4(ambientLight, 0.0f);
    col = saturate(col);

    //saturate(1.0f-input.pos.z) * 50.0f
    return float4(col.rgb, pixCol.a * alpha);
}