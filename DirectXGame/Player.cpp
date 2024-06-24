#include "Player.h"
#include "cassert"

void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {

	// NULLポインタチェック
	assert(model);

	model_ = model;

	textureHandle_ = textureHandle;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;
}

void Player::Update() {

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

}

void Player::Draw() {

	//3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

}
