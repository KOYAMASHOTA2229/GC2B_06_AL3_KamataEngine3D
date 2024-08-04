#include "DeathParticles.h"
#include "cassert"
#include "Model.h"
#include "algorithm"
#include "crossJudge.h"

void DeathParticles::Initialize(Model* model, ViewProjection* viewprojection, const Vector3 position) {

	assert(model);

	model_ = model;

	viewProjection_ = viewprojection;

	for (auto& WorldTransform : worldTransforms_) {
		WorldTransform.Initialize();
		WorldTransform.translation_ = position;
	}

}

void DeathParticles::Update() {

	// 終了なら何もしない
	if (isFinished_) {
		return;
	}

	for (auto& WorldTransform : worldTransforms_) {
		WorldTransform.UpdateMatrix();
	}

	for (uint32_t i = 0; i < kNumParticles; i++) {
		Vector3 velocity = {kSpeed, 0, 0};
		float angle = kAngleUnit * i;
		Matrix4x4 rotateMatrixZ = crossJudge::MakeRotateZMatrix(angle);
		velocity =crossJudge::Transform(velocity, rotateMatrixZ);
		
		worldTransforms_[i].translation_ += velocity;
	}

	
	counter_ += 1.0f / 60.0f;

	//上限に達したら終了に
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		
		isFinished_ = true;
	}

}

void DeathParticles::Draw() {

	// 終了なら何もしない
	if (isFinished_) {
		return;
	}

	for (auto& WorldTransform : worldTransforms_) {
		model_->Draw(WorldTransform, *viewProjection_);
	}
}
