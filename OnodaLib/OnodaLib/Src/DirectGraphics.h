#pragma once

#ifndef DIRECT_GRAPHICS_H_
#define DIRECT_GRAPHICS_H_

#include <map>
#include <string>

#include <d3d11.h>
#include "WICTextureLoader.h"

#include "ShaderBase.h"
#include "GraphicsUtility.h"
#include "CameraAndLight.h"

//=====================================================================//
//! DirectX11の機能を実装するクラス
//=====================================================================//

class DirectionalLight;

class DirectGraphics
{
public:
	static DirectGraphics* GetInstance()
	{
		static DirectGraphics instance;
		return &instance;
	}

public:

	/** Destructor */
	~DirectGraphics() {}
	
	/**
	* @brief 初期化関数@n
	* DirectX11の初期化を行い、成功したらtrue、失敗したらfalseを返す
	* @return 初期化の成否 成功(true)
	*/
	bool Init();

	/**
	* @brief DirectX11の解放関数@n
	* 保持しているデバイスなどを解放する
	*/
	void Release();

	/**
	* @brief 描画開始関数@n
	* 描画処理を行う場合、必ずこの関数の後に実行する@n
	* 実行しないと描画されない
	*/
	void StartRendering();

	/**
	* @brief 描画終了関数@n
	* 描画処理が完了した場合、必ずこの関数を実行する
	* 実行しないと描画内容が反映されない
	*/
	void FinishRendering();
	
	void SetUpContext(Vector3 pos, Vector3 scale, Vector3 degree, Vector3 axis, float axisAngle, Vector3 axisR, VertexShader* m_VertexShader, GeometryShader* m_GeometryShader, PixelShader* m_PixelShader);
	ID3D11Device* GetDevice()
	{
		return m_Device;
	}

	ID3D11Buffer* GetConstantBuffer()
	{
		return m_ConstantBuffer;
	}

	ConstantBuffer* GetConstantBufferData()
	{
		return &m_ConstantBufferData;
	}

	ID3D11DeviceContext* GetContext()
	{
		return m_Context;
	}

	ID3D11BlendState* GetBlendState() {
		return g_pBlendState;
	}


	void SetMaterial(ObjMaterial* material);

	void SetTexture(ID3D11ShaderResourceView* texture);

private:
	/** Constructor */
	DirectGraphics() :
		m_Device(nullptr),
		m_Context(nullptr),
		m_SwapChain(nullptr),
		m_RenderTargetView(nullptr),
		m_DepthStencilTexture(nullptr),
		m_DepthStencilView(nullptr)

	{}

	void SetUpDxgiSwapChainDesc(DXGI_SWAP_CHAIN_DESC* dxgi);

	bool CreateDeviceAndSwapChain();

	bool CreateRenderTargetView();

	bool CreateDepthAndStencilView();

	void CreateBlendState();

	public:
	void SetUpViewPort();
	private:

	bool CreateTextureSampler();

	bool CreateConstantBuffer();

	bool CreateDepthStencil();

	bool CreateRasterizer();

	//シャドウマップのラスタライザ
private:
	bool CreateRastrizerSM();

	bool CreateDepthStencilSM();

	bool CreateTextureSamplerSM();


	bool SetUpViewPortSM();
	
//エッジ用ラスタライザ
private:
	bool CreateRasterizerEdge();

//描画ターゲットになるテクスチャ
private:
	//
	ID3D11Texture2D* viewTexture = NULL;
	ID3D11Texture2D* viewTextureDST;
	ID3D11DepthStencilView* viewTextureDSV = NULL;	
	ID3D11ShaderResourceView* viewTextureSRView = NULL;
	D3D11_VIEWPORT           viewTextureViewPort[1];       
	ID3D11RenderTargetView* viewTextureRTV;
	
	void CreateTexture2D_TV();

	void CreateDepthAndStencilView_TV();

	void CreateShaderResource_TV();

	void SetUpViewPort_TV();

	void CreateRenderTargetView_TV();
private:
	//キューブマップ系統
	ID3D11Texture2D* cubeMap = NULL;
	ID3D11Texture2D* cubeMapDST;
	ID3D11DepthStencilView* cubeMapDSV = NULL;
	
	public: ID3D11ShaderResourceView* cubeMapSRView = NULL;
private:
	D3D11_VIEWPORT           cubeMapViewPort[1];
	ID3D11RenderTargetView* cubeMapRTV;

