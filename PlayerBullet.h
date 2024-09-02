#pragma once
#include<Model.h>
#include<WorldTransform.h>
#include<cassert>
#include "TextureManager.h"
#include"MatRix.h"

class PlayerBullet {
public:
	// 初期化
	void Initialize(Model* model, const Vector3& position,const Vector3& velocity);
	// 更新処理
	void Update();
	// 描画処理
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

private:
	// ワールドトランスフォームの初期化
	WorldTransform worldTransform_;
    // 3Dモデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 velocity_;
	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime; 
	// デスフラグ
	bool isDead_ = false;
};
