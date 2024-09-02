#include "Player.h"

void Player::OnCollision() {}

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
	//  bullets_の解放
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 position) {
	assert(model);
	// 引数として受け取ったデータをメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
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
	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;
	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;

	// 活動限界座標
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 19.0f;
	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

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

	// 親が存在する場合は、親の変換を考慮して自身の変換を更新
	if (worldTransform_.parent_) {
		worldTransform_.UpdateMatrix();
	} else {
		// 親がいない場合、自身の変換のみを適用
		worldTransform_.UpdateMatrix();
	}

	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;
	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Player");
	ImGui::DragFloat3("Player", &worldTransform_.translation_.x, 0.01f);
	ImGui::End();

	// キャラクターの攻撃処理
	Attack();
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
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
			// 速度ベクトルを自機の向きに合わせて回転させる
			velocity = TransformNormal(velocity, worldTransform_.matWorld_);
			// 球を生成し、初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(model_, {position.x, position.y, position.z}, velocity);
			// 球を登録する
			bullets_.push_back(newBullet);

			bulletsTimer_ = ReloadTime_;
		}
	} else {
		bulletsTimer_ = ReloadTime_;
	}
}

void Player::Draw(ViewProjection& viewProjection) {
	// プレイヤー描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//	弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}