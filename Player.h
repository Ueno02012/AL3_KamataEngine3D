#pragma once
#include<Model.h>
#include<WorldTransform.h>
#include "ViewProjection.h"
#include<cassert>
#include<Input.h>
#include"MatRix.h"
#include<imgui.h>
#include "PlayerBullet.h"
#include<list>
#include<DirectXMath.h>
#include<Sprite.h>
#include "WinApp.h"


class Player {
public:
	// デストラクタ
	~Player();
	// 初期化
	void Initialize(Model* model, uint32_t textureHandle, Vector3 position, ViewProjection* viewProjection);
	// 更新処理
	void Update();
	// 描画処理
	void Draw(ViewProjection& viewProjection);
	// 攻撃
	void Attack();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	// 親となるワールドトランスフォームをセット
	void SetParent(const WorldTransform* parent);
	
	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	static Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
		Vector3 result{};
		result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
		result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
		result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
		float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
		assert(w != 0.0f);
		result.x /= w;
		result.y /= w;
		result.z /= w;
		return result;
	}


private:
	// ワールドトランスフォームの初期化
	WorldTransform worldTransform_;
	// 3Dモデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// キーボード入力
	Input* input_ = nullptr;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	// 弾
	std::list<PlayerBullet*> bullets_;
	// 発射タイマー
	int32_t bulletsTimer_ = 0;
	// リロードタイム
	int32_t ReloadTime_ = 10;
	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

	// 3Dレティクル用トランスフォーム
	WorldTransform worldTransform3DReticle_;
	// 2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;
	// 3Dから2Dの計算
	Vector3 positionReticle;
	// ビューポート行列
	Matrix4x4 matViewport;
};
