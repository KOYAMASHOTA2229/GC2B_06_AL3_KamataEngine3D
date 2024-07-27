#include "GameScene.h"
#include "MapChipField.h"
#include "Matrix4x4.h"
#include "Skydome.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include <cassert>
#include "Player.h"

// 02_05


GameScene::GameScene() {}

GameScene::~GameScene() {

	delete model_;

	delete debugCamera_;

	delete modelSkydome_;

	delete skydome_;

	delete mapChipField_;

	delete player_;

	delete modelPlayer_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t kNumBlockVirtical = mapChipField_->kNumBlockVirtical;
	uint32_t kNumBlockHorizontal = mapChipField_->kNumBlockHorizontal;

	// 要素数を変更する
	worldTransformBlocks_.resize(20);
	for (uint32_t i = 0; i < 20; i++) {
		worldTransformBlocks_[i].resize(100);
	}
	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// 3Dモデルの生成
	model_ = Model::Create();

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	mapChipField_ = new MapChipField();

	mapChipField_->LoadMapChipCsv("Resources/block.csv");

	modelPlayer_ = Model::CreateFromOBJ("player", true);

	GenerateBlocks();

	//自キャラの位置生成
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 17);
	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(modelPlayer_, &viewProjection_, playerPosition);

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// カメラコントローラのインスタンス作成
	cameraController_ = new CameraController();
	// 初期化処理
	cameraController_->Initialize();
	// 追従対象をセット
	cameraController_->SetTarget(player_);
	// リセット(瞬間合わせ)
	cameraController_->Reset();

	cameraController_->SetMovableArea({20, 175, 0, 50});


	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//天球
	skydome_ = new Skydome();

	skydome_->Initialize(modelSkydome_, &viewProjection_);

}

void GameScene::Update() {
	player_->Update();
	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			worldTransformBlock->matWorld_ = Matrix4x4::MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			// 定数バッファに転送
			worldTransformBlock->TransferMatrix();
		}
	}

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_C)) {
		// デバッグカメラ有効のフラグがおられている時
		if (isDebugCameraActive_ == false) {
			// フラグを立てる
			isDebugCameraActive_ = true;
		} else if (isDebugCameraActive_ == true) {
			// フラグを折る
			isDebugCameraActive_ = false;

			}
	}
#endif // DEBUG

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		viewProjection_.TransferMatrix();
	} else {
		// カメラコントローラの更新処理
		cameraController_->Update();

		// カメラコントローラからビュー行列とプロジェクション行列をコピーする
		viewProjection_.matView = cameraController_->GetViewProjection().matView;
		viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	player_->Draw(viewProjection_);

	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			model_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	skydome_->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
