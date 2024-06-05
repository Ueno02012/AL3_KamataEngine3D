#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"
GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_;
	delete player_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle_ = TextureManager::Load("uvChecker.png");


	// 3Dモデルの生成
	model_ = Model::Create();

	// ワールドトランスフォームの初期化
	//worldTransform_.Initialize();

	viewProjection_.Initialize();



	//自キャラの生成
	player_ = new Player();

	//自キャラの初期化
	player_->Initialize(model_,textureHandle_);


	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	// デバッグカメラの生成
	//debugCamera_ = new DebugCamera(1280, 720);

	//// 軸方向表示の表示を有効にする
	//AxisIndicator::GetInstance()->SetVisible(true);
	//// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	//AxisIndicator::SetTargetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() { 
	//自キャラの更新
	player_->Update();

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	//sprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 自キャラの描画
	player_->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f,0.0f, 0.0f, 1.0f});

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
