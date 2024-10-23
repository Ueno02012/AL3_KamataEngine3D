#include "Player.h"

void Player::OnCollision() {
	// バリアが有効ならばダメージを無効化
	if (isBarrierActive_) {
		// ダメージ無効化の処理
		return;
	}
	// それ以外の通常の衝突処理
	// バリアがない状態で敵の弾に当たった場合、HPを減少させる
	hp_--;

	// HPが0以下になったら死亡フラグを立てる
	if (hp_ <= 0) {
		isDead_ = true;
		// プレイヤーの死亡処理を追加する場合はここに書く
	}
}
// 親子関係を結ぶ
void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
};

Player::~Player() {
	// bullets_の解放
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Initialize(Model* model, Model* barrierModel,uint32_t textureHandle, Vector3 position) {
	assert(model);
	assert(barrierModel);
	// 引数として受け取ったデータをメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransformBarrier_.Initialize();
	worldTransformBarrier_.scale_ = {2, 2, 2};
	worldTransform_.translation_ = position;
	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	// プレイヤーのHPを初期化
	hp_ = 10;                 // 10回弾に当たると死ぬ
	isDead_ = false;          // 最初は生きている
	isBarrierActive_ = false; // 最初はバリアがない
	barrierModel_ = barrierModel; // バリアのモデルは必要なら外部から渡す
	
}

void Player::Update() {
	// デスフラグの立った球を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.2f;
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;

	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 19.0f;
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// バリアのON/OFF
	if (input_->TriggerKey(DIK_RETURN)) {
		isBarrierActive_ = !isBarrierActive_; // バリアの状態を反転
	}

	if (isBarrierActive_) {
		// バリアが有効な場合、プレイヤーは攻撃できない
		bulletsTimer_ = ReloadTime_; // 攻撃のクールダウンを維持
	} else {
		// 通常の攻撃処理
		Attack();
	}

	// バリアが有効なら無敵状態
	if (isBarrierActive_) {
		// 無敵状態の処理が必要であればここに追加
	}

	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// プレイヤーの回転処理
	const float kRotSpeed = 0.02f;
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

	// ワールド行列の更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	worldTransformBarrier_.translation_ = worldTransform_.translation_;
	worldTransformBarrier_.translation_.x -= 0.5f;
	worldTransformBarrier_.matWorld_ = MakeAffineMatrix(worldTransformBarrier_.scale_, worldTransformBarrier_.rotation_, worldTransformBarrier_.translation_);
	worldTransformBarrier_.TransferMatrix();
}

void Player::Attack() {
	if (input_->PushKey(DIK_SPACE)) {
		bulletsTimer_--;
		if (bulletsTimer_ <= 0) {
			// 自キャラの座標をコピー
			DirectX::XMFLOAT3 position = {
			    worldTransform_.matWorld_.m[3][0],
			    worldTransform_.matWorld_.m[3][1],
			    worldTransform_.matWorld_.m[3][2],
			};

			// 弾の速度
			const float kBulletSpeed = 1.0f;
			Vector3 velocity(0, 0, kBulletSpeed);
			velocity = TransformNormal(velocity, worldTransform_.matWorld_);
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(model_, {position.x, position.y, position.z}, velocity);
			bullets_.push_back(newBullet);

			bulletsTimer_ = ReloadTime_;
		}
	} else {
		bulletsTimer_ = ReloadTime_;
	}
}

void Player::Draw(ViewProjection& viewProjection) {
	// プレイヤーの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// バリアが有効ならバリアを描画
	if (isBarrierActive_ && barrierModel_) {
		barrierModel_->Draw(worldTransformBarrier_, viewProjection);
	}

	// 弾の描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}
