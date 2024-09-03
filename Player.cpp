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
	delete sprite2DReticle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 position, ViewProjection* viewProjection) {
	assert(model);
	// 引数として受け取ったデータをメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	//  3Dレティクルのワールドトランスフォームの初期化
	worldTransform3DReticle_.Initialize();
	// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("Target.png");
	// スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle, Vector2(0.5, 0.5f), Vector4(1.0f, 1.0f, 1.0f, 1.0f));
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
	const float kCharacterSpeed = 0.5f;

	// 自機のワールド座標から3Dレティクルのワールド座標を計算処理

	// 自機から3Dレティクルへの距離
	const float kDistancePlayerToReticle = 50.0f;
	// 自機から3Dレティクルへのオフセット（Z+向き）
	Vector3 offset = {0.0f, 0.0f, 1.0f};
	// 自機のワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	// ベクトルの長さを整える
	offset.x = Normalize(offset).x * kDistancePlayerToReticle;
	offset.y = Normalize(offset).y * kDistancePlayerToReticle;
	offset.z = Normalize(offset).z * kDistancePlayerToReticle;
	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_.x = worldTransform_.translation_.x + offset.x;
	worldTransform3DReticle_.translation_.y = worldTransform_.translation_.y + offset.y;
	worldTransform3DReticle_.translation_.z = worldTransform_.translation_.z + offset.z;

	// 行列の更新
	worldTransform3DReticle_.UpdateMatrix();

	// 3Dレティクルのワールド座標からから2Dレティクルのスクリーン座標にを計算
	positionReticle = {worldTransform3DReticle_.matWorld_.m[3][0], worldTransform3DReticle_.matWorld_.m[3][1], worldTransform3DReticle_.matWorld_.m[3][2]};
	// ビューポート行列
	matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = Multiply(Multiply(viewProjection_->matView, viewProjection_->matProjection), matViewport);
	// ワールド→スクリーン座標変換（ここで3Dから2Dになる）
	positionReticle = Transform(positionReticle, matViewProjectionViewport);
	// スプライトのレティクル座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x - 32, positionReticle.y - 32));


	// スプライトの現在座標を取得
	Vector2 spritePosition = sprite2DReticle_->GetPosition();

	// ゲームパッドの状態を得る変数（XINPUT）
	XINPUT_STATE joyState;

	// ジョイスティックが有効なら
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
		spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
		// スプライトの座標変更を取得
		sprite2DReticle_->SetPosition(spritePosition);
	}
	// スクリーン座標
	Vector3 posNear = Vector3((float)positionReticle.x, (float)positionReticle.y, 0);
	Vector3 posFar = Vector3((float)positionReticle.x, (float)positionReticle.y, 1);

	matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	
	// ビュープロジェクションビューポート
	Matrix4x4 matVPV =Multiply(Multiply( viewProjection_->matView , viewProjection_->matProjection ), matViewport);

	// 合成関数の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);
	

	// スクリーン座標からワールド座標系へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

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

	// ゲームパッドの状態を得る変数（XINPUT）
	XINPUT_STATE joyState;

	// ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	// Rトリガーを押していたら
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
	
		bulletsTimer_--;
		if (bulletsTimer_ <= 0) {

			// 自機のワールド座標を取得
			Vector3 worldPosition = GetWorldPosition();

			// 弾の速度
			const float kBulletSpeed = 1.0f;

			Vector3 velocity;
			// 速度ベクトルを自機の向きに合わせて回転させる
			velocity.x = worldTransform3DReticle_.translation_.x - worldTransform_.translation_.x;
			velocity.y = worldTransform3DReticle_.translation_.y - worldTransform_.translation_.y;
			velocity.z = worldTransform3DReticle_.translation_.z - worldTransform_.translation_.z;
			velocity.x = Normalize(velocity).x * kBulletSpeed;
			velocity.y = Normalize(velocity).y * kBulletSpeed;
			velocity.z = Normalize(velocity).z * kBulletSpeed;
			// 球を生成し、初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(model_, worldPosition, velocity);
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
	// 3Dレティクルの描画
	//model_->Draw(worldTransform3DReticle_, viewProjection);
}

void Player::DrawUI() { sprite2DReticle_->Draw(); }

//void Player::MouseUpdate() {
//
//	POINT mousePosotion;
//	// マウス座標（スクリーン座標）を取得する
//	GetCursorPos(&mousePosotion);
//
//	// クライアントエリア座標に変換する
//	HWND hwnd = WinApp::GetInstance()->GetHwnd();
//	ScreenToClient(hwnd, &mousePosotion);
//
//	// マウス座標を2Dレティクルのスプライトに代入
//	positionReticle = {worldTransform3DReticle_.matWorld_.m[3][0], worldTransform3DReticle_.matWorld_.m[3][1], worldTransform3DReticle_.matWorld_.m[3][2]};
//	matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
//
//	// ビュープロジェクションビューポート
//	Matrix4x4 matVPV =Multiply(Multiply( viewProjection_->matView , viewProjection_->matProjection ), matViewport);
//
//	// 合成関数の逆行列を計算する
//	Matrix4x4 matInverseVPV = Inverse(matVPV);
//
//	// スクリーン座標
//	Vector3 posNear = Vector3((float)mousePosotion.x, (float)mousePosotion.y, 0);
//	Vector3 posFar = Vector3((float)mousePosotion.x, (float)mousePosotion.y, 1);
//
//	// スクリーン座標からワールド座標系へ
//	posNear = Transform(posNear, matInverseVPV);
//	posFar = Transform(posFar, matInverseVPV);
//
//	// マウスレイの方向
//	Vector3 mouseDirection = {posFar.x - posNear.x, posFar.y - posNear.y, posFar.z - posNear.z};
//	mouseDirection = Normalize(mouseDirection);
//
//	// カメラから照準オブジェクトの距離
//	const float kDistanceTestObject = 215.0f;
//
//	worldTransform3DReticle_.translation_ = {
//	    posNear.x + mouseDirection.x * kDistanceTestObject, posNear.y + mouseDirection.y * kDistanceTestObject, posNear.z + mouseDirection.z * kDistanceTestObject};
//
//	worldTransform3DReticle_.UpdateMatrix();
//
//	// デバッグ表示
//	 ImGui::Begin("Player");
//	 ImGui::Text("2DReticle : (%f,%f)", positionReticle.x, positionReticle.y);
//	 ImGui::Text("Near : (%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
//	 ImGui::Text("Far : (%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
//	 ImGui::Text("3DReticle : (%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x, worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
//	 ImGui::End();
//}