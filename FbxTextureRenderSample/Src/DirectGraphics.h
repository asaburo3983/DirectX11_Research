#ifndef DIRECT_GRAPHICS_H_
#define DIRECT_GRAPHICS_H_

#include <map>
#include <string>

#include <d3d11.h>
#include "WICTextureLoader.h"
#include "VertexShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"

#include "GraphicsUtility.h"

//=====================================================================//
//! DirectX11の機能を実装するクラス
//=====================================================================//
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
	
	void SetUpContext(Vector3 pos, Vector3 scale, Vector3 degree,VertexShader* m_VertexShader,GeometryShader* m_GeometryShader, PixelShader* m_PixelShader);

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

	/**
	* @brief DXGI_SWAP_CHAIN_DESCの設定関数@n
	* SwapChainを作成するうえで必要な設定をDXGI_SWAP_CHAIN_DESCに行う
	* @param[out] dxgi 設定を行うDXGI_SWAP_CHAIN_DESCのポインタ
	*/
	void SetUpDxgiSwapChainDesc(DXGI_SWAP_CHAIN_DESC* dxgi);

	/**
	* @brief DeviceとSwapChainの作成関数@n
	* DirectX11のDeviceとSwapChainを作成する@n
	* まとめた理由は生成関数をD3D11CreateDeviceAndSwapChainにしたため
	* @return 作成の成否 成功(true)
	*/
	bool CreateDeviceAndSwapChain();

	/**
	* @brief RenderTargetViewの作成関数@n
	* DirectX11のRenderTargetViewを作成する
	* @return 作成の成否 成功(true)
	*/
	bool CreateRenderTargetView();

	/**
	* @brief DepthバッファStencilバッファ作成関数
	* @return 作成の成否 成功(true)
	*/
	bool CreateDepthAndStencilView();

	void CreateBlendState();

	/**
	* @brief ViewPort設定関数@n
	* ContextにViewPortの設定を行う関数@n
	* ゲーム中に変更がなければ最初に１度行えば問題ない
	*/
	void SetUpViewPort();

	/**
	* @brief TextureSampler作成関数@n
	* TextureSamplerの設定を行い、ID3D11SamplerStateを作成する
	* @return 作成の成否 成功(true)
	*/
	bool CreateTextureSampler();

	/**
	* @brief ConstantBuffer作成関数@n
	* TextureSamplerの設定を行い、ID3D11SamplerStateを作成する
	* @return 作成の成否 成功(true)
	*/
	bool CreateConstantBuffer();

	bool CreateDepthStencil();

	bool CreateRasterizer();

	//シャドウマップのラスタライザ
private:
	bool CreateRastrizerSM();

	bool CreateDepthStencilSM();

	bool CreateTextureSamplerSM();

	bool CreateTexture2DSM();

	bool CreateDepthStencilViewSM();

	bool CreateShaderResourceSM();

	bool SetUpViewPortSM();

public:

	//ワールド行列の設定
	void SetShader(VertexShader* m_VertexShader, GeometryShader* m_GeometryShader, PixelShader* m_PixelShader);

	void SetWorldMatrix(Vector3 pos, Vector3 scale, Vector3 degree);
	//カメラや視野の設定
	void SetCamera(Vector3 pos, Vector3 focs, Vector3 up);
	void SetProjection(float _fov, float _nearZ, float _farZ, bool perspective);

	//
	void SetLight(Vector3 pos, Vector3 focus, Vector3 up, float _fov, Vector3 color, bool shadowMap);
	//影系統の設定


	void SetUpContextSM(Vector3 pos, Vector3 scale, Vector3 degree, VertexShader* m_VertexShader, Vector3 lightPos, Vector3 lightFocus, Vector3 lightUp, float lightFov);

	void SetUpContextSM2(Vector3 pos, Vector3 scale, Vector3 degree, VertexShader* m_VertexShader, GeometryShader* m_GeometryShader, PixelShader* m_PixelShader);
	void SetTextureSM(ID3D11ShaderResourceView* texture);
public:
	ID3D11Device* m_Device;												//!< @brief DirectX11のDeviceのInterface
	ID3D11DeviceContext* m_Context;										//!< @brief Context
	IDXGISwapChain* m_SwapChain;										//!< @brief SwapChainのInterface
	ID3D11RenderTargetView* m_RenderTargetView;							//!< @brief RenderTargetViewのInterface
	ID3D11Texture2D* m_DepthStencilTexture;								//!< @brief ID3D11DepthStencilViewを生成するためのテクスチャ
	ID3D11DepthStencilView* m_DepthStencilView;							//!< @brief DepthStencilViewのInterface
	ID3D11SamplerState* m_SamplerState;									//!< @brief Textureサンプラー
	ID3D11Buffer* m_ConstantBuffer;										//!< @brief 定数バッファ
	ConstantBuffer m_ConstantBufferData;								//!< @brief ConstantBufferデータ

	ID3D11BlendState* g_pBlendState;			// ブレンド・ステート・オブジェクト
	ID3D11DepthStencilState* g_pDepthStencilState;
	ID3D11RasterizerState* g_pRasterizerState;

	//シャドウマップ
public:
	ID3D11RasterizerState* g_pRasterizerStateSM;

	ID3D11DepthStencilState* g_pDepthStencilStateSM;
private:
	ID3D11SamplerState* m_SamplerStateSM;									//!< @brief Textureサンプラー

	ID3D11Texture2D* g_pShadowMap = NULL;
	ID3D11DepthStencilView*   g_pShadowMapDSView = NULL;	// 深度/ステンシル・ビュー
	ID3D11ShaderResourceView* g_pShadowMapSRView = NULL;	// シェーダ・リソース・ビュー
	D3D11_VIEWPORT            g_ViewPortSM[1];       // ビューポート
};

#endif
