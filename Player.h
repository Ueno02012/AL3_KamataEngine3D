#pragma once
#include "WorldTransform.h"
#include"Model.h"


class Player {
public:

	/*初期化*/
	void Initialize(Model* model, ViewProjection* viewProjection);
	//更新
	void Update();
	//描画
	void Draw();

private:
		
		//ワールド変換
	    WorldTransform worldTransform_;
		//モデル
	    Model* model_ = nullptr;
		//テクスチャハンドル
	    uint32_t textureHandle_ = 0u;

		ViewProjection* viewProjection_ = nullptr;




};
