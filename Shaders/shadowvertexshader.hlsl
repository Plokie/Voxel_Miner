cbuffer c_buffer : register(b0)
{
    float4x4 model;
    float4x4 view;
    float4x4 proj;
    float4x4 lightView;
    float4x4 lightProj;
    //float4 lightPos;
    //float4 eyePos;
    float time;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float2 texOffset : TEXOFFSET;
};

struct SHADOW_PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

SHADOW_PS_INPUT main(VS_INPUT input)
{
    SHADOW_PS_INPUT output;

    float4 pos = float4(input.pos, 1.0f);
    //float4 modelPos = mul(pos, model);
    pos = mul(pos, model);
    pos = mul(pos, view);
    pos = mul(pos, proj);
    output.pos = pos;
    
    output.texCoord = input.texCoord;
    
 
    return output;
}