#pragma once

class Vector {
public:
	Vector(void) {}
	Vector(float a, float b, float c)
	{
		X = a;
		Y = b;
		Z = c;
	}
	float X = 0.0f;
	float Y = 0.0f;
	float Z = 0.0f;
};