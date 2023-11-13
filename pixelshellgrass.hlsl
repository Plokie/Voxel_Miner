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
    float2 texOffset : TEXOFFSET;
    float3 worldPos : WORLDPOS;
};

Texture2D tex : TEXTURE : register(t0);
SamplerState samplerState : SAMPLER : register(s0);

float rand(in float2 uv)
{
    float2 noise = (frac(sin(dot(uv, float2(12.9898, 78.233) * 2.0)) * 43758.5453));
    return abs(noise.x + noise.y) * 0.5;
}

static const float atlas_size = 256.0f;
static const float tile_size = 16.0f;
static const float shell_pix_height = 2.0f;

float4 main(PS_INPUT input) : SV_TARGET
{
    float pixelSize = (tile_size / atlas_size);
    float4 pixCol = tex.Sample(samplerState, input.texCoord + input.texOffset);
    float2 normalUV = input.texCoord / pixelSize;
    
    float height = frac(input.worldPos.y);
    // relative height 0.0f = base, 1.0f = top
    float relHeight = (height / (pixelSize * (shell_pix_height + pixelSize)));
    float adjHeight = ((relHeight - 1.0f) / 0.8f) + 1.0f;
    
    float2 pixelIndex = floor(normalUV * 16.0f);
    float r = saturate(rand(pixelIndex + floor(input.worldPos.xz)) - adjHeight);
    if (r > 0.0f) {
        r = 1.0f;
    }
    else
        discard;
    
    //return float4(relHeight, relHeight, relHeight, 1.0f);
    return float4(pixCol.rgb * length(input.normal) * ((saturate(relHeight) * 0.5f) + 1.0f), r);
}