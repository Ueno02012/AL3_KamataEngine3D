#pragma once
#include "Model.h"
#include "Vector3.h"
#include "WorldTransform.h"
class Enemy {

	// 行動フェーズ
	enum class Phase {
		Approach, // 接近
		Leave     // 離脱
	};

public:
	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 接近フェーズの更新関数
	/// </summary>
	void UpdateApproach();


	/// <summary>
	/// 離脱フェーズの更新関数
	/// </summary>
	void UpdateLeave();

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	// フェーズ
	Phase phase_ = Phase::Approach;
};
