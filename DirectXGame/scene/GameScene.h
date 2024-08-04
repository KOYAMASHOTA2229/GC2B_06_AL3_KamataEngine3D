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

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
private:
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

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// ビュープロジェクション
	ViewProjection viewProjection_;
	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	MapChipField* mapChipField_;
	void GenerateBlocks();

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
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
