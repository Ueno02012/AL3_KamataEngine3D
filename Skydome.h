#pragma once
#include<Model.h>
#include<WorldTransform.h>
#include<cassert>
#include "ViewProjection.h"
#include<imgui.h>

class Skydome {
public:
	// 初期化
	void Initialize(Model* model, ViewProjection* viewProjection);
	// 更新処理
	void Update();
	// 描画処理
	void Draw();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;
	// 3Dモデル
	Model* model_ = nullptr;
};

