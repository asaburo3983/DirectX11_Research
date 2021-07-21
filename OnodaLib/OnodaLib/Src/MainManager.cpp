#include "MainManager.h"

void MainManager::Load(Scene* _scene) {
	
	KEY = KeyInput::GetInstance();
	MOUSE = MouseInput::GetInstance();

	sceneOld = scene;
	scene = _scene;

	AllDeleteLocal();//���[�J���f�[�^�̍폜
	scene->Load();
}

void MainManager::Updata() {
	UpdateInput();//���͏����X�V	

	scene->Updata();
}
void MainManager::Draw() {
	DirectGraphics::GetInstance()->StartRendering();

	scene->Draw();

	DirectGraphics::GetInstance()->FinishRendering();//�`�悷�閈��̏���
}
