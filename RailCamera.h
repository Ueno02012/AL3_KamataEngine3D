#pragma once
#include "ImGuiManager.h"
#include "MatRix.h"
#include "ViewProjection.h"
#include <Model.h>
#include <WorldTransform.h>
#include <cassert>
//#include <imgui.h>

class RailCamera {
public:
	// 初期化
	void Initialize(Vector3 position, Vector3 rotation);
	// 更新処理
	void Update();

	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	const ViewProjection& GetView() { return viewProjection_; }
	const ViewProjection& GetProjection() { return viewProjection_; }

	// ワールド座標を取得
	// const WorldTransform GetWorldPosition();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

};