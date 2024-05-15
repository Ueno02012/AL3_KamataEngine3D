#include "Skydome.h"
#include "cassert"

Skydome::Skydome() {}
Skydome::~Skydome() {
}

void Skydome::Intialize(Model* model, ViewProjection* viewProjection) { 
	assert(model);
	model_ = model;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.scale_ = {100, 100, 100};
	// 引数の内容をメンバ変数に記録する
	viewProjection_ = viewProjection;
}


void Skydome::Update() {
	

	// 行列の転送
	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();

}

void Skydome::Draw() { 
     model_->Draw(worldTransform_, *viewProjection_);

}