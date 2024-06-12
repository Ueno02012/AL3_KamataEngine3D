#include "Enemy.h"
#include "Matrix.h"
#include "TextureManager.h"
#include <cassert>
#include "Player.h"
Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}
void Enemy::InitializeApproach() { 
	FireTimer = kFireInterval;
}

void Enemy::UpdateApproach() {

	// 移動(ベクトルを加算)
	worldTransform_.translation_.z -= 0.1f;
	// 基底の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::UpdateLeave() { 

	// 移動(ベクトルを加算)
	worldTransform_.translation_.x -= 0.1f;
	worldTransform_.translation_.y += 0.1f;
}

void Enemy::Fire() {
	//assert(player_);

	// 弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//player_->GetWorldPosition();
	//Enemy::GetWorldPosition()

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	bullets_.push_back(newBullet);
}

//Vector3 Enemy::GetWorldPosition() { 
//
//	//ワールド座標を入れる変数
//	Vector3 worldPos;
//
//	// ワールド行列の平行移動成分を取得(ワールド座標)
//	worldPos.x = worldTransform_.matWorld_.m[3][0];
//	worldPos.y = worldTransform_.matWorld_.m[3][1];
//	worldPos.z = worldTransform_.matWorld_.m[3][2];
//
//	return worldPos;
//}


void Enemy::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("Enemy.png");

	worldTransform_.Initialize();

	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	worldTransform_.translation_.x = 5.0f;
	worldTransform_.translation_.y = 2.0f;
	worldTransform_.translation_.z = 20.0f;

	
}

void Enemy::Update() { 

	// デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	worldTransform_.UpdateMatrix();

	switch (phase_) {
	case Enemy::Phase::Approach:
	default:
		UpdateApproach();

		break;
	case Enemy::Phase::Leave:

		// 移動(ベクトルを加算)
		//worldTransform_.translation_.x -= 0.1f;
		//worldTransform_.translation_.y += 0.1f;
		break;
	}


	FireTimer--;

	if (FireTimer <= 0) {
		Fire();
		InitializeApproach();
	}


	// 弾更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

}

void Enemy::Draw(const ViewProjection& viewProjection) {
	
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 弾描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}



