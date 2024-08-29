#include "GameScene.h"
#include "MapChipField.h"
#include "Matrix4x4.h"
#include "Player.h"
#include "Skydome.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include <cassert>
#include "TitleScene.h"

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

	delete modelGoal_;
	delete goal_;

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	enemies_.clear();

	delete modelEnemy_;

	delete modelDeathParticles_;

	if (deathParticles_ != nullptr) {
		delete deathParticles_;
	}

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

	modelPlayer_ = Model::CreateFromOBJ("player", true);

	modelEnemy_ = Model::CreateFromOBJ("enemy", true);

	mapChipField_ = new MapChipField();

	mapChipField_->LoadMapChipCsv("Resources/block.csv");

	GenerateBlocks();
	// 自キャラの位置生成
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);
	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(modelPlayer_, &viewProjection_, playerPosition);

	//ゴールの位置
	Vector3 goalPosition = mapChipField_->GetMapChipPositionByIndex(48, 18);
	modelGoal_ = Model::CreateFromOBJ("goal", true);
	goal_ = new Clear();
	goal_->Initialize(modelGoal_, &viewProjection_, goalPosition);

	// デス演出用パーティクルの生成と初期化
	modelDeathParticles_ = Model::CreateFromOBJ("deathParticles", true);
	deathParticles_ = new DeathParticles();
	deathParticles_->Initialize(modelDeathParticles_, &viewProjection_, playerPosition);

	// 敵キャラの位置生成
	Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(12, 18);
	for (int32_t i = 1; i < 8; i++) {
		Enemy* newEnemy = new Enemy();

		// X座標とY座標の計算を直接行う
	 enemyPosition = mapChipField_->GetMapChipPositionByIndex(12*i, 18);

		newEnemy->Initialize(modelEnemy_, &viewProjection_, enemyPosition);
		enemies_.push_back(newEnemy);
	}

	player_->SetMapChipField(mapChipField_);

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
	// 天球
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	// ゲームプレイフェーズから開始
	phase_ = Phase::kPlay;

}

void GameScene::EnemyCollision() {

	#pragma region 自キャラと敵キャラの当たり判定

	AABB aabb1, aabb2;

	aabb1 = player_->GetAABB();

	for (Enemy* enemy : enemies_) {

		aabb2 = enemy->GetAABB();

		// AABB同士の交差判定
		if (crossJudge::IsCollision(aabb1, aabb2)) {
			// 自キャラの衝突時コールバック関数を呼び出す
			player_->OnCollision(enemy);

			enemy->OnCollision(player_);
		}
	}
	// 自キャラがデス状態
	if (player_->GetIsDead()) {

		phase_ = Phase::kDeath;

		const Vector3& deathParticlesPosition = player_->GetWorldPosition();

		deathParticles_ = new DeathParticles();
		deathParticles_->Initialize(modelDeathParticles_, &viewProjection_, deathParticlesPosition);
	}
#pragma endregion

}

void GameScene::GoalCollision() {

	AABB aabb1, aabb2;

	aabb1 = player_->GetAABB();

	aabb2 = goal_->GetAABB();

	if (crossJudge::IsCollision(aabb1, aabb2)) {

		isFinished_ = true;

	}
}

void GameScene::CheckAllCollision() {

	GameScene::EnemyCollision();

	GameScene::GoalCollision();

}

void ::GameScene::Update() { GameScene::ChangePhase(); }

	void GameScene::UpdatekPlay() {

		// 自キャラの更新
		player_->Update();

		goal_->Update();

		// 敵の更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

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

		// 全ての当たり判定
		GameScene::CheckAllCollision();
	}

	void GameScene::UpdateKDeath() {

		goal_->Update();

		// 敵の更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		// デス演出用パーティクルの更新処理
		if (deathParticles_ != nullptr) {
			deathParticles_->Update();
		}

		if (input_->TriggerKey(DIK_SPACE)) {
			if (isDebugCameraActive_ == false) {
				isDebugCameraActive_ = true;
			} else if (isDebugCameraActive_ == true) {
				isDebugCameraActive_ = false;
			}
		}
		// デバッグカメラ有効のフラグが立っている時に
		if (isDebugCameraActive_ == true) {
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
			viewProjection_.TransferMatrix();
		}
		// ブロックの更新処理
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}
				worldTransformBlock->UpdateMatrix();
			}
		}

		if (deathParticles_ && deathParticles_->GetIsFinished()) {
		    isFinished_ = true;
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

	player_->Draw();

	goal_->Draw();

	// デス演出用パーティクルの描画
	if (deathParticles_ != nullptr) {
		deathParticles_->Draw();
	}

	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection_);
	}

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

void GameScene::ChangePhase() {

	switch (phase_) {
	case Phase::kPlay:

		GameScene::UpdatekPlay();

		break;

	case Phase::kDeath:

		GameScene::UpdateKDeath();

		break;
	}
}
