#include "RailCamera.h"

void RailCamera::Initialize(Vector3 position, Vector3 rotation) {

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000;
	viewProjection_.Initialize();
}

void RailCamera::Update() {

	//worldTransform_.translation_.z += 0.15f;
	//worldTransform_.rotation_.y += 0.001f;
	//  カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	// カメラの座標を画面表示する処理
	ImGui::Begin("RailCamera");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();
}