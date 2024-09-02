// #pragma once
// #include "Model.h"
// #include "WorldTransform.h"
// #include <assert.h>
//
///// <summary>
///// 基底クラス
///// </summary>
// class GameObject {
// public:
//	virtual ~GameObject() = default;
//
//	virtual void Initialize(Model* model, const Vector3& position) {
//		assert(model);
//		model_ = model;
//		worldTransform_.Initialize();
//		worldTransform_.translation_ = position;
//	}
//
//	virtual void Update() { worldTransform_.UpdateMatrix(); }
//
//	virtual void Draw(const ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, textureHandle_); }
//
//	WorldTransform& GetWorldTransform() { return worldTransform_; }
//
// protected:
//	WorldTransform worldTransform_;
//	Model* model_ = nullptr;
//	uint32_t textureHandle_ = 0u;
// };
