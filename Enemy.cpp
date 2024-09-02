#include "Enemy.h"
#include"Player.h"

void Enemy::OnCollision(){};

 Vector3 Enemy::GetWorldPosition(){
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
};

Enemy::~Enemy() {
	//  bullets_の解放
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	// 引数として受け取ったデータをメンバ変数に記録
	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = textureHandle;
	// ワールド変換の初期化
	worldTransform_.translation_= {15.0f,3.0f,80.0f};
	worldTransform_.Initialize();
	input_ = Input::GetInstance();

}

void Enemy::Update() {
	
	// デスフラグの立った球を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// キャラクターの移動速さ
	const float approachSpeed = 0.05f;
	const float LeaveSpeed = 0.1f;
	switch (phase_) {
	case Phase::approach:
	default:
		ApproachMove(approachSpeed);
		break;
	case Phase::Leave:
		LeavePhaseMove(LeaveSpeed);
		break;
	}
	
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			// スケーリング行列
			Matrix4x4 scale = MakeScaleMatrix(worldTransformBlock->scale_);
			// X軸の回転行列
			Matrix4x4 rotateX = MakeRotateXMatrix(worldTransformBlock->rotation_.x);
			// Y軸の回転行列
			Matrix4x4 rotateY = MakeRotateYMatrix(worldTransformBlock->rotation_.y);
			// Z軸の回転行列
			Matrix4x4 rotateZ = MakeRotateZMatrix(worldTransformBlock->rotation_.z);
			// 回転行列の合成
			Matrix4x4 rotationZXY = Multiply(rotateZ, Multiply(rotateX, rotateY));
			// 平行移動行列の作成
			Matrix4x4 translate = MakeTranslateMatrix(worldTransformBlock->translation_);
		}
	}

	// 定数バッファに転送する
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	// 発射タイマーカウントダウン
	ReloadTimer_--;
	// 指定時間に達したら
	if (ReloadTimer_ <= 0) {	
		// 弾を発射
		Fire();
		ReloadTimer_ = kFireInterval;
	}
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}	
}

void Enemy::Fire() {	
	assert(player_);

	// 自キャラの座標をコピー
	DirectX::XMFLOAT3 position = {
		worldTransform_.translation_.x, 
		worldTransform_.translation_.y,
		worldTransform_.translation_.z
	};

	// 弾の速さ(調整項目)
	const float kBulletSpeed = 1.0f;

	// 自キャラのワールド座標を取得する
	Vector3 playerPosition = player_->GetWorldPosition();
	// 敵キャラのワールド座標を取得する
	Vector3 enemyPosition = GetWorldPosition();
	// 敵キャラから自キャラへの差分ベクトルを求める
	Vector3 direction = {
		playerPosition.x - enemyPosition.x,
		playerPosition.y - enemyPosition.y,
		playerPosition.z - enemyPosition.z
	};
	// ベクトルの正規化
	float length = Length(direction);
	// ベクトルの正規化
	Vector3 normalizedDirection = Normalize(direction, length);	
	// ベクトルの長さを、速さに合わせる
	Vector3 velocity = {
		normalizedDirection.x * kBulletSpeed,
		normalizedDirection.y * kBulletSpeed,
		normalizedDirection.z * kBulletSpeed
	};

	// 球を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, {position.x, position.y, position.z}, velocity);
	// 球を登録する
	bullets_.push_back(newBullet);
}


void Enemy::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//	弾描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}



void Enemy::ApproachMove(const float speed) {
	// 移動(ベクトルを加算)
	worldTransform_.translation_.z -= speed;
	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		//phase_ = Phase::Leave;
	}
}
void Enemy::LeavePhaseMove(const float speed) {
	// 移動(ベクトルを加算)
	worldTransform_.translation_.x += speed;
	worldTransform_.translation_.y += speed;
	worldTransform_.translation_.z -= speed;
}

void Enemy::ApproachPhaseInitialize() {
	// 発射タイマーを初期化
	ReloadTimer_ = kFireInterval;
}