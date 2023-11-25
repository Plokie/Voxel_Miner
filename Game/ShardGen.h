#pragma once

#include <math.h>

#include "../Engine/MathUtil.h"

// From https://www.shadertoy.com/view/dlKyWw

#define tau 6.283185307179586f

Vector2 phash(Vector2 p) {
	p = Vector2(Vector2::dot(p, Vector2(127.1f, 311.7f)), Vector2::dot(p, Vector2(269.5f, 183.3f)));
	p = Vector2(
		frac(sin(p.x) * 43758.5453123f),
		frac(sin(p.y) * 43758.5453123f)
	);
	return p;
}

float fhash(Vector2 p)
{
    Vector3 p3 = Vector3(p.x,p.y,p.x) * 0.13;
    p3 = Vector3(frac(p3.x), frac(p3.y), frac(p3.z));
    float dot = Vector3::dot(p3, Vector3(p3.y, p3.z, p3.x) + Vector3(3.333f, 3.333f, 3.333f));
    p3 += Vector3(dot,dot,dot);
    return frac((p3.x + p3.y) * p3.z);
}

float shard_noise(const Vector2& p, const float sharpness) {
	Vector2 ip = Vector2(floor(p.x), floor(p.y));
	Vector2 fp = Vector2(frac(p.x), frac(p.y));

	float v = 0.f;
	float t = 0.f;
	for(int y = -1; y <= 1; y++) {
        for(int x = -1; x <= 1; x++) {
            Vector2 o = Vector2((float)x, (float)y);
            Vector2 io = ip + o;
            Vector2 h = phash(io);
            Vector2 r = fp - (o + h);
            
            float w = exp2f(-tau * Vector2::dot(r, r));
            float s = sharpness * Vector2::dot(r, phash(io + Vector2(27.f, 61.f)) - Vector2(0.5f,0.5f));

            v += w * s * (1.f / sqrtf(1.0f + s * s));
            t += w;
        }
	}
    return ((v / t) * 0.5f) + 0.5f;
}

float layered_shard_noise(Vector2 p, float sharpness = 10.0f, int octaves = 5) {
    float sample = 0.f;
    //
    //const float SHARPNESS = 4.0f;
    //const int OCTAVES = 5;
    
    for(int i = 0; i < octaves; i++) {
        sample += shard_noise(p * powf(2.f, static_cast<float>(i+2)), sharpness) * powf(2.f, -static_cast<float>(i + 1));
    }

    return sample;
}