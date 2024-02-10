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

struct SHADOW_PS_INPUT
{
    float4 pos : SV_POSITION;
};

void main(SHADOW_PS_INPUT input)
{
}

//float4 main(SHADOW_PS_INPUT input) : SV_TARGET
//{
//    return float4(0.5f, 0.0f, 0.0f, 1.0f);
//}