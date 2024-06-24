#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "DebugCamera.h"
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画　
	/// </summary>
	void Draw();

	void CheckAllCollisions();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// スプライト
	//Sprite* sprite_ = nullptr;
	
	uint32_t textureHandle_= 0;

	//　3Dモデル
	Model* model_ = nullptr;

	Player* player_ = nullptr;

	Enemy* enemy_ = nullptr;

	Skydome* skydome_ = nullptr;

	Model* modelSkydome_ = nullptr;

	//レールカメラ
	Model* railCamera_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// ImGuiで値を入力する変数
	//float inputFloat3[3] = {0, 0, 0};

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
