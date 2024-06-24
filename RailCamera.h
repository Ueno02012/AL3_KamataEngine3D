#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Matrix.h"
class RailCamera {

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WorldTransform* worldTransform,Vector3 rotation);


	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	const WorldTransform& GetWorldTransform() { return *worldTransform_; }

	const ViewProjection& GetViewProjection() { return viewProjection_; }

private:

	// ワールド変換データ
	WorldTransform* worldTransform_;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	Vector3 velocity{1,1,1};

};
