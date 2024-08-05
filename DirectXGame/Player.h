#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"
#include "Vector3.h"

//前方宣言
class MapChipField;

struct CollisionMapInfo {
	bool hitCeilingFlag = false;
	bool isLandingFlag = false;
	bool hitWallFlag = false;

	Vector3 move;
};

//角
enum Corner {
	kRightBottom,
	kLeftBottom,
	kRightTop,
	kLeftTop,

	kNumCorner
};

//左右
enum class LRDirection {
	kRight,
	kLeft,
};

/// <summary>
/// 自キャラ
/// </summary>
class Player {
private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	//ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

	MapChipField* mapChipField_ = nullptr;

	Vector3 velocity_ = {};

	//慣性移動
	static inline const float kAcceleration = 0.05f;

	//速度減衰
	static inline const float kAttenuation = 0.1f;

	// 最大速度制限
	static inline const float kLimitRunSpeed = 0.5f;

	LRDirection lrDirection_ = LRDirection::kRight;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間
	static inline const float kTimeTurn = 0.3f;

	// 接地状態フラグ
	bool onGround_ = true;

	// 重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.5f;
	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 0.3f;
	// ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 2.0f;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 1.8f;
	static inline const float kHeight =1.8f;

	static inline const float kAttenuationLanding = 0.4f;
	static inline const float kAttenuationWall = 1.0f;

	static inline const float kBlankWidth = 0.1f;
	static inline const float kBlankHeight = 0.1f;

	static inline const float kBlank = 0.1f;

	void Move();
	bool Landing();
	void SwitchOnGround(bool landing);

	void Rotation();

	void CollisionMapJudge(CollisionMapInfo& info);

	void CollisionMapUpJudge(CollisionMapInfo& info);
	void CollisionMapDownJudge(CollisionMapInfo& info);
	void CollisionMapLeftJudge(CollisionMapInfo& info);
	void CollisionMapRightJudge(CollisionMapInfo& info);

	//反映させて移動
	void judgeMove(const CollisionMapInfo& info);

	//天井接地時
	void hitCeilling(const CollisionMapInfo& info);

	Vector3 CornerPosition(const Vector3& center, Corner corner);

	//接地の状態
	void groundState(const CollisionMapInfo& info);
	//上記の切り替え
	void switchState(const CollisionMapInfo& info);

	void touchWall(const CollisionMapInfo& info);

public:
	/// 初期化
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	/// 更新
	void Update();

	/// 描画
	void Draw(const ViewProjection& viewProjection);

	const WorldTransform& GetWorldTransform() { return worldTransform_; };

	const Vector3& GetVelocity() const { return velocity_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	float EaseInOutSine(float easing);


};