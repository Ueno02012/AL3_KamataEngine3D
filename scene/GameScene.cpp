#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>
GameScene::GameScene():gameState_(GameState::Title) {}

GameScene::~GameScene() {
	// 3Dモデルデータの開放
	delete model_;
	// 自キャラの開放
	delete player_;
	// デバックカメラの開放
	delete debugCamera_;
	// 敵キャラの解放
	for (Enemy* enemy : enemys_) {
		delete enemy;
	}
	//  敵の弾の解放
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
	// 天球の解放
	delete modelSkydome_;
	delete skydome_;
	// レールカメラの解放
	delete railCamera_;
	delete barrierModel_;
	delete playerBulletModel_;
	delete enemyBulletModel_;
	//delete barrierModel_;
	delete titleModel_;
	delete gameoverModel_;
	delete clearModel_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	worldScene_.Initialize();
	worldScene_.scale_ = {3, 3, 3};
	worldScene_.translation_.x = -15.0f;
	worldScene_.translation_.y = 0.0f;
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("Player.png");
	EnemytextureHandle_ = TextureManager::Load("Enemy.png");
	// 3Dモデルデータの生成
	model_ = Model::CreateFromOBJ("Player",true);

	playerBulletModel_ = Model::CreateFromOBJ("PlayerBullet", true);
	enemyBulletModel_ = Model::CreateFromOBJ("EnemyBullet", true);
	// 天球の生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	barrierModel_ = Model::CreateFromOBJ("barrier",true);

	titleModel_ = Model::CreateFromOBJ("Title", true);
	gameoverModel_ = Model::CreateFromOBJ("GAMEOVER", true);
	clearModel_ = Model::CreateFromOBJ("GAMEClEAR", true);


	BGM_ = audio_->LoadWave("BGM.wav");

	// UIテクスチャの読み込み
	hpBarBackgroundHandle_ = TextureManager::Load("HPBarBackground.png");
	hpBarHandle_ = TextureManager::Load("HPBar.png");

	// 自機HPバーの生成
	playerHpBarBackground_ = Sprite::Create(hpBarBackgroundHandle_, {50, 50});
	playerHpBar_ = Sprite::Create(hpBarHandle_, {60, 60}); // 背景より少し内側に配置

	// 敵HPバーの生成
	enemyHpBarBackground_ = Sprite::Create(hpBarBackgroundHandle_, {50, 100});
	enemyHpBar_ = Sprite::Create(hpBarHandle_, {60, 110});

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	Vector3 PlayerPosition(0.0f, 0.0f, 40.0f);
	player_->Initialize(model_,barrierModel_,playerBulletModel_, textureHandle_, PlayerPosition);

	

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);


	// 敵発生データの読み込み
	LoadEnemyPopData();

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

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();
	for (EnemyBullet* bullet : enemyBullets_) {
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
	for (Enemy* enemy : enemys_) {
		// 敵キャラの座標
		posB = enemy->GetWorldPosition();
		for (PlayerBullet* bullet : playerBullets) {
			// 自弾の座標
			posA = bullet->GetWorldPosition();
			// 座標AとBの距離を求める
			float distance = Length(Distance(posA, posB));
			// 球同士が当たっていれば
			if (distance < 1.0f) {
				// 敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
				//count += 1;
				//enemy->GetHp();
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	for (PlayerBullet* bulletA : playerBullets) {
		for (EnemyBullet* bulletB : enemyBullets_) {
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
	switch (gameState_) {
	case GameState::Title:
		worldScene_.UpdateMatrix();
		player_->SetHp();
		if (input_->TriggerKey(DIK_RETURN)) {
			// ゲームプレイ初期化処理など
			Initialize();
			gameState_ = GameState::Play;
			Start();
		}
		break;
	// プレイヤーが死んでいない場合のみ更新処理を行う
	case GameState::Play:
		if (!player_->IsDead()) {
			// 衝突判定と応答
			CheckAllCollisions();
			// 敵発生コマンドの更新
			UpdateEnemyPopCommands();
			// デスフラグの立った球を削除
			enemyBullets_.remove_if([](EnemyBullet* bullet) {
				if (bullet->IsDead()) {
					delete bullet;
					return true;
				}
				return false;
			});

			// デスフラグの立った敵を削除
			enemys_.remove_if([](Enemy* enemy) {
				if (enemy->IsDead()) {
					delete enemy;
					return true;
				}
				return false;
			});

			// 発射タイマーカウントダウン
			ReloadTimer_--;
			// 指定時間に達したら
			if (ReloadTimer_ <= 0) {
				// 弾を発射
				Fire();
				ReloadTimer_ = kFireInterval;
			}
			for (EnemyBullet* bullet : enemyBullets_) {
				bullet->Update();
			}

			// 敵キャラの更新
			for (Enemy* enemy : enemys_) {
				enemy->Update();
			}
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
			// ゲームクリアの判定
			if (isEnemySpawned_ && enemys_.empty()) {
				gameState_ = GameState::Clear;
			}
		} else {
			gameState_ = GameState::GameOver;
		}
		break;
	case GameState::Clear:
		worldScene_.UpdateMatrix();

			audio_->StopWave(NowBgm);
		if (input_->PushKey(DIK_RETURN)) {
			gameState_ = GameState::Title; // タイトル画面に戻る
			count = 0;
			player_->SetHp();
		}
		break;

	case GameState::GameOver:
		worldScene_.UpdateMatrix();
		audio_->StopWave(NowBgm);
		if (input_->PushKey(DIK_RETURN)) {
			gameState_ = GameState::Title; // タイトル画面に戻る
			count = 0;
			player_->SetHp();
		}
		break;
	}
//		ImGui::Begin("Life");
//		ImGui::Text("Life:%d", player_->GetHp());
//		ImGui::End();
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

	switch (gameState_) {
	case GameScene::GameState::Title:
		break;
	case GameScene::GameState::Play:
		break;
	case GameScene::GameState::Clear:
		// 2Dの背景を出す場所

		break;
	case GameScene::GameState::GameOver:
		break;
	default:
		break;
	}

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
	switch (gameState_) {
	case GameScene::GameState::Title:
		worldScene_.translation_.x = -17.0f;
		titleModel_->Draw(worldScene_, viewProjection_);
		break;
	case GameScene::GameState::Play:
		// 自キャラの描画
		player_->Draw(viewProjection_);

		// 敵キャラの描画
		for (Enemy* enemy : enemys_) {
			enemy->Draw(viewProjection_);
		}
		//	敵の弾の描画
		for (EnemyBullet* bullet : enemyBullets_) {
			bullet->Draw(viewProjection_);
		}

		// 天球の描画
		skydome_->Draw();
		break;
	case GameScene::GameState::Clear:
		worldScene_.translation_.x = -10.0f;
		clearModel_->Draw(worldScene_, viewProjection_);
		break;
	case GameScene::GameState::GameOver:
		worldScene_.translation_.x =-10.0f;
		gameoverModel_->Draw(worldScene_, viewProjection_);
		break;
	default:
		break;
	}
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

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::Fire() {
	for (Enemy* enemy : enemys_) {
		assert(player_);
		Vector3 pos = enemy->GetWorldTranslation();

		// 自キャラの座標をコピー
		DirectX::XMFLOAT3 position = {pos.x, pos.y, pos.z};

		// 弾の速さ(調整項目)
		const float kBulletSpeed = 1.0f;

		// 自キャラのワールド座標を取得する
		Vector3 playerPosition = player_->GetWorldPosition();
		// 敵キャラのワールド座標を取得する
		Vector3 enemyPosition = enemy->GetWorldPosition();
		// 敵キャラから自キャラへの差分ベクトルを求める
		Vector3 direction = {playerPosition.x - enemyPosition.x, playerPosition.y - enemyPosition.y, playerPosition.z - enemyPosition.z};
		// ベクトルの正規化
		float length = Length(direction);
		// ベクトルの正規化
		Vector3 normalizedDirection = Normalize(direction, length);
		// ベクトルの長さを、速さに合わせる
		Vector3 velocity = {normalizedDirection.x * kBulletSpeed, normalizedDirection.y * kBulletSpeed, normalizedDirection.z * kBulletSpeed};

		// 球を生成し、初期化
		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(enemyBulletModel_, {position.x, position.y, position.z}, velocity);
		// 球を登録する
		AddEnemyBullet(newBullet);
	}
}

void GameScene::ApproachPhaseInitialize() {
	// 発射タイマーを初期化
	ReloadTimer_ = kFireInterval;
}
void GameScene::LoadEnemyPopData() {
	// ファイルを開く
	std::ifstream file{};
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (isWaiting_) {
		waitTimer_--;
		if (waitTimer_ <= 0) {
			// 待機完了
			isWaiting_ = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる

			EnemyPop(Vector3(x, y, z));

			// WAITコマンド
		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isWaiting_ = true;
			waitTimer_ = waitTime;

			// コマンドを抜ける
			break;
		}
	}
}

void GameScene::EnemyPop(Vector3 v) {

	if (!enemys_.empty()) {
		return;

	}
	// 敵キャラの生成
	Enemy* enemy = new Enemy();
	Vector3 initialPosition = {0.0f, 5.0f, 50.0f};
	v = initialPosition;
	enemy->Initialize(model_, EnemytextureHandle_,v);
	// 敵キャラに自キャラのアドレスを渡す
	enemy->SetPlayer(player_);
	// 敵キャラにゲームシーンを渡す
	enemy->SetGameScene(this);
	enemys_.push_back(enemy);
	// 敵生成済みフラグを設定
	isEnemySpawned_ = true;
	
}

void GameScene::Start() {
	enemys_.remove_if([](Enemy* enemy) {
			delete enemy;
			return true;
	});
	LoadEnemyPopData();
	NowBgm = audio_->PlayWave(BGM_);
	
	isEnemySpawned_ = false;
}
