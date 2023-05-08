#pragma once
#include <d3d9.h>

typedef union Vector3 {
	struct {
		float x, y, z;
	};
	float data[3];
	friend Vector3 operator + (Vector3 const& lhs, Vector3 const& rhs);
	friend Vector3 operator - (Vector3 const& lhs, Vector3 const& rhs);
	friend Vector3 operator / (Vector3 const& lhs, int const& rhs);
	friend Vector3 operator * (Vector3 const& lhs, float const& rhs);
	Vector3& operator *= (float number);
	Vector3& operator -= (Vector3 const& rhs);
	Vector3& operator += (Vector3 const& rhs);
	friend std::ostream& operator<<(std::ostream& os, Vector3 const& pos);
};

typedef struct {
	float Matrix[3][4];
} Matrix3x4_t;