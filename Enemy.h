#pragma once
#include "EnemyBullet.h"
#include "MatRix.h"
#include "TextureManager.h"
#include <DirectXMath.h>
#include <Input.h>
#include <Model.h>
#include <WorldTransform.h>
#include <cassert>
#include <algorithm>

#include <list>
// 自機クラスの宣言
class Player;

enum class Phase {
	approach, // 接近する
	Leave,    // 離脱する
};

// ゲームシーンの前方宣言
class GameScene;

class Enemy {
public:
	// デストラクタ
	~Enemy();
	// 初期化
	void Initialize(Model* model, uint32_t textureHandle, Vector3 Position);
	// 更新処理
	void Update();
	// 描画処理
	void Draw(const ViewProjection& viewProjection);

	// 接近速度
	void ApproachMove(const float speed);
	// 離脱速度
	void LeavePhaseMove(const float speed);

	// 発射感覚
	static const int kFireInterval = 60;
	// 接近フェーズ初期化
	void ApproachPhaseInitialize();

	void SetPlayer(Player* player) { player_ = player; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();
	// 敵の座標を取得
	Vector3 GetWorldTranslation();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	// HPのゲッター
	int GetHp() const { return hp_; }
	int GetMaxHp() const { return maxHp_; }
	//void SetHp() { hp_ = 0; }

	bool IsDead() const { return isDead_; }

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
	// 自キャラ
	Player* player_ = nullptr;
	// ゲームシーン
	GameScene* gameScene_ = nullptr;
	// デスフラグ
	bool isDead_;
	int hp_;    // 現在のHP
	int maxHp_; // 最大HP
};