#include "Enemy.h"
#include "TextureManager.h"
#include <cassert>

void Enemy::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("Enemy.png");

	worldTransform_.Initialize();

	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
}

void Enemy::Update() { 

	worldTransform_.UpdateMatrix();


	worldTransform_.translation_.x = 0;
	worldTransform_.translation_.y = 3;
	worldTransform_.translation_.z -=0.3f;
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
