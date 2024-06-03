#pragma once
#include "WorldTransform.h"
#include"Model.h"
// 左右
enum class LRDirection {
	kRight,
	kLeft,
};



class Player {
public:

	/*初期化*/
	void Initialize(Model* model, ViewProjection* viewProjection,const Vector3& position);
	//更新
	void Update();
	//描画
	void Draw();

	float Lerp(float a, float b, float t) { return a + t * (b - a);}

private:
		
		//ワールド変換
	    WorldTransform worldTransform_;
		//モデル
	    Model* model_ = nullptr;
		//テクスチャハンドル
	    uint32_t textureHandle_ = 0u;

		ViewProjection* viewProjection_ = nullptr;

		Vector3 velocity_ = {};

		LRDirection lrDirection_ = LRDirection::kRight;

		// 旋回開始時の角度
		float turnFirstRotationY_ = 0.0f;

		// 旋回タイマー
	    float turnTimer_ = 0.0f;

		// 設置状態フラグ
		bool onGround_ = true;

		static inline const float kAcceletation = 0.01f;
	    static inline const float kAttenuation = 0.1f;
	    static inline const float kLimitRunSpeed = 5.0f;

		// 旋回時間 <秒>
	    static inline const float kTimeTurn = 0.3f;

		// 重力加速度(下方向)
	    static inline const float kGravityAcceleration = 0.03f;

		// 最大落下速度(下方向)
	    static inline const float kLimitFallSpeed = 0.1f;

		// ジャンプ加速(上方向)
	    static inline const float kJumpAcceleration = 0.4f;

		//摩擦
		static inline const float kAttenuationLanding = 0.05f;
};
