#include "DirectX11Lib.h"


using namespace DXTK11LIB;


int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmpLine,
	INT       nCmdShow)
{

	int wx = GetSystemMetrics(SM_CXSCREEN);
	int wy = GetSystemMetrics(SM_CYSCREEN);
	AllInit("UNT", 640, 480, hInstance);

	//SetDefaultImageExtend((float)wx / 1920.0);

	Image im;
	im.Load("test.png");
	im.LoadShader("../x64/Debug/PixelShader_2D.cso");

	float time = 0.0f;


	FBX_MODEL* model = new FBX_MODEL;

	model->Load("Res/Model/2R2.fbx",
		"Res/Texture/",
		"../x64/Debug/VertexShader_Live2D.cso",
		"../x64/Debug/GeometryShader.cso",
		"../x64/Debug/PixelShader.cso");

	//3D—p
	Vector3 pos = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 degree = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);

	CAMERA cam;

	scale = scale / 200;

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

			time += 0.001f;
			UpdateInput();//“ü—Íˆ—XV	
			DirectGraphics::GetInstance()->StartRendering();

			DirectGraphics::GetInstance()->SetLight(Vector3(5, 0, 0), Vector3(0, 0, 0), Vector3(0, 1, 0), 45, Vector3(1, 1, 1), true);
			model->RenderS_SM(pos, scale, degree);

			cam.pos = Vector3(0, 0, -4);
			cam.Updata();

			//DirectGraphics::GetInstance()->SetCamera(cameraPos, Vector3(0, 0, 0), Vector3(0, 1, 0));
			DirectGraphics::GetInstance()->SetProjection(45.0f, 0.1f, 1000.0f, true);

			model->Animation(0, 1);
			model->RenderS(pos, scale, degree);

			im.m_ConstantBufferData.CameraPos.x = GameMath::GetRand(0, 1000);
			im.m_ConstantBufferData.CameraPos.y = GameMath::GetRand(0, 1000);
			im.DrawEX(320, 240, 0, 1, 1, 1, true, false, false, true ,RECT{0,0,400,400});



	
			DirectGraphics::GetInstance()->FinishRendering();//•`‰æ‚·‚é–ˆ‰ñ‚Ìˆ—

		}
	}

	UninitInput();
	DirectGraphics::GetInstance()->Release();

	return 0;
}
