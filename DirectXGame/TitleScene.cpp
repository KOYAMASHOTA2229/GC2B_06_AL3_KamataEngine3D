#include "TitleScene.h"
#include "Player.h"

void TitleScene::Initialize() {}

void TitleScene::Update() {

if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		isFinished_ = true;
	}


}

void TitleScene::Draw() {}
