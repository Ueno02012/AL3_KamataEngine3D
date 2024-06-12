#include "Enemy.h"
#include "TextureManager.h"
#include <cassert>

void Enemy::UpdateApproach() {

	// 移動(ベクトルを加算)
	worldTransform_.translation_.z -= 0.1f;
	// 基底の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::UpdateLeave() { 

	// 移動(ベクトルを加算)
	worldTransform_.translation_.x -= 0.1f;
	worldTransform_.translation_.y += 0.1f;
}


void Enemy::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("Enemy.png");

	worldTransform_.Initialize();

	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	worldTransform_.translation_.y = 2.0f;
	worldTransform_.translation_.z = 10.0f;

	
}

void Enemy::Update() { 

	worldTransform_.UpdateMatrix();

	switch (phase_) {
	case Enemy::Phase::Approach:
	default:
		UpdateApproach();

		break;
	case Enemy::Phase::Leave:

		// 移動(ベクトルを加算)
		worldTransform_.translation_.x -= 0.1f;
		worldTransform_.translation_.y += 0.1f;
		break;
	}


}

void Enemy::Draw(const ViewProjection& viewProjection) {
	
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

