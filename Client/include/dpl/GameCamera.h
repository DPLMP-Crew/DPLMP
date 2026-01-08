#pragma once
#include "dpl/Matrix.h"

class GameCamera {
public:
	Matrix m_Matrix;
	static GameCamera* GetInstance();
};