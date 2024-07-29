#define NOMINMAX
#include "Player.h"
#include <Input.h>
#include <Matrix4x4.h>
#include <numbers>
#define _USE_MATH_DEFINES
#include <algorithm>
#include <math.h>
#include "MapChipField.h"
#include "DebugText.h"
using namespace std;

 float Player::EaseInOutSine(float easing) { return -(cosf(float(M_PI) * easing) - 1) / 2; }

 

void Player::Move() {
	if (onGround_) {
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			// 左右加速
			Vector3 acceleration = {};

			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {

				if (velocity_.x < 0.0f) {
					// 急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x += kAcceleration;

				// 振り向き
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}

			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

				if (velocity_.x > 0.0f) {
					// 急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x -= kAcceleration;

				// 振り向き
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}

			}

			// 加速減速
			velocity_ += acceleration;

			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);


		} else {

			velocity_.x *= (1.0f - kAttenuation);
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			velocity_ += Vector3(0, kJumpAcceleration, 0);
		}
		// 空中
	} else {

		velocity_ += Vector3(0, -kGravityAcceleration, 0);

		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}


}

bool Player::Landing() {
	// 着地フラグ
	bool landing = false;

	// 地面との当たり判定
	if (velocity_.y < 0) {
		// Y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= 2.0f) {
			landing = true;
		}
	} // 接地判定

	return landing;

}

void Player::SwitchOnGround(bool landing) {
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態に移行
			onGround_ = false;
		}
	} else {
		// 着地
		if (landing) {
			// めり込み排斥
			worldTransform_.translation_.y = 2.0f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuation);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 接地状態に移行
			onGround_ = true;
		}
	}
}

void Player::Rotation() {

	// 旋回制御
	if (turnTimer_ > 0.0f) 
	{

		turnTimer_ -= 1.0f / 60.0f;

		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

		// 状況に応じた目標角度
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		// 自キャラの角度
		worldTransform_.rotation_.y = std::lerp(turnFirstRotationY_, destinationRotationY,1-turnTimer_/kTimeTurn);
	}

}

void Player::CollisionMapJudge(CollisionMapInfo& info) { 
	CollisionMapUpJudge(info); 
	/*CollisionMapDownJudge(info);
	CollisionMapLeftJudge(info);
	CollisionMapRightJudge(info);*/
}

void Player::CollisionMapUpJudge(CollisionMapInfo& info) {

	// 上昇あり？
	if (info.move.y <= 0) {
		return;
	}


	array<Vector3, static_cast<uint32_t>(Corner::kNumCorner)> positionsNew;

	for (uint32_t index = 0; index < positionsNew.size(); index++) {
		positionsNew[index] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(index));
	}

	MapChipType mapChipType;

	// 真上の当たり判定
	bool hit = false;
	
	// 左上
	IndexSet indexSet;

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット?
	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(info.move);
		mapChipRect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::max(0.0f, info.move.y);
		info.hitCeilingFlag = true;
	}
}


//void Player::CollisionMapDownJudge(CollisionMapInfo& info) {}
//
//void Player::CollisionMapLeftJudge(CollisionMapInfo& info) {}
//
//void Player::CollisionMapRightJudge(CollisionMapInfo& info) {}

void Player::judgeMove(const CollisionMapInfo& info) {

	worldTransform_.translation_ += info.move;

}

void Player::hitCeilling(const CollisionMapInfo& info) {
	if (info.hitCeilingFlag) {

		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}

}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
        {-kWidth / 2.0f, -kHeight / 2.0f, 0},
        {+kWidth / 2.0f, +kHeight / 2.0f, 0},
        {-kWidth / 2.0f, +kHeight / 2.0f, 0}
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}


void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {

	// NULLポインタチェック
	assert(model);

	model_ = model;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	// 引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;
}

void Player::Update() {


	Move();

	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.move = velocity_;

	CollisionMapJudge(collisionMapInfo);

	hitCeilling(collisionMapInfo);

	bool isLanding = Landing();

	SwitchOnGround(isLanding);

	Rotation();
	// 移動
	worldTransform_.translation_ += velocity_;

	// 行列計算
	worldTransform_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection);
}
