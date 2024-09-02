#include "Skydome.h"

void Skydome::Initialize(Model* model, ViewProjection* viewProjection) {

	assert(model);
	// 引数として受け取ったデータをメンバ変数に記録
	model_ = model;

	// 引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;
	viewProjection_->farZ = 550.0f;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.scale_ = {500.0f,500.0f,500.0f};
}


void Skydome::Update(){

	// 行列の転送
	worldTransform_.matWorld_;
	worldTransform_.UpdateMatrix();
}


void Skydome::Draw() { 
	// 3Dモデルを描画
	model_->Draw(worldTransform_,*viewProjection_);
}