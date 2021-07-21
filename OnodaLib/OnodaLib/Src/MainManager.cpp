#include "MainManager.h"

void MainManager::Load(Scene* _scene) {
	
	KEY = KeyInput::GetInstance();
	MOUSE = MouseInput::GetInstance();

	sceneOld = scene;
	scene = _scene;

	AllDeleteLocal();//ローカルデータの削除
	scene->Load();
}

void MainManager::Updata() {
	UpdateInput();//入力処理更新	

	scene->Updata();
}
void MainManager::Draw() {
	DirectGraphics::GetInstance()->StartRendering();

	scene->Draw();

	DirectGraphics::GetInstance()->FinishRendering();//描画する毎回の処理
}
