#include "Player.h"
#include <cassert>
void Player::Initialize(Model* model, uint32_t textureHandle) { 
	assert(model);
	// 引数として受け取ったデータをメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;
	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void Player::Update() {}

void Player::Draw() {}
