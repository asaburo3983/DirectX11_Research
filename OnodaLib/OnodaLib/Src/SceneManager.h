#pragma once

#include <string>
#include "MainManager.h"

class MainManager;
//‚±‚ê‚Ì”h¶ƒNƒ‰ƒX‚ğì‚Á‚ÄSceneManager‚É–„‚ß‚Ş
class Scene{

protected:
	MainManager* mana;
public:
	Scene();
	virtual void Load()=0;
	virtual void Updata()=0;
	virtual void Draw()=0;
};
