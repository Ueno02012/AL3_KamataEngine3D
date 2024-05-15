#pragma once
#include "Model.h"
#include "WorldTransform.h"

//天球

class Skydome {
public:
	void Intialize(Model* model, ViewProjection* viewProjection);



	void Update();



	void Draw();

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;
};
