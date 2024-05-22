#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include<cmath>
#include"Matrix.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_;
	delete player_;
	delete blockmodel_;
	delete debugCamera_;
	delete modelSkydome_;
	delete skydome_;
	delete mapChipField_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}

	}
	worldTransformBlocks_.clear();

}
void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	//  列数を設定(縦のブロック数)
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		// 列数を設定(横のブロック数)
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// キューブの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}


void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	//自機の初期化

	//テクスチャを読み込む
	//textureHandle_ = TextureManager::Load("uvChecker.png");

	//3Dモデルの生成
	model_ = Model::CreateFromOBJ("Chara",true);
	//3Dモデル(Skydome)の生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	
	// 3Dモデル(Block)の生成
	blockmodel_ = Model::Create();
	//ワールドトランスフォームの初期化
	//worldTransform_.Initialize();
	
	 
	//ビュープロジェクションの初期化
	viewProjection_.farZ = 320;
	viewProjection_.Initialize();
	//自機の生成
	player_ = new Player();
	//天球の生成
	skydome_ = new Skydome();

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	//自機の初期化
	player_->Initialize(model_,&viewProjection_);
	
	//Skydoneの初期化
	skydome_->Intialize(modelSkydome_, &viewProjection_);


	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	GenerateBlocks();
}


void GameScene::Update() {

	////自機の更新
	player_->Update(); 
	
	skydome_->Update();

	//ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
			worldTransformBlock->TransferMatrix();

		}
	}
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RETURN)) {
		isDebugCameraActive_ = true;	
	}
#endif // _DEBUG

	//カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView =debugCamera_->DebugCamera::GetView();
		viewProjection_.matProjection = debugCamera_->DebugCamera::GetProjection();

		viewProjection_.TransferMatrix();

	} else {
		viewProjection_.UpdateMatrix();
	}

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

	//自機の描画
	player_->Draw();
	
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			blockmodel_->Draw(*worldTransformBlock, viewProjection_);
		}
	}
		
	skydome_->Draw();



	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

