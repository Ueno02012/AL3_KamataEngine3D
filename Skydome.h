#pragma once
#include "WorldTransform.h"
#include "Model.h"
class Skydome {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


private:

	// ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;
};
