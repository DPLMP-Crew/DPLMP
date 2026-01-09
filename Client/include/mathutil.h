#pragma once
#include <glm/glm.hpp>
#include "glm/ext/quaternion_float.hpp"
#include "dpl/Vector.h"
#include "dpl/Vector4.h"
#include "dpl/Matrix.h"

#define VEC3_RIGHT glm::vec3(1.0, 0.0, 0.0)
#define VEC3_UP glm::vec3(0.0, 1.0, 0.0)
#define VEC3_FORWARD glm::vec3(0.0, 0.0, 1.0)

inline glm::vec3 toGLM(const Vector& v) { return glm::vec3(v.X, v.Y, v.Z); }
inline glm::vec4 toGLM(const Vector4& v) { return glm::vec4(v.X, v.Y, v.Z, v.W); }

/// <summary>
/// Converts Hamster Row-Major Matrix to GLM Column-Major Matrix
/// </summary>
inline glm::mat4 toGLM(const Matrix& m) { return glm::mat4(toGLM(m.right), toGLM(m.up), toGLM(m.forward), toGLM(m.pos)); }

inline Vector fromGLM(const glm::vec3& v) { return Vector(v.x, v.y, v.z); }
inline Vector4 fromGLM(const glm::vec4& v) { return Vector4(v.x, v.y, v.z, v.w); }

/// <summary>
/// Converts GLM Column-Major Matrix to Hamster Row-Major Matrix
/// </summary>
inline Matrix fromGLM(const glm::mat4& m) {
	return Matrix(
		Vector4(m[0][0], m[0][1], m[0][2], m[0][3]),
		Vector4(m[1][0], m[1][1], m[1][2], m[1][3]),
		Vector4(m[2][0], m[2][1], m[2][2], m[2][3]),
		Vector4(m[3][0], m[3][1], m[3][2], m[3][3])
	);
}