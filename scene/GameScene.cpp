#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete debugCamera_;
	delete enemy_;
	delete modelSkydome_;
	delete skydome_;
	delete railCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("uvChecker.png");
	EnemytextureHandle_ = TextureManager::Load("Enemy.png");
	// 3Dモデルデータの生成
	model_ = Model::Create();
	// 天球の生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	Vector3 PlayerPosition(0.0f, 0.0f, 40.0f);
	player_->Initialize(model_, textureHandle_, PlayerPosition);

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//	縦方向表示の表示を有効する
	AxisIndicator::GetInstance()->SetVisible(true);
	// 縦方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// 敵キャラの生成
	enemy_ = new Enemy();
	enemy_->Initialize(model_, EnemytextureHandle_);
	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	// 天球の生成
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	// レールカメラの生成
	railCamera_ = new RailCamera();
	railCamera_->Initialize(Vector3{0.0f, 0.0f, -40.0f}, Vector3{0.0f, 0.0f, 0.0f});

	// 自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());
}

void GameScene::CheckAllCollisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();
		Vector3 diff = Distance(posA, posB);
		// 座標AとBの距離を求める
		float distance = Length(diff);
		// 球と球の交差判定
		if (distance < 1.0f) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	// 敵キャラの座標
	posB = enemy_->GetWorldPosition();
	for (PlayerBullet* bullet : playerBullets) {
		// 自弾の座標
		posA = bullet->GetWorldPosition();
		// 座標AとBの距離を求める
		float distance = Length(Distance(posA, posB));
		// 球同士が当たっていれば
		if (distance < 1.0f) {
			// 敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			// 自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	for (PlayerBullet* bulletA : playerBullets) {
		for (EnemyBullet* bulletB : enemyBullets) {
			// 自弾の座標
			posA = bulletA->GetWorldPosition();
			// 敵弾の座標
			posB = bulletB->GetWorldPosition();
			// 座標AとBの距離を求める
			float distance = Length(Distance(posA, posB));
			if (distance < 1.0f) {
				// 自弾の衝突時コールバックを呼び出す
				bulletA->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				bulletB->OnCollision();
			}
		}
	}
#pragma endregion
}

void GameScene::Update() {

	// 敵キャラの更新
	enemy_->Update();
	// 衝突判定と応答
	GameScene::CheckAllCollisions();
	// 天球の更新
	skydome_->Update();

	// デバックカメラの更新
	debugCamera_->Update();
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_TAB)) {
		if (isDebugCameraActive_ == true) {
			isDebugCameraActive_ = false;
		} else {
			isDebugCameraActive_ = true;
		}
	}
#endif //  _DEBUG

	// カメラの処理
	if (isDebugCameraActive_) {
		// デバックカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// レールカメラの更新
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の更新と転送
		viewProjection_.TransferMatrix();
	}
	// 自キャラの更新
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

	// 自キャラの描画
	player_->Draw(viewProjection_);

	// 敵キャラの描画
	enemy_->Draw(viewProjection_);

	// 天球の描画
	skydome_->Draw();

	/// </summary>

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