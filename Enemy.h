#pragma once
#include<Model.h>
#include<WorldTransform.h>
#include<cassert>
#include "TextureManager.h"
#include"MatRix.h"
#include<list>
#include<Input.h>
#include<DirectXMath.h>
#include "EnemyBullet.h"

// 自機クラスの宣言
class Player;

enum class Phase {
	approach, // 接近する
	Leave,    // 離脱する
};

class Enemy {
public:
	// デストラクタ
	~Enemy();
	// 初期化
	void Initialize(Model* model, uint32_t textureHandle);
	// 更新処理
	void Update();
	// 描画処理
	void Draw(const ViewProjection& viewProjection);

	// 接近速度
	void ApproachMove(const float speed);
	// 離脱速度
	void LeavePhaseMove(const float speed);	
	
	// 攻撃
	void Fire();

	// 発射感覚
	static const int kFireInterval = 60;
	// 接近フェーズ初期化
	void ApproachPhaseInitialize();

	void SetPlayer(Player* player) { player_ = player; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

private:
	// ワールドトランスフォームの初期化
	WorldTransform worldTransform_;
	// 3Dモデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	// フェーズ
	Phase phase_ = Phase::approach;
	// 弾
	std::list<EnemyBullet*> bullets_;
	// 発射タイマー
	int32_t ReloadTimer_ = 0;

	// 自キャラ
	Player* player_ = nullptr;
};