#include "Skydome.h"



void Skydome::Initialize(Model* model, ViewProjection* viewProjection){
	
	// NULLポインタチェック
	assert(model);
	model_ = model;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// 引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;

}

void Skydome::Update() {}

void Skydome::Draw() {
	model_->Draw(worldTransform_, *viewProjection_);
}
