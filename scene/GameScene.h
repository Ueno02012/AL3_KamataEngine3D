#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "RailCamera.h"
#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <sstream>
#include<list>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数

	enum class GameState { 
	 Title, 
	 Play, 
	 Clear,
	 GameOver 
	};

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

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet"></param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);
	// 攻撃
	void Fire();
	// 発射感覚
	static const int kFireInterval = 60;
	// 接近フェーズ初期化
	void ApproachPhaseInitialize();

	///// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	void EnemyPop(Vector3 v);

	void Start();
	//void DrawTitleScreen();    // タイトル画面の描画
	//void DrawClearScreen();    // クリア画面の描画
	//void DrawGameOverScreen(); // ゲームオーバー画面の描画

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	uint32_t EnemytextureHandle_ = 0u;
	// 3Dモデルデータ
	Model* model_ = nullptr;
	Model* modelEnemy_ = nullptr;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 自キャラ
	Player* player_ = nullptr;

	Enemy* enemy_ = nullptr;
	int count = 0;
	// デバックカメラ有効
	bool isDebugCameraActive_ = false;
	// デバックカメラ
	DebugCamera* debugCamera_ = nullptr;
	// 敵キャラ	
	std::list<Enemy*> enemys_;
	// 敵の弾
	std::list<EnemyBullet*> enemyBullets_;
	// 敵発生コマンド
	std::stringstream enemyPopCommands;
	Vector3 EnemyPosition;
	// 敵の待機
	bool isWaiting_ = false;
	int32_t waitTimer_ = 0;  

	// 発射タイマー
	int32_t ReloadTimer_ = 0;

	// 3Dモデル(天球)
	Model* modelSkydome_ = nullptr;
	// 天球
	Skydome* skydome_ = nullptr;
	// カメラ
	RailCamera* railCamera_ = nullptr;

	Model* barrierModel_ = nullptr;
	Model* playerBulletModel_ = nullptr;
	Model* enemyBulletModel_ = nullptr;
	GameState gameState_; // ゲームの現在の状態を保持

	WorldTransform worldScene_;

	Model* titleModel_ = nullptr;
	Model* clearModel_ = nullptr;
	Model* gameoverModel_ = nullptr;

	// UIスプライト
	uint32_t hpBarBackgroundHandle_; // HPバー背景
	uint32_t hpBarHandle_;           // HPバー
	Sprite* playerHpBarBackground_;  // 自機HPバー背景スプライト
	Sprite* playerHpBar_;            // 自機HPバースプライト
	Sprite* enemyHpBarBackground_;   // 敵HPバー背景スプライト
	Sprite* enemyHpBar_;             // 敵HPバースプライト

	bool isEnemySpawned_ = false; // 敵が生成されたかどうかを管理するフラグ

	uint32_t BGM_ = 0;
	uint32_t NowBgm = 0;
	/// <summary>
/// ゲームシーン用
/// </summary>


};

