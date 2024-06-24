#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player {
private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;

public:
	/// 初期化
	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection);

	/// 更新
	void Update();

	/// 描画
	void Draw();
};
