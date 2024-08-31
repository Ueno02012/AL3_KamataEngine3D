#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include "Vector.h"
#include "imgui.h"
#include <cassert>

GameScene::GameScene() : model_(nullptr), player_(nullptr), debugCamera_(nullptr), enemy_(nullptr), skydome_(nullptr), modelSkydome_(nullptr), isDebugCameraActive_(false) {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete debugCamera_;
	delete enemy_;
	delete skydome_;
	delete modelSkydome_;
	delete railCamera_;
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("Player.png");

	// 3Dモデルの生成
	model_ = Model::Create();
	// 3Dモデル(Skydome)の生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 320;
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();

	// 自キャラの初期化
	Vector3 playerPosition(0, 0, 50);

	player_->Initialize(model_, textureHandle_,playerPosition);

	enemy_ = new Enemy();
	enemy_->Initialize(model_, Vector3(5, 2, 70));

	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(300, 200);


	// レールカメラの初期化
	railCamera_ = new RailCamera();
	Vector3 railworldTransform = {0.0f, 0.0f,0.0f};
	Vector3 railRotation = {0.0f, 0.0f, 0.0f};
	railCamera_->Initialize(railworldTransform, railRotation);

	player_->SetParent(&railCamera_->GetWorldTransform());


	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();

	// 敵キャラの更新
	enemy_->Update();

	// スカイドームの更新
	skydome_->Update();

	// 衝突判定
	CheckAllCollisions();

	


	// プレイヤーのワールドトランスフォームを取得
	WorldTransform& playerTransform = player_->GetWorldTransform();

	// ImGuiを使ったプレイヤーの位置調整
	ImGui::Begin("Move");
	ImGui::DragFloat3("Player", &playerTransform.translation_.x, 0.01f);
	ImGui::End();

// デバッグカメラの切り替え
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RETURN)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		// デバッグカメラがアクティブな場合の行列設定
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		// レールカメラの更新
	
		railCamera_->Update();
		// レールカメラがアクティブな場合の行列設定
		viewProjection_.matView = railCamera_->GetView();
		viewProjection_.matProjection = railCamera_->GetWorld();
		viewProjection_.TransferMatrix();

	}

	// ビュープロジェクション行列の更新と転送
	//viewProjection_.UpdateMatrix();
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

	// sprite_->Draw();

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

	// 敵キャラの描画
	enemy_->Draw(viewProjection_);

	// スカイドームの描画
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

			// 自弾の衝突時コールバックを呼び出す
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
