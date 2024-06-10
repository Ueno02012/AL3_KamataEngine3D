#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <Input.h>
#include "PlayerBullet.h"
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

	WorldTransform& GetWorldTransform() { return worldTransform_; }

private:

	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	const float kRotSpeed = 0.02f;

	//弾
	PlayerBullet* bullet_ = nullptr;


};
