#include "DirectX11Lib.h"

void AllInit(const char* winName,int sizeX,int sizeY,bool fullScreen,HINSTANCE hInstance) {

	Window window(winName, sizeX, sizeY);
	// �E�B���h�E���쐬
	window.Create(fullScreen);

	//�L�[���͏�����
	InitInput(hInstance, FindWindow(Window::ClassName, nullptr));

	DirectGraphics::GetInstance()->Init();

	//2D���y�A2D�摜�n�̏�����
	DXTK11LIB::InitDXTK11Lib();
}
void SetAntialias(bool on) {
	DirectGraphics::GetInstance()->SetAntialias(on);
}

