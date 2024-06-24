#include "RailCamera.h"

void RailCamera::Initialize(WorldTransform* worldTransform) {
	
	// ワールドトランスフォームの初期設定
	worldTransform_ = worldTransform;

	viewProjection_.Initialize();
}

void RailCamera::Update() {}
