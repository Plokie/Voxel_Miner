//cbuffer c_buffer : register(b0)
//{
//    float4x4 proj;
//    float4x4 view;
//    float4 sunPos;
//}
//cbuffer cbuff : register(b0)
//{
//    float alpha = 1.0f;
//    float2 uvOffset = float2(0.f, 0.f);
//}

Texture2D tex : TEXTURE : register(t0);
SamplerState samplerState : SAMPLER : register(s0);

struct SHADOW_PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

void main(SHADOW_PS_INPUT input)
{
    float4 texCol = tex.Sample(samplerState, input.texCoord);
    clip(texCol.a - 0.2f);
    //if (texCol.a < 0.5f)
    //    discard;
}

//float4 main(SHADOW_PS_INPUT input) : SV_TARGET
//{
//    return float4(0.5f, 0.0f, 0.0f, 1.0f);
//}