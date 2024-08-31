#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <Input.h>
#include "PlayerBullet.h"
#include <list>

class Player {

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	void Initialize(Model* model,uint32_t textureHandle,Vector3 position);

	void Update();

	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();


	//衝突を検出したらコールバック関数
	void OnCollision();

	void SetParent(const WorldTransform* parent);

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	WorldTransform& GetWorldTransform() { return worldTransform_; }

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

private:

	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	Vector3 position_ = {0,0,0};

	const float kRotSpeed = 0.02f;
	//弾
	std::list<PlayerBullet*> bullets_;


};
