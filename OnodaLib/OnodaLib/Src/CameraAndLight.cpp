#include "CameraAndLight.h"
#include "DXTK11Lib.h"
#include "DirectGraphics.h"
using namespace GameMath;

MainCamera::MainCamera() {
	pos = Vector3(0.0f, 0.0f, 0.0f);
	angle = Vector3(0.0f, 0.0f, 0.0f);

	Pos2 returnPosXZ = GetCirclePoint(pos.X, pos.Z, length, angle.Y);
	Pos2 returnPosYZ = GetCirclePoint(pos.Y, pos.Z * -1, length, angle.X);

	focs = Vector3(returnPosXZ.p[0], returnPosYZ.p[0], returnPosXZ.p[1] * -1);
}
void MainCamera::Updata(bool local) {


	if (local) {
		Pos2 returnPosXZ = GetCirclePoint(pos.X, pos.Z, length, angle.Y);
		Pos2 returnPosYZ = GetCirclePoint(pos.Y, pos.Z * -1, length, angle.X);
		focs = Vector3(returnPosXZ.p[0], returnPosYZ.p[0], returnPosXZ.p[1] * -1);

		Vector3 ang = angle;
		ang.Y = angle.Y * -1;
		DirectGraphics::GetInstance()->SetCameraLocal(pos, focs, Vector3(0, 1, 0), ang);
	}
	else {
		DirectGraphics::GetInstance()->SetCamera(pos, focs, Vector3(0, 1, 0));
	}

}

void MainCamera::Debug(float speed, int strX, int strY) {
	KeyInput* KEY = KeyInput::GetInstance();

	char buf[256];
	WCHAR wbuf[256];
	sprintf_s(buf, sizeof(buf), "POS:X:%.2f:Y:%.2f:Z:%.2f", pos.X, pos.Y, pos.Z);
	mbstowcs_s(0, wbuf, 256, buf, _TRUNCATE);
	DXTK11LIB::DrawString(wbuf, strX + 0, strY + 30, DXTK11LIB::StrColor{ 1,0,0,1 });

	sprintf_s(buf, sizeof(buf), "DEGREE:X:%.2f:Y:%.2f:Z:%.2f", angle.X, angle.Y, angle.Z);
	mbstowcs_s(0, wbuf, 256, buf, _TRUNCATE);
	DXTK11LIB::DrawString(wbuf, strX + 0, strY + 90, DXTK11LIB::StrColor{ 1,0,0,1 });

	//移動
	if (KEY->key[DIK_LSHIFT] >= 1) {
		if (KEY->key[DIK_W] >= 1) {
			pos.Y += speed;
		}
		if (KEY->key[DIK_S] >= 1) {
			pos.Y -= speed;
		}
		if (KEY->key[DIK_D] >= 1) {
			pos.X += speed;
		}
		if (KEY->key[DIK_A] >= 1) {
			pos.X -= speed;
		}
		if (KEY->key[DIK_X] >= 1) {
			pos.Z += speed;
		}
		if (KEY->key[DIK_Z] >= 1) {
			pos.Z -= speed;
		}
		return;
	}
	//回転
	if (KEY->key[DIK_LCONTROL] >= 1) {
		if (KEY->key[DIK_D] >= 1) {

			angle.Y++;
		}
		if (KEY->key[DIK_A] >= 1) {
			angle.Y--;
		}

		if (KEY->key[DIK_W] >= 1) {
			angle.X++;
		}
		if (KEY->key[DIK_S] >= 1) {
			angle.X--;
		}

		if (KEY->key[DIK_X] >= 1) {
			angle.Z++;
		}
		if (KEY->key[DIK_Z] >= 1) {
			angle.Z--;
		}
		return;
	}

}


