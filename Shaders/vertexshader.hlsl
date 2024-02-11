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

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float2 texOffset : TEXOFFSET;
    float3 worldPos : WORLDPOS;
    
    float4 lPos : LIGHTSPACEPOS;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    //output.pos = float4(input.pos + float3(offsetX, offsetY, 0), 1.0f);
    output.worldPos = input.pos;
    
    float4 pos = float4(input.pos, 1.0f);
    float4 modelPos = mul(pos, model);
    pos = mul(modelPos, view);
    pos = mul(pos, proj);
    output.pos = pos;
   
    float4 lPos = mul(modelPos, lightView);
    output.lPos = mul(lPos, lightProj);
    
    output.normal = mul(float4(input.normal, 0.0f), model).xyz;
    output.texCoord = input.texCoord;
    output.texOffset = input.texOffset;
 
    return output;
}