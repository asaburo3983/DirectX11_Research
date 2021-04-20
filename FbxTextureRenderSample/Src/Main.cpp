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