DirectionalLight::DirectionalLight() {
	length = 10.0f;
	pos = Vector3(0, 0, 0);
	angle= Vector3(0, 0, 0);
	focus= Vector3(0, 0, 0);
	up  = Vector3(0, 0, 0);
	fov=90;
	Vector3 color = Vector3(0, 0, 0);
}
void DirectionalLight::Create() {

	DirectGraphics* graphics = DirectGraphics::GetInstance();
	ID3D11Device* dev = graphics->GetDevice();
	// シャドウ マップの作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = 512 * 2;   // 幅
	descDepth.Height = 512 * 2;  // 高さ
	descDepth.MipLevels = 1;       // ミップマップ レベル数
	descDepth.ArraySize = 1;       // 配列サイズ
	descDepth.Format = DXGI_FORMAT_R32_TYPELESS;  // フォーマット
	descDepth.SampleDesc.Count = graphics->GetMSAA().Count;
	descDepth.SampleDesc.Quality = graphics->GetMSAA().Quality;
	descDepth.Usage = D3D11_USAGE_DEFAULT;      // デフォルト使用法
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE; // 深度/ステンシル、シェーダ リソース ビューとして使用
	descDepth.CPUAccessFlags = 0;   // CPUからはアクセスしない
	descDepth.MiscFlags = 0;   // その他の設定なし
	graphics->GetDevice()->CreateTexture2D(
		&descDepth,         // 作成する2Dテクスチャの設定
		NULL,               // 
		&g_pShadowMap);     // 作成したテクスチャを受け取る変数

	//深度/ステンシル ビューの作成
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;            // ビューのフォーマット

	descDSV.ViewDimension = (graphics->GetAntialias()) ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags = 0;
	descDSV.Texture2D.MipSlice = 0;
	graphics->GetDevice()->CreateDepthStencilView(
		g_pShadowMap,         // 深度/ステンシル・ビューを作るテクスチャ
		&descDSV,             // 深度/ステンシル・ビューの設定
		&g_pShadowMapDSView); // 作成したビューを受け取る変数

	// シェーダ リソース ビューの作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT_R32_FLOAT; // フォーマット
	srDesc.ViewDimension = (graphics->GetAntialias()) ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
	srDesc.Texture2D.MostDetailedMip = 0;   // 最初のミップマップ レベル
	srDesc.Texture2D.MipLevels = -1;  // すべてのミップマップ レベル

	// シェーダ リソース ビューの作成
	graphics->GetDevice()->CreateShaderResourceView(
		g_pShadowMap,          // アクセスするテクスチャ リソース
		&srDesc,               // シェーダ リソース ビューの設定
		&g_pShadowMapSRView);  // 受け取る変数
}

void DirectionalLight::Updata() {

	DirectGraphics* graphics = DirectGraphics::GetInstance();


	DirectX::XMVECTOR light = DirectX::XMVectorSet(pos.X, pos.Y, pos.Z, 0.0f);
	DirectX::XMVECTOR lightColor = DirectX::XMVectorSet(color.X,color.Y,color.Z,1.0);

	XMStoreFloat4(&graphics->m_ConstantBufferData.LightColor, lightColor);
	XMStoreFloat4(&graphics->m_ConstantBufferData.LightVector, light);

	// ライトのカラー設定
	//m_ConstantBufferData.LightColor = DirectX::XMFLOAT4(color.X, color.Y, color.Z, 1);

	// ビュー変換行列(光源から見る)
	DirectX::XMVECTORF32 focusPosition = { focus.X, focus.Y,  focus.Z };
	DirectX::XMVECTORF32 upDirection = { up.X, up.Y,  up.Z };
	DirectX::XMFLOAT3	g_vLightPos(pos.X, pos.Y, pos.Z);   // 光源座標(ワールド座標系)

	matShadowMapView = XMMatrixLookAtLH(DirectX::XMLoadFloat3(&g_vLightPos), focusPosition, upDirection);
	
	//ビュー変換行列セット
	XMStoreFloat4x4(&graphics->m_ConstantBufferData.View, XMMatrixTranspose(matShadowMapView));

	// 射影変換行列セット
	matShadowMapProj = DirectX::XMMatrixPerspectiveFovLH(
	DirectX::XMConvertToRadians(fov),
			1,	// アスペクト比
			0.1f,							// 前方投影面までの距離
			1000.0f);						// 後方投影面までの距離
		XMStoreFloat4x4(&graphics->m_ConstantBufferData.Projection, XMMatrixTranspose(matShadowMapProj));

}
