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
	float X, Y, Z;
};