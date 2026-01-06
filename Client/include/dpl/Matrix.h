#pragma once
#include "dpl/Vector.h"

class Matrix {
public:
	// Why
	union {
		struct {
			Vector right;
			float pad;
			Vector up;
			float _pad;
			Vector forward;
			float __pad;
			Vector pos;
			float ___pad;
		};
		struct
		{
			float M[4][4];
		};
	};
};