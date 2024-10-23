
#pragma once
#include "MatRix.h"
#include "PlayerBullet.h"
#include <DirectXMath.h>
#include <Input.h>
#include <Model.h>
#include <WorldTransform.h>
#include <cassert>
#include <imgui.h>
#include <list>

class Player {
public:
	// デストラクタ
	~Player();
	// 初期化
	void Initialize(Model* model, Model* barrierModel, uint32_t textureHandle, Vector3 position);
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
	bool IsDead() const { return isDead_; } // プレイヤーの死亡状態を確認する

private:
	// ワールドトランスフォームの初期化
	WorldTransform worldTransform_;
	WorldTransform worldTransformBarrier_;

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

	Model* barrierModel_;

    int hp_;               // プレイヤーのHP
	bool isDead_;          // プレイヤーが死んでいるかどうか
	bool isBarrierActive_; // バリアが有効かどうかを管理
};
