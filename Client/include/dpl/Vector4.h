#pragma once

class Vector4 {
public:
	Vector4(void) {}
	Vector4(float a, float b, float c, float d)
	{
		X = a;
		Y = b;
		Z = c;
		W = d;
	}
	float X = 0.0f;
	float Y = 0.0f;
	float Z = 0.0f;
	float W = 0.0f;
};