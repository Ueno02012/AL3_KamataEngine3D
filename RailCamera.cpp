#include "RailCamera.h"
#include "ImGuiManager.h"

void RailCamera::Initialize(Vector3 worldTransform,Vector3 rotation) {

	// ワールドトランスフォームの初期設定
	worldTransform_.Initialize();

	worldTransform_.translation_ = worldTransform;
	worldTransform_.rotation_ = rotation;

	viewProjection_.Initialize();
	viewProjection_.farZ = 1000.0f;
}

void RailCamera::Update() {


	worldTransform_.translation_.z -= 0.1f;
	worldTransform_.rotation_.y += 0.001f;
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	worldTransform_.UpdateMatrix();

	ImGui::Begin("Camera");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotate", &worldTransform_.rotation_.y, 0.01f);
	ImGui::End();


}

//const Matrix4x4& RailCamera::GetViewProjectionMatrix() const {
//	return viewProjection_.matView;//ビュープロジェクション行列を返す
//}