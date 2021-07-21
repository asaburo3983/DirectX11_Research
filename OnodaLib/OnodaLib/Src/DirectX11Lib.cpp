#include "DirectX11Lib.h"

void AllInit(const char* winName,int sizeX,int sizeY,bool fullScreen,HINSTANCE hInstance) {

	Window window(winName, sizeX, sizeY);
	// ウィンドウを作成
	window.Create(fullScreen);

	//キー入力初期化
	InitInput(hInstance, FindWindow(Window::ClassName, nullptr));

	DirectGraphics::GetInstance()->Init();

	//2D音楽、2D画像系の初期化
	DXTK11LIB::InitDXTK11Lib();
}
void SetAntialias(bool on) {
	DirectGraphics::GetInstance()->SetAntialias(on);
}

