#pragma once
#include "dpl/Vector4.h"

class Matrix {
public:
	union {
		struct {
			Vector4 right;
			Vector4 up;
			Vector4 forward;
			Vector4 pos;
		};
		struct
		{
			float M[4][4];
		};
	};
	Matrix();
	Matrix(Vector4 setright, Vector4 setup, Vector4 setforward, Vector4 setpos) {
		right = setright;
		up = setup;
		forward = setforward;
		pos = setpos;
	}
};