#include "EffekseerLib.h"
#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }	// 解放マクロ

EffekseerRendererDX11::Renderer* renderer;
Effekseer::Manager* manager;
Effekseer::Effect* effect;
Effekseer::Handle handle = 0;

ID3D11Device* tmpDevice;
ID3D11DeviceContext* tmpDeviceCon;
void EffekseerInit() {
	// エフェクトのレンダラーの作成
	tmpDevice=DirectGraphics::GetInstance()->GetDevice();
	tmpDeviceCon=DirectGraphics::GetInstance()->GetContext();
	renderer = ::EffekseerRendererDX11::Renderer::Create(tmpDevice, tmpDeviceCon, 8000);
	// エフェクトのマネージャーの作成
	manager = ::Effekseer::Manager::Create(8000);
	// 描画モジュールの設定
	manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
	manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
	manager->SetRingRenderer(renderer->CreateRingRenderer());
	manager->SetTrackRenderer(renderer->CreateTrackRenderer());
	manager->SetModelRenderer(renderer->CreateModelRenderer());
	// テクスチャ、モデル、マテリアルローダーの設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	manager->SetTextureLoader(renderer->CreateTextureLoader());
	manager->SetModelLoader(renderer->CreateModelLoader());
	manager->SetMaterialLoader(renderer->CreateMaterialLoader());

	// Specify a position of view
	// 視点位置を確定
	auto g_position = ::Effekseer::Vector3D(0.0f, 0.0f, 20.0f);

	// Specify a projection matrix
	// 投影行列を設定
	renderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovLH(
		90.0f / 180.0f * 3.14f, (float)640 / (float)480, 0.1f, 1000.0f));

	// Specify a camera matrix
	// カメラ行列を設定
	renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(g_position, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));

	// エフェクトの読込
	char16_t s16[] = u"Laser.efk";
	effect = Effekseer::Effect::Create(manager, s16);

	int32_t time = 0;

}

int time = 0;
void EffekseerDraw() {


	if (time % 120 == 0)
	{
		// Play an effect
		// エフェクトの再生
		handle = manager->Play(effect, 0, 0, 0);
	}

	if (time % 120 == 119)
	{
		// Stop effects
		// エフェクトの停止
		manager->StopEffect(handle);
	}
	manager->AddLocation(handle, Effekseer::Vector3D(0.2f, 0.0f, 0.0f));
	// Update the manager
	// マネージャーの更新
	manager->Update();

	// Begin to rendering effects
	// エフェクトの描画開始処理を行う。
	renderer->BeginRendering();

	// Render effects
	// エフェクトの描画を行う。
	manager->Draw();

	// Finish to rendering effects
	// エフェクトの描画終了処理を行う。
	renderer->EndRendering();

	time++;
}

void EffekseerUnInit() {
	renderer->Destroy();
	// Release effects
// エフェクトの解放
	SAFE_RELEASE(effect);

	// Dispose the manager
	// マネージャーの破棄
	manager->Destroy();
}