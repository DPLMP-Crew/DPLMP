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
	float X, Y, Z, W;
};