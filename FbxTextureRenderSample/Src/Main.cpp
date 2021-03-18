#include <Windows.h>
#include <DirectXMath.h>
#include <vector>
#include <time.h>
#include <mmsystem.h>
#include "Window.h"
#include "DirectGraphics.h"
#include "FbxMeshFile.h"
#define FBXSDK_SHARED
#pragma comment(lib, "winmm.lib")
/*
#pragma comment( lib, "libfbxsdk-mt.lib" )//FBXSDK用　追加の依存ファイルに入れる　MD(動的)orMT(静的
#pragma comment( lib, "libxml2-mt.lib" )
#pragma comment( lib, "zlib-mt.lib" )
*/
#include "DXTK11Lib.h"
#include "input.h"
using namespace DXTK11LIB;



int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmpLine,
	INT       nCmdShow)
{
	Window window("UNT", 1920, 1080);
	// ウィンドウを作成
	if (window.Create() == false)
	{
		return 0;
	}

	//キー入力初期化
	InitInput(hInstance, FindWindow(Window::ClassName, nullptr));

	DirectGraphics::GetInstance()->Init();

	DirectGraphics::GetInstance()->SetCamera(Vector3(0, 0, -5), Vector3(0, 1, 0), Vector3(0, 0, 0.1));
	DirectGraphics::GetInstance()->SetProjection(45.0f, 0.1f, 1000.0f, true);


	FbxMeshFile* model = new FbxMeshFile();
	model->con = DirectGraphics::GetInstance()->GetContext();
	model->Load("Res/Model/S.fbx",
		//"../x64/Debug/VertexShader_Live2D.cso",
		"../x64/Debug/VertexShader.cso",
		"../x64/Debug/GeometryShader.cso",
		"../x64/Debug/PixelShader.cso");


	
	FbxMeshFile* model2 = new FbxMeshFile();
	model2->con = DirectGraphics::GetInstance()->GetContext();
	model2->Load("Res/Model/stage.fbx",
		"../x64/Debug/VertexShader.cso",
		"../x64/Debug/GeometryShader.cso",
		"../x64/Debug/PixelShader.cso");


	//3D用
	Vector3 pos = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 degree = Vector3(90.0f, 0.0f, 180.0f);
	Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);
	//scale=scale / 200;
	InitDXTK11Lib();

	while (true)
	{
		MSG msg;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			UpdateInput();//入力処理更新	
			DirectGraphics::GetInstance()->StartRendering();
			
			//人モデル影表示
			model->RenderS_SM(DirectGraphics::GetInstance(), pos, scale, degree, Vector3(5, 0, 0), Vector3(0, 0, 0), Vector3(0, 1, 0), 45);

			DirectGraphics::GetInstance()->SetCamera(Vector3(0, 0, -4), Vector3(0, 0, 0), Vector3(0, 1, 0));
			DirectGraphics::GetInstance()->SetProjection(45.0f, 0.1f, 1000.0f, true);

			model->RenderS(DirectGraphics::GetInstance(),pos,scale,degree);

		//	model->Animation(1, 1);
			model2->RenderS(DirectGraphics::GetInstance(), pos, scale, degree);



			//model->Render(DirectGraphics::GetInstance(), pos, scale, degree);
			
			//model->RenderSV(DirectGraphics::GetInstance());
			//model->RenderS(DirectGraphics::GetInstance());



			{
			if (KeyInput::GetInstance()->key[DIK_LSHIFT] >= 1) {

				if (KeyInput::GetInstance()->key[DIK_UP] >= 1) {
					degree.Y++;
				}
				if (KeyInput::GetInstance()->key[DIK_DOWN] >= 1) {
					degree.Y--;
				}
				if (KeyInput::GetInstance()->key[DIK_RIGHT] >= 1) {
					degree.X++;
				}
				if (KeyInput::GetInstance()->key[DIK_LEFT] >= 1) {
					degree.X--;
				}
				if (KeyInput::GetInstance()->key[DIK_Z] >= 1) {
					degree.Z--;
				}
			}
			else {
				if (KeyInput::GetInstance()->key[DIK_UP] >= 1) {
					pos.Y += 0.01;
				}
				if (KeyInput::GetInstance()->key[DIK_DOWN] >= 1) {
					pos.Y -= 0.01;
				}
				if (KeyInput::GetInstance()->key[DIK_RIGHT] >= 1) {
					pos.X += 0.01;
				}
				if (KeyInput::GetInstance()->key[DIK_LEFT] >= 1) {
					pos.X -= 0.01;
				}
				if (KeyInput::GetInstance()->key[DIK_Z] >= 1) {
					pos.Z -= 0.01;
				}
				if (KeyInput::GetInstance()->key[DIK_A] >= 1) {
					pos.Z += 0.01;
				}
			}


		}
			DirectGraphics::GetInstance()->FinishRendering();//描画する毎回の処理

		}
	}

	UninitInput();
	delete model;

	DirectGraphics::GetInstance()->Release();

	return 0;
}
