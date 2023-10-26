cbuffer c_buffer : register(b0)
{
    float4x4 mx;
    float4x4 modelMx;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
};

VS_OUTPUT main(VS_INPUT input) {
    VS_OUTPUT output;
    //output.pos = float4(input.pos + float3(offsetX, offsetY, 0), 1.0f);
    output.pos = mul(float4(input.pos, 1.0f), mx);
    //output.col = input.col;
    output.texCoord = input.texCoord;
    
    //output.normal = -input.normal;
    
    output.normal = mul(float4(-input.normal, 0.0f), modelMx).xyz;
    //output.normal = input.normal;
    
    return output;
}