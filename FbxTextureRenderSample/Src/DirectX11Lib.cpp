#include "DirectX11Lib.h"

void AllInit(const char* winName,int sizeX,int sizeY,HINSTANCE hInstance) {

	Window window(winName, sizeX, sizeY);
	// ウィンドウを作成
	window.Create();

	//キー入力初期化
	InitInput(hInstance, FindWindow(Window::ClassName, nullptr));

	DirectGraphics::GetInstance()->Init();


	DXTK11LIB::InitDXTK11Lib();
}

CAMERA::CAMERA() {
	pos = Vector3(0.0f, 0.0f, 0.0f);
	angle = Vector3(0.0f, 0.0f, 0.0f);

	Pos2 returnPosXZ = GetCirclePoint(pos.X, pos.Z, length, angle.Y);
	Pos2 returnPosYZ = GetCirclePoint(pos.Y, pos.Z*-1, length, angle.X);

	focs = Vector3(returnPosXZ.p[0], returnPosYZ.p[0], returnPosXZ.p[1]*-1);
}
void CAMERA::Updata() {

	Pos2 returnPosXZ = GetCirclePoint(pos.X, pos.Z, length, angle.Y);
	Pos2 returnPosYZ = GetCirclePoint(pos.Y, pos.Z * -1, length, angle.X);
	focs = Vector3(returnPosXZ.p[0], returnPosYZ.p[0], returnPosXZ.p[1] * -1);

	DirectGraphics::GetInstance()->SetCamera(pos, focs, Vector3(0, 1, 0));
}
