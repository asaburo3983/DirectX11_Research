#include "Game.h"


Image fakeWindowIm;
bool  fakeWindowGrab[4] = {false,false,false,false};
class FakeWindow {
private:
	int num = -1;
	Pos2 pos;
	Pos2 size;
	
	int angle = 0;
	float sizeEX = 1.0;
public:

	void Init(int _x, int _y,int _num) {
		pos.p[0] = _x; pos.p[1] = _y; size.p[0] = 320; size.p[1] = 240;
		fakeWindowIm.Load("Data/Image/window.png");
		num = _num;
	};
	void updata() {
		Pos2 mouse;
		mouse.p[0] = MouseInput::GetInstance()->x;
		mouse.p[1] = MouseInput::GetInstance()->y;

		//掴む動作
		if (MouseInput::GetInstance()->left >= 1&&HitRect(pos,size, mouse)==true) {
			bool grab = false;
			for(int i = 0; i < 4; i++) {
				if (i != num) {
					if (fakeWindowGrab[i] == true) {
						grab = true;
						break;
					}
				}
			}
			if (!grab) {
				fakeWindowGrab[num] =true;
				pos.p[0] = mouse.p[0];
				pos.p[1] = mouse.p[1];
			}
		}
		else {
			fakeWindowGrab[num] = false;
		}
	};

	void draw() {
		fakeWindowIm.Draw(pos.p[0], pos.p[1], angle, sizeEX);
	}
};
FakeWindow fakeWindow[4];
void Load() {
	
	for (int h = 0; h < 2; h++) {
		for (int i = 0; i < 2; i++) {
			fakeWindow[h*2+i].Init(320 + 480 * (i % 2), 240 + 280 * (h % 2),h*2+i);
		}
	}

}
void Updata() {
	//フェイクウィンドウの行進
	for (int i = 0; i < 4; i++) {
		fakeWindow[i].updata();
	}

}

void Draw() {
	//フェイクウィンドウの描画
	for (int i = 0; i < 4; i++) {
		fakeWindow[i].draw();
	}

}