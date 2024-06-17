#pragma once
#include "Model.h"
#include "Vector3.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include <list>

class Player;


class Enemy {

	// 行動フェーズ
	enum class Phase {
		Approach, // 接近
		Leave     // 離脱
	};

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void InitializeApproach();

	/// <summary>
	/// 接近フェーズの更新関数
	/// </summary>
	void UpdateApproach();

	/// <summary>
	/// 離脱フェーズの更新関数
	/// </summary>
	void UpdateLeave();

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	void SetPlayer(Player* player) { player_ = player; }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

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



	// ワールド座標を入れる変数
	Vector3 GetWorldPosition();

	static const int kFireInterval = 60;

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }


private:

	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	//自キャラ
	Player* player_ = nullptr;


	// フェーズ
	Phase phase_ = Phase::Approach;



	int32_t FireTimer = 0;

	// 弾
	std::list<EnemyBullet*> bullets_;

};
