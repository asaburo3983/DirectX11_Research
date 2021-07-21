#pragma once

#include <string>
#include "MainManager.h"

class MainManager;
//����̔h���N���X�������SceneManager�ɖ��ߍ���
class Scene{

protected:
	MainManager* mana;
public:
	Scene();
	virtual void Load()=0;
	virtual void Updata()=0;
	virtual void Draw()=0;
};
