#pragma warning(disable:C26812)
#pragma warning(disable:C4067)



#pragma comment(lib, "libfbxsdk-mt.lib")
#pragma comment(lib, "zlib-mt.lib")
#pragma comment(lib, "libxml2-mt.lib")

#include "DirectX11Lib.h"

//TODO
/*
�G�b�W�@�@���}�b�v�Ɛ[�x�}�b�v�Ō��o���ďo�����ق�����������

���C�g�̕�������

�����V���h�E�͂���ǂ�

�@�f�B���N�V�������C�g�������g�p���Đ�������iShadowMap�͂P�j
 �@
*/

using namespace DXTK11LIB;
DirectionalLight light;

class TitleScene : public Scene , public ResourceManager{
public:
	void Load();
	void Updata();
	void Draw();
};
void TitleScene::Load() {

	VertexShaderData* vs;
	vs = new VertexShaderData("../x64/Debug/VertexShader.cso");
	SetShaderV("vertex", vs);
	VertexShaderData* vse;
	vse = new VertexShaderData("../x64/Debug/VertexShader_Edge.cso");
	SetShaderV("edge", vse);
	VertexShaderData* vsenv;
	vsenv = new VertexShaderData("../x64/Debug/VertexShader_Enviroment_Surface.cso");
	SetShaderV("envi_V", vsenv);

	VertexShaderData* vsc;
	vsc = new VertexShaderData("../x64/Debug/VertexShader_CubeMap.cso");
	SetShaderV("cubeMap", vsc);

	GeometryShaderData* gs=new GeometryShaderData("../x64/Debug/GeometryShader.cso");
	SetShaderG("geometry",gs);
	GeometryShaderData* gsc;
	gsc = new GeometryShaderData("../x64/Debug/GeometryShader_CubeMap.cso");
	SetShaderG("cubeMap", gsc);


	PixelShaderData* ps = new PixelShaderData("../x64/Debug/PixelShader.cso");
	SetShaderP("pixel", ps);

	PixelShaderData* psTex = new  PixelShaderData("../x64/Debug/PixelShader_TextureColor.cso");
	SetShaderP("texture", psTex);

	PixelShaderData* pse = new PixelShaderData("../x64/Debug/PixelShader_Edge.cso");
	SetShaderP("edge", pse);

	PixelShaderData*  psc = new PixelShaderData("../x64/Debug/PixelShader_CubeMap.cso");
	SetShaderP("cubeMap", psc);

	PixelShaderData* psen = new  PixelShaderData("../x64/Debug/PixelShader_Enviroment.cso");
	SetShaderP("envi_V", psen);
	
	FbxModel* skyDome = new FbxModel;
	skyDome->md.SetShader(*vsc, *gsc, *psc);
	skyDome->md.scale = Vector3(10,10,10)*3;
	skyDome->md.degree = Vector3(-90, 0, 0);

	skyDome->Load("Data/Model/sky.fbx", "Data/Model/Tex/");
	SetModel("skydome", skyDome);

	FbxModel* figther = new FbxModel;
	figther->md.SetShader(*vsc, *gsc, *psc);
	figther->md.scale = Vector3(0.3, 0.3, 0.3);
	figther->md.degree = Vector3(-90,0,0);

	figther->Load("Data/Model/Fighter.fbx", "Data/Model/Tex/");
	SetModel("fighter", figther);


	MainCamera* cam = MainCamera::GetInstance();
	cam->pos.Z = -4;

}
void TitleScene::Updata() {

	MainCamera* cam = MainCamera::GetInstance();
	
	float speed = 0.1f;
	if (mana->KEY->key[DIK_LSHIFT] >= 1) {
		speed = 0.5;
	}
	if (GetModel("fighter")->md.degree.Y >= 360) {
		GetModel("fighter")->md.degree.Y = 0;
	}
	if (mana->KEY->key[DIK_W] >= 1) {
		cam->pos.Z += speed;

		if (GetModel("fighter")->md.degree.Y <= 180 &&
			GetModel("fighter")->md.degree.Y > 1) {
			GetModel("fighter")->md.degree.Y--;
		}
		if (GetModel("fighter")->md.degree.Y > 180 && GetModel("fighter")->md.degree.Y > 1) {
			GetModel("fighter")->md.degree.Y++;
		}


		GetModel("fighter")->md.pos.Z+=speed;
	}
	if (mana->KEY->key[DIK_S] >= 1) {
		if (GetModel("fighter")->md.degree.Y < 180)
			GetModel("fighter")->md.degree.Y++;
		else
			GetModel("fighter")->md.degree.Y--;

		cam->pos.Z -= speed;
		GetModel("fighter")->md.pos.Z-=speed;
	}
	if (mana->KEY->key[DIK_D] >= 1) {
		if (GetModel("fighter")->md.degree.Y < 90)
			GetModel("fighter")->md.degree.Y++;
		else
			GetModel("fighter")->md.degree.Y--;

		cam->pos.X += speed;
		GetModel("fighter")->md.pos.X+=speed;
	}
	if (mana->KEY->key[DIK_A] >= 1) {
		if (GetModel("fighter")->md.degree.Y < 270)
			GetModel("fighter")->md.degree.Y++;
		else
			GetModel("fighter")->md.degree.Y--;

		cam->pos.X -= speed;
		GetModel("fighter")->md.pos.X-=speed;
	}

}
void TitleScene::Draw() {
	MainCamera* cam = MainCamera::GetInstance();

	//

	DirectGraphics::GetInstance()->lightL = &light;
	light.Updata();

	////�L���[�u�}�b�v�p���f���`�揈��
	DirectGraphics::GetInstance()->SetCamera_CM(Vector3(0, 0, 0), 1);
	DirectGraphics::GetInstance()->SetProjection_CM();
	GetModel("skydome")->md.SetShader(*GetShaderV("cubeMap"), *GetShaderG("cubeMap"), *GetShaderP("cubeMap"));
	GetModel("skydome")->RenderCubeMap();


	//�ʏ�̕`�揈��
	//cam->Updata(true);
	Vector3 cameraPos = GetModel("fighter")->md.pos;
	cameraPos.Z -= 3;
	cameraPos.Y += 2;

	DirectGraphics::GetInstance()->SetCamera(cameraPos, GetModel("fighter")->md.pos, Vector3(0, 1, 0));
	DirectGraphics::GetInstance()->SetProjection(45.0f, 0.1f, 1000.0f, true);

	GetModel("skydome")->md.SetShader(*GetShaderV("vertex"), *GetShaderG("geometry"), *GetShaderP("texture"));
	GetModel("skydome")->RenderShadowModel();

	
	GetModel("fighter")->md.SetShader(*GetShaderV("envi_V"), *GetShaderG("geometry"), *GetShaderP("envi_V"));
	GetModel("fighter")->RenderShadowModel();

	GetModel("fighter")->md.DebugMode();
}

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmpLine,
	INT       nCmdShow)
{
	
	int wx = GetSystemMetrics(SM_CXSCREEN);
	int wy = GetSystemMetrics(SM_CYSCREEN);
	SetAntialias(true);//���Ӑ}�I�ɃA���`�G�C���A�X���I�t�ɂł���
	AllInit("Death_T", wx, wy,false, hInstance);
	SetDefaultImageExtend((float)wx / 1920.0);



	MainManager* manager = MainManager::GetInstance();

	TitleScene ts;
	manager->Load(&ts);//�����ŐV�����ق��̃f�[�^�����[�h�����


	light.Create();
	light.fov = 90;//�f�B���N�V�������C�g�͂X�O�x���炢������
	light.pos = Vector3(3, 0, 0);
	light.focus = Vector3(0, 0, 0);
	light.up = Vector3(0, 1, 0);
	light.color = Vector3(1, 1, 1);
	light.Create();
	DirectGraphics::GetInstance()->lightL = &light;

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

			
			

			//syatiku->RenderShadowMap(&md)

			manager->Updata();
			
		

			manager->Draw();

			////�L���[�u�}�b�v�p���f���`�揈��
			//DirectGraphics::GetInstance()->SetCamera_CM(Vector3(0, 0, 0), 1);
			//DirectGraphics::GetInstance()->SetProjection_CM();
			//sky2->RenderCubeMap(&skyMD2);
			//syatiku->RenderCubeMap(&md);

			////�ʏ�̕`�揈��
			//DirectGraphics::GetInstance()->SetCamera(Vector3(0, 0, 5), Vector3(0, 0, 0), Vector3(0, 1, 0));
			//DirectGraphics::GetInstance()->SetProjection(90.0f, 0.1f, 1000.0f, true);
			//
			//sky->RenderShadowModel(&skyMD);
			//syatiku2->RenderShadowModel(&md2);

			////�`�揈��
			////syatiku_E->RenderEdge(&md,Vector3(1.01,1.01,1.01),Vector3(1,0.5,0.5));
			////syatiku->RenderShadowModel(&md2);

			//cube->RenderShadowModel(&cubeMD);//�������܂ꂽ�e�N�X�`���̕`��
			////sphere->RenderShadowModel(&sphereMD);//�������܂ꂽ�e�N�X�`���̕`��
			//
			//
			////cubeMD.DebugMode(0.05);

			//cubeMD.DebugMode(0.05);
			//sphereMD.DebugMode(0.05);
			
			
		//	if (gm->gameEnd|| gm->KEY->key[DIK_ESCAPE]) {

			//	break;
			//}
		}
	}
	manager->AllDeleteLocal();
	manager->AllDeleteGrobal();
	UninitInput();
	DirectGraphics::GetInstance()->Release();

	return 0;
}
