cbuffer cbuff : register(b0)
{
    float alpha = 1.0f;
    float2 uvOffset = float2(0.f, 0.f);
    float pad = 0.0f;
    //float4x4 lightMx;
    //float4x4 lightMxInv;
}

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float2 texOffset : TEXOFFSET;
    float3 worldPos : WORLDPOS;
    
    float4 lPos : LIGHTSPACEPOS;
};

Texture2D tex : TEXTURE : register(t0);
Texture2D lightTex : LIGHTTEXTURE : register(t1);
SamplerState samplerState : SAMPLER : register(s0);
SamplerState lightSamplerState : LIGHTSAMPLER : register(s1);

//SamplerComparisonState cmpSampler
//{
//    // sampler state
//    Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
//    AddressU = MIRROR;
//    AddressV = MIRROR;
 
//   // sampler comparison state
//    ComparisonFunc = LESS_EQUAL;
//};

//float2 shadowMapSize = float2(4096.0, 4096.0);

//float2 texOffset(int u, int v)
//{
//    return float2(u * (1.0f / shadowMapSize.x), v * (1.0f / shadowMapSize.y));
//}

float4 main(PS_INPUT input) : SV_TARGET
{
    float shadowFac = 1.0f;
    
    input.lPos.xyz /= input.lPos.w; //re-homogenize
    
    if (input.lPos.x < -1.0f || input.lPos.x > 1.0f ||
        input.lPos.y < -1.0f || input.lPos.y > 1.0f ||
        input.lPos.z < 0.0f || input.lPos.z > 1.0f)
    {
        //return float4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else {
        //float4 pixelRoundedWorldPos = floor(worldPos + float4(0.0001f, 0.0001f, 0.0001f, 0.0f)) + (floor(worldPos * 16.0) / 16.0);

        input.lPos.x = input.lPos.x / 2 + 0.5;
        input.lPos.y = input.lPos.y / -2 + 0.5;
        input.lPos.z -= 0.00001f;
        
        //input.lPos.xyz += input.normal * 0.00001f;
        
        //float pcfSum = 0;
        //float x, y;
        
        //for (y = -1.5; y <= 1.5; y += 1.0) {
        //    for (x = -1.5; x <= 1.5; x += 1.0) {
        //        pcfSum += lightTex.SampleCmpLevelZero(cmpSampler, input.lPos.xy + texOffset(x,y), input.lPos.z).r;
        //    }
        //}
        
        //float shadowMapSample = pcfSum / 16.0;
        
        //float shadowMapSample = lightTex.SampleCmp(cmpSampler, input.lPos.xy, input.lPos.z).r;
        float shadowMapSample = lightTex.Sample(lightSamplerState, input.lPos.xy).r;
    
    
        shadowFac = (shadowMapSample < input.lPos.z) ? 0.5f : 1.0f;
    }
    
    
    
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
    
    
    float4 col = float4(pixCol.rgb * lightDot * length(input.normal), pixCol.a);
    //col = lerp(col, pixCol, 0.3f);
    //col += float4(ambientLight, 0.0f);
    col *= shadowFac;
    
    col = saturate(col);
    

    //saturate(1.0f-input.pos.z) * 50.0f
    return float4(col.rgb, pixCol.a * alpha);
}