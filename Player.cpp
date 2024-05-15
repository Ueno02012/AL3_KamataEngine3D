#include "Player.h"
#include "cassert"
void Player::Initialize(Model* model, ViewProjection* viewProjection) {
	
	// NULLポインタチェック
	assert(model);
	// 引数として受け取ったデータをメンバ変数に記録
	model_ = model;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	// 引数の内容をメンバ変数に記録する
	worldTransform_.scale_ = {1.3f, 1.3f, 1.3f};
	worldTransform_.translation_ = {-3.0f, 0.0f, 0.0f};

	viewProjection_ = viewProjection;
}

void Player::Update() {

	// 行列の転送
	worldTransform_.matWorld_;
	worldTransform_.UpdateMatrix();


}

void Player::Draw() {

	// 3Dモデルを描画する
	model_->Draw(worldTransform_, *viewProjection_);
}