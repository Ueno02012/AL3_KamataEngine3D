#include "GameScene.h"
#include "Vector.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "imgui.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_;
	delete player_;
	delete debugCamera_;
	delete enemy_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("uvChecker.png");

	// 3Dモデルの生成
	model_ = Model::Create();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);


	enemy_ = new Enemy();
	enemy_->Initialize(model_, Vector3(5,2,70));
	
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(300, 200);


	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

}

void GameScene::Update() { 
	//自キャラの更新
	player_->Update();

	enemy_->Update();

	CheckAllCollisions();

	// デバッグカメラの更新
	debugCamera_->Update();

	WorldTransform& playerTransform = player_->GetWorldTransform();

	ImGui::Begin("Move");
	ImGui::DragFloat3("Player", &playerTransform.translation_.x, 0.01f);
	ImGui::End();

	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RETURN)) {
		isDebugCameraActive_ = true;
	}
	#endif

	if (isDebugCameraActive_) {
		

		viewProjection_.matView = debugCamera_->DebugCamera::GetView();
		viewProjection_.matProjection = debugCamera_->DebugCamera::GetProjection();

		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		//ビュープロジェクション行列の更新と転送
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
	player_->Draw(viewProjection_);

	enemy_->Draw(viewProjection_);

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

void GameScene::CheckAllCollisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

// 自キャラと敵弾の当たり判定
#pragma region
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {

		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		Vector3 distance = Subtract(posB, posA);

		if ((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z) <= 1.0f) {

			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();

			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

// 自弾と敵キャラの当たり判定
#pragma region

	posA = enemy_->GetWorldPosition();

	for (PlayerBullet* bullet : playerBullets) {

		posB = bullet->GetWorldPosition();

		Vector3 distance = Subtract(posB, posA);

		if ((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z) <= 1.0f) {

			// 敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();

			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

// 自弾と敵弾の当たり判定
#pragma region
	for (PlayerBullet* bullet : playerBullets) {
		for (EnemyBullet* bullets : enemyBullets) {

			posA = bullet->GetWorldPosition();
			posB = bullets->GetWorldPosition();
			Vector3 distance = Subtract(posB, posA);

			if ((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z) <= 1.0f) {

				bullet->OnCollision();
				bullets->OnCollision();
			}
		}
	}
#pragma endregion
}