	public:
	void SetCamera_CM(Vector3 center, float dist);
	void SetProjection_CM();
	private:
	void CreateTexture2D_CM();

	void CreateDepthAndStencilView_CM();

	void CreateShaderResource_CM();

	void SetUpViewPort_CM();

	void CreateRenderTargetView_CM();
public:

	//ワールド行列の設定
	void SetShader(VertexShader* m_VertexShader, GeometryShader* m_GeometryShader, PixelShader* m_PixelShader);

	void SetWorldMatrix(Vector3 pos, Vector3 scale, Vector3 degree, Vector3 axis, float axisAngle, Vector3 axisR);

	void SetCamera(Vector3 pos, Vector3 focs, Vector3 up);

	void SetCameraLocal(Vector3 _pos, Vector3 _focs, Vector3 _up, Vector3 _degree);

	void SetProjection(float _fov, float _nearZ, float _farZ, bool perspective);


	void SetUpContextSM(Vector3 pos, Vector3 scale, Vector3 degree,Vector3 axis,float axisAngle, Vector3 axisR, VertexShader* m_VertexShader);

	void SetUpContextSM2(Vector3 pos, Vector3 scale, Vector3 degree, Vector3 axis, float axisAngle, Vector3 axisR, VertexShader* m_VertexShader, GeometryShader* m_GeometryShader, PixelShader* m_PixelShader);
	void SetTextureSM(ID3D11ShaderResourceView* texture);

	//エッジ描画系統の設定
	void SetUpContextEdge(Vector3 edgeSize, Vector3 edgeColor, Vector3 pos, Vector3 scale, Vector3 degree, Vector3 axis, float axisAngle, Vector3 axisR, VertexShader* m_VertexShader, GeometryShader* m_GeometryShader, PixelShader* m_PixelShader);

	//テクスチャへ描画用の設定
	void SetUpContextTextureView(Vector3 pos, Vector3 scale, Vector3 degree, Vector3 axis, float axisAngle, Vector3 axisR, VertexShader* m_VertexShader, GeometryShader* m_GeometryShader, PixelShader* m_PixelShader);
	
	//キューブマップの描画
	void SetUpContextCubeMap(Vector3 pos, Vector3 scale, Vector3 degree, Vector3 axis, float axisAngle, Vector3 axisR, VertexShader* m_VertexShader, GeometryShader* m_GeometryShader, PixelShader* m_PixelShader);


public:
	//メインで使うやつら
	ID3D11Device* m_Device;												//!< @brief DirectX11のDeviceのInterface
	ID3D11DeviceContext* m_Context;										//!< @brief Context
	IDXGISwapChain* m_SwapChain;										//!< @brief SwapChainのInterface
	ID3D11RenderTargetView* m_RenderTargetView;							//!< @brief RenderTargetViewのInterface
	ID3D11Texture2D* m_DepthStencilTexture;								//!< @brief ID3D11DepthStencilViewを生成するためのテクスチャ
	ID3D11DepthStencilView* m_DepthStencilView;							//!< @brief DepthStencilViewのInterface
	ID3D11SamplerState* m_SamplerState;									//!< @brief Textureサンプラー
	ID3D11Buffer* m_ConstantBuffer;										//!< @brief 定数バッファ
	ID3D11BlendState* g_pBlendState;			// ブレンド・ステート・オブジェクト
	ID3D11DepthStencilState* g_pDepthStencilState;
	ID3D11RasterizerState* g_pRasterizerState;


	ConstantBuffer m_ConstantBufferData;								//!< @brief ConstantBufferデータ
	
public:

	//シャドウマップ 複数のライトで共有できるもの
	ID3D11RasterizerState* g_pRasterizerStateSM;
	ID3D11DepthStencilState* g_pDepthStencilStateSM;
	ID3D11SamplerState* m_SamplerStateSM;
	D3D11_VIEWPORT            g_ViewPortSM[1];       // ビューポート



	
	public:
		DirectionalLight* lightL;
	
private:
									
	


	//裏面描画用ラスタライザ
public:
	ID3D11RasterizerState* g_pRasterizerStateEdge;

	
private:
	DXGI_SAMPLE_DESC MSAA;
	bool antialias = true;
	int shadowMapSize = 512;
public:
	void SetAntialias(bool on);
	bool GetAntialias() { return antialias; };
	void SetShadowMapSize(int size) { shadowMapSize = size; }
	int GetShadowMapSize() { return shadowMapSize; }

	DXGI_SAMPLE_DESC GetMSAA() { return MSAA; }
};

#endif