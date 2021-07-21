#pragma once

#include "ResourceManager.h"
#include "SceneManager.h"
#include "input.h"

class Scene;
class MainManager : public ResourceManager {

private:
	Scene* scene;
	Scene* sceneOld;

public:
	//“ü—ÍŒn
	KeyInput* KEY;
	MouseInput* MOUSE;
	X_INPUT pad1;
	X_INPUT pad2;
	X_INPUT pad3;
	X_INPUT pad4;

	//‰¹—Ê
	float volumeMaster = 1.0;
	float volumeBGM = 0.5;
	float volumeSE = 0.5;

public:
	static MainManager* GetInstance() {
		static MainManager gm;
		return &gm;
	}
	void Load(Scene* _scene);
	void Updata();
	void Draw();
	
};