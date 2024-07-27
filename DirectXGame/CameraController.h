#pragma once
#include "ViewProjection.h"

class Player;

struct Rect {
	float left = 0.0f;   
	float right = 1.0f; 
	float bottom = 0.0f;
	float top = 1.0f; 
};

Vector3 Lerp(const Vector3& a, const Vector3& b, float t);

	class CameraController {
public:

	CameraController();

	~CameraController();

	void Initialize();

	void Reset();

	void Update();

	// セッター
	public:

		void SetTarget(Player* target) { target_ = target; }

		const ViewProjection& GetViewProjection() { return viewProjection_; }

		void SetMovableArea(const Rect& area) { movableArea_ = area; }

		private:

			ViewProjection viewProjection_;

			Player* target_ = nullptr;

			Vector3 targetOffset_ = {0, 0, -30.0f};

			Rect movableArea_ = {0, 100, 0, 100};

			Rect margin_ = {-5, 10, -5, 10};

			Vector3 targetPosition_;

			static inline const float kIntrepolationRate = 0.9f;

			static inline const float kVelocityBias_ = 15.0f;

    };