#include "RailCamera.h"


void RailCamera::Initialize(Vector3 worldTransform,Vector3 rotation) {

	// ワールドトランスフォームの初期設定
	worldTransform_.translation_ = worldTransform;
	worldTransform_.rotation_ = rotation;

	viewProjection_.Initialize();
}

void RailCamera::Update() {

	worldTransform_.UpdateMatrix();

	viewProjection_.matView = Inverse(worldTransform_.matWorld_);



}

