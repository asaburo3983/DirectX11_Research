#pragma once

#include "input.h"
#include "GameMathLib.h"
#include "DirectGraphics.h"
class DirectionalLight {
public:

	float length;
	Vector3 pos;
	Vector3 angle;
	Vector3 focus;
	Vector3 up;
	float fov;
	Vector3 color;

	DirectX::XMMATRIX matShadowMapView;
	DirectX::XMMATRIX matShadowMapProj;
	ID3D11Texture2D* g_pShadowMap;
	ID3D11ShaderResourceView* g_pShadowMapSRView ;
	ID3D11DepthStencilView* g_pShadowMapDSView;	// 深度/ステンシル・ビュー
public:
	DirectionalLight();
	void Create();
	void Updata();
};

class MainCamera {

public:
	static MainCamera* GetInstance() {
		static MainCamera cam;
		return &cam;
	}
private:

	const float length = 10.0f;
public:
	Vector3 pos;
	Vector3 angle;
	Vector3 focs;
public:
	MainCamera();
	void Updata(bool local);
	void Debug(float speed = 1, int strX = 0, int strY = 0);
};
