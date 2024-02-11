cbuffer c_buffer : register(b0)
{
    float4x4 model;
    float4x4 view;
    float4x4 proj;
    float4x4 lightView;
    float4x4 lightProj;
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



float hash(float2 p)
{
    float3 p3 = frac(float3(p.xyx) * 0.13);
    p3 += dot(p3, p3.yzx + 3.333);
    return frac((p3.x + p3.y) * p3.z);
}
float noise(float2 x)
{
    float2 i = floor(x);
    float2 f = frac(x);

	// Four corners in 2D of a tile
    float a = hash(i);
    float b = hash(i + float2(1.0, 0.0));
    float c = hash(i + float2(0.0, 1.0));
    float d = hash(i + float2(1.0, 1.0));

    // Simple 2D lerp using smoothstep envelope between the values.
	// return vec3(mix(mix(a, b, smoothstep(0.0, 1.0, f.x)),
	//			mix(c, d, smoothstep(0.0, 1.0, f.x)),
	//			smoothstep(0.0, 1.0, f.y)));

	// Same code, with the clamps in smoothstep and common subexpressions
	// optimized away.
    float2 u = f * f * (3.0 - 2.0 * f);
    return lerp(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}
static const int NUM_NOISE_OCTAVES = 3;
float fbm(float2 x)
{
    float v = 0.0;
    float a = 0.5;
    float2 shift = float2(100, 100);

    for (int i = 0; i < NUM_NOISE_OCTAVES; ++i)
    {
        v += a * noise(x);
        x = x * 2.0 + shift;
        a *= 0.5;
    }
    return v;
}

static const float atlas_size = 256.0f;
static const float tile_size = 16.0f;
static const float shell_pix_height = 2.0f;

static const float WindFrequency = 2.0f;
static const float WindSpeed = 0.3f;
static const float WindAmplitude = 0.1875f;

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    //output.pos = float4(input.pos + float3(offsetX, offsetY, 0), 1.0f);
    
    float4 modelPos = mul(float4(input.pos, 1.0f), model);
    float3 modelMxPos = modelPos.xyz;
    float height = frac(input.pos.y);
    // relative height 0.0f = base, 1.0f = top
    float relHeight = (height / ((tile_size / atlas_size) * shell_pix_height));
    
    float noiseSample01 = fbm((modelMxPos.xz * WindFrequency) + (float2(time, time) * WindSpeed));
    float noiseSample11 = (noiseSample01 - 0.5f) * 2.0f;
    
    input.pos.x -= noiseSample11 * WindAmplitude * relHeight;
    
    
    output.worldPos = input.pos;
    float4 pos = float4(input.pos, 1.0f);
    
    pos = mul(pos, model);
    pos = mul(pos, view);
    pos = mul(pos, proj);
    
    output.pos = pos;
    //output.col = input.col;
    output.texCoord = input.texCoord;
    output.texOffset = input.texOffset;
    
    output.lPos = mul(modelPos, lightView);
    output.lPos = mul(output.lPos, lightProj);
    
    //output.normal = -input.normal;
    
    output.normal = mul(float4(input.normal, 0.0f), model).xyz;
    //output.normal = input.normal;
    
    return output;
}