#pragma once
#include "Vector3.h"
#include <cmath>

float Length(const Vector3& v) {
	Vector3 result{};
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}
// 正規化
Vector3 Nomalize(const Vector3& v) {
	Vector3 result{};
	result.x = v.x / Length(v);
	result.y = v.y / Length(v);
	result.z = v.z / Length(v);
	return result;
}
static Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result;

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;

	return result;
}

static Vector3 Multiply(const Vector3& v1,const float v2) { 
	Vector3 result;
	result.x = v1.x * v2;
	result.y = v1.y * v2;
	result.z = v1.z * v2;

	return result;



}


static Vector3 Subtract(const Vector3& v1, const Vector3& v2) { 
	Vector3 result;

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;

	return result;
}
