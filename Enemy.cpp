#include "Enemy.h"
#include "Player.h"
#include <imgui.h>

void Enemy::OnCollision() { 
	hp_ -= 10;
	if (hp_ <= 0) {
		isDead_ = true;
	}
};

Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
};

Vector3 Enemy::GetWorldTranslation() {
	// 敵の座標を入れる変数
	Vector3 Pos;
	// 敵の座標成分を取得
	Pos.x = worldTransform_.translation_.x;
	Pos.y = worldTransform_.translation_.y;
	Pos.z = worldTransform_.translation_.z;
	return Pos;
};

Enemy::~Enemy() {
}

void Enemy::Initialize(Model* model, uint32_t textureHandle,Vector3 Position) {
	assert(model);
	// 引数として受け取ったデータをメンバ変数に記録
	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = textureHandle;
	// ワールド変換の初期化
	worldTransform_.translation_ = Position;
	worldTransform_.Initialize();
	input_ = Input::GetInstance();
	hp_ = 100; // 10回弾に当たると死ぬ
	maxHp_ = 100;
	isDead_ = false; // 最初は生きている
}

void Enemy::Update() {

	// キャラクターの移動速さ
	const float approachSpeed = 0.08f;
	const float LeaveSpeed = 0.05f;
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
	// ある程度まで近づいてきたら停止
	if (worldTransform_.translation_.z <= 10.0f) {
		worldTransform_.translation_.z = 10.0f;
	}
	//if (worldTransform_.translation_.y >= -5.0f) {
	//	worldTransform_.translation_.y = -5.0f;
	//}

	// 定数バッファに転送する
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
	ImGui::Begin("Enemy");
	ImGui::DragFloat3("Enemy", &worldTransform_.translation_.x, 0.01f);
	ImGui::End();
}


void Enemy::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::ApproachMove(const float speed) {
	// 移動(ベクトルを加算)
	worldTransform_.translation_.z -= speed;
	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z <= 10.0f) {
		phase_ = Phase::Leave;
	}
}
void Enemy::LeavePhaseMove(const float speed) {
	// 移動(ベクトルを加算)
	if (worldTransform_.translation_.x <= 5.0f) {
		worldTransform_.translation_.x += speed;

	}
	if (worldTransform_.translation_.y >= -5.0f) {
		worldTransform_.translation_.y -= speed;
	}
	//worldTransform_.translation_.z -= speed;
}

