#pragma once

#include "Audio.h"
#include "CameraController.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "MapChipField.h"
#include "Model.h"
#include "Player.h"
#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <vector>
#include "Enemy.h"
#include "crossJudge.h"
#include "DeathParticles.h"
#include "Clear.h"

enum class Phase {
	kPlay,
	kDeath,
};

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene{
private:

	//  テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 生成
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Skydome* skydome_ = nullptr;
	Player* player_ = nullptr;
	Model* modelPlayer_ = nullptr;
	DebugCamera* debugCamera_ = nullptr;

	CameraController* cameraController_ = nullptr;

	Model* modelEnemy_ = nullptr;
	std::list<Enemy*> enemies_;

	Model* modelDeathParticles_ = nullptr;
	DeathParticles* deathParticles_ = nullptr;

	//ゴール
	Model* modelGoal_ = nullptr;
	Clear* goal_ = nullptr;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// ビュープロジェクション
	ViewProjection viewProjection_;
	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	MapChipField* mapChipField_;
	void GenerateBlocks();

	// 現在の現在フェーズ
	Phase phase_;

	// 終了フラグ
	bool isFinished_ = false;

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	void EnemyCollision();

	void GoalCollision();

	void CheckAllCollision();

	void UpdatekPlay();

	void UpdateKDeath();

	void ChangePhase();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// デスフラグのgetter
	bool IsFinished() const { return this->isFinished_; }


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
