#include "Player.h"
#include "cassert"

void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {

	// NULLポインタチェック
	assert(model);
	// 引数として受け取ったデータをメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	// 引数の内容をメンバ変数に記録する
	viewProjection_ = viewProjection;
}

void Player::Update() {

	// 行列の転送
	worldTransform_.matWorld_;
	worldTransform_.TransferMatrix();
}

void Player::Draw() {

	// 3Dモデルを描画する
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}