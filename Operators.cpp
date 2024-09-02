#include "Matrix4x4.h"
#include "Vector3.h"

// デフォルトコンストラクタ: 0で初期化
Matrix4x4::Matrix4x4() {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			m[i][j] = 0.0f;
		}
	}
}

// 指定された値で初期化するコンストラクタ
Matrix4x4::Matrix4x4(float elements[4][4]) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			m[i][j] = elements[i][j];
		}
	}
}

Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& other) {
	// 加算の実装
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m[i][j] += other.m[i][j];
	return *this;
}

Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& other) {
	// 減算の実装
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m[i][j] -= other.m[i][j];
	return *this;
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& other) {
	// 乗算の実装
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = 0;
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += m[i][k] * other.m[k][j];
			}
		}
	}
	*this = result;
	return *this;
}

Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = m1;
	result += m2;
	return result;
}

Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = m1;
	result -= m2;
	return result;
}

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = m1;
	result *= m2;
	return result;
}

Vector3::Vector3() : x(0), y(0), z(0) {}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

Vector3 Vector3::operator-() const { return Vector3(-x, -y, -z); }

Vector3 Vector3::operator+() const { return *this; }

Vector3& Vector3::operator+=(const Vector3& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

Vector3& Vector3::operator*=(float s) {
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator/=(float s) {
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

Vector3 operator+(const Vector3& v1, const Vector3& v2) { return Vector3(v1) += v2; }

Vector3 operator-(const Vector3& v1, const Vector3& v2) { return Vector3(v1) -= v2; }

Vector3 operator*(const Vector3& v1, const Vector3& v2) { return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z); }

Vector3 operator*(const Vector3& v, float s) { return Vector3(v) *= s; }

Vector3 operator*(float s, const Vector3& v) { return v * s; }

Vector3 operator/(const Vector3& v, float s) { return Vector3(v) /= s; }