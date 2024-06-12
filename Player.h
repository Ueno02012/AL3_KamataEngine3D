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

	void Initialize(Model* model,uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	// ワールド座標を取得
	Vector3 GetWorldPosition();


	WorldTransform& GetWorldTransform() { return worldTransform_; }

private:

	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	const float kRotSpeed = 0.02f;
	//弾
	std::list<PlayerBullet*> bullets_;


};
