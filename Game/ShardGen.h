#pragma once

#include <math.h>

#include "../Engine/MathUtil.h"
#include "FastNoiseLite.h"

// From https://www.shadertoy.com/view/dlKyWw

#define tau 6.283185307179586f

//Vector2 phash(Vector2 p) {
//	p = Vector2(Vector2::dot(p, Vector2(127.1f, 311.7f)), Vector2::dot(p, Vector2(269.5f, 183.3f)));
//	p = Vector2(
//		frac(sin(p.x) * 43758.5453123f),
//		frac(sin(p.y) * 43758.5453123f)
//	);
//	return p;
//}
//
//float fhash(Vector2 p)
//{
//    Vector3 p3 = Vector3(p.x,p.y,p.x) * 0.13;
//    p3 = Vector3(frac(p3.x), frac(p3.y), frac(p3.z));
//    float dot = Vector3::dot(p3, Vector3(p3.y, p3.z, p3.x) + Vector3(3.333f, 3.333f, 3.333f));
//    p3 += Vector3(dot,dot,dot);
//    return frac((p3.x + p3.y) * p3.z);
//}

Vector2 vec2fnl(const Vector2& p, FastNoiseLite& fnl0, FastNoiseLite& fnl1) {
    //return Vector2( (fnl0.GetNoise(p.x, p.y) + 1.f) / 2.f, (fnl1.GetNoise(p.x, p.y)+1.f) / 2.f);
    return Vector2( abs(fnl0.GetNoise(p.x, p.y)), abs(fnl1.GetNoise(p.x, p.y)));
}

inline float shardFrac(float x) {
    return x - floor(x);
}


float shard_noise(const Vector2& p, const float sharpness, FastNoiseLite& fnl0, FastNoiseLite& fnl1, const float verticality = 0.0f) {
    Vector2 ip = Vector2(floor(p.x), floor(p.y));
	Vector2 fp = Vector2(shardFrac(p.x), shardFrac(p.y));

    // if u set this v value to 1 it makes pretty cool mountains
	float v = verticality;
	float t = 0.f;
	for(int y = -1; y <= 1; y++) {
        for(int x = -1; x <= 1; x++) {
            Vector2 o = Vector2(static_cast<float>(x), static_cast<float>(y));
            Vector2 io = ip + o;
            //Vector2 h = phash(io);
            Vector2 h = vec2fnl(io, fnl0, fnl1);
            Vector2 r = fp - (o + h);
            
            float w = exp2f(-tau * Vector2::dot(r, r));
            //float s = sharpness * Vector2::dot(r, phash(io + Vector2(27.f, 61.f)) - Vector2(0.5f,0.5f));
            float s = sharpness * Vector2::dot(r, vec2fnl(io + Vector2(27.f, 61.f), fnl0, fnl1) - Vector2(0.5f,0.5f));
            //float s = sharpness * Vector2::dot(r, phash(io + Vector2(14521.f, 2141.f)) - Vector2(0.5f,0.5f));

            v += w * s * (1.f / sqrtf(1.0f + s * s));
            t += w;
        }
	}
    return ((v / t) * 0.5f) + 0.5f;
}

float layered_shard_noise(Vector2 p, FastNoiseLite& fnl0, FastNoiseLite& fnl1, float verticality = 0.0f, float sharpness = 10.0f, int octaves = 5) {
    float sample = 0.f;
    //
    //const float SHARPNESS = 4.0f;
    //const int OCTAVES = 5;
    
    for(int i = 0; i < octaves; i++) {
        sample += shard_noise(p * powf(2.f, static_cast<float>(i+2)), sharpness, fnl0, fnl1, verticality) * powf(2.f, -static_cast<float>(i + 1));
    }

    return sample;
}