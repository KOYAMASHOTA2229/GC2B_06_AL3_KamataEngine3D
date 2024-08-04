#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "array"

#pragma once
class DeathParticles {

public:

	void Initialize(Model* model, ViewProjection* viewprojection, const Vector3 position);

	void Update();

	void Draw();

	private:

	    Model* model_ = nullptr;

	    ViewProjection* viewProjection_ = nullptr;

		// パーティクルの個数
	    static inline const uint32_t kNumParticles = 8;

	    std::array<WorldTransform, kNumParticles> worldTransforms_;

		// 存続時間
	    static inline const float kDuration = 2.0f;

	    //移動
	    static inline const float kSpeed = 0.08f;

		static inline const float pi = 3.14f;

		//角度
		static inline const float kAngleUnit = (2 * pi) / kNumParticles;

		// 終了フラグ
	    bool isFinished_ = false;
	    float counter_ = 0.0f;

};
