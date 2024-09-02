#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <assert.h>
#include <cmath>

// 長さ(ノルム)
float Length(const Vector3& v);
// ベクトルを正規化
Vector3 Normalize(const Vector3& v);
// ベクトルを正規化
Vector3 NorMalize(const Vector3& v, const float length);

// 1. X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);

// 2. Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);

// 3. Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

// 4.合成
Matrix4x4 Multiply(const Matrix4x4 m1, const Matrix4x4 m2);

// 5.3次元アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& teanslate);

// ビューボート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float facDlip);

// スケール行列
Matrix4x4 MakeScaleMatrix(const Vector3& Scale);

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

// 座標AとBの距離を引く
Vector3 Distance(const Vector3& A, const Vector3& B);
// 逆行列
Matrix4x4 Inverse(const Matrix4x4& matrix);
