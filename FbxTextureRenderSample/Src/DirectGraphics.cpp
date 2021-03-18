#include <stdio.h>
#include <atlstr.h>
#include <tchar.h>
#include <DirectXMath.h>
#include "DirectGraphics.h"
#include "ShaderBase.h"
#include "Window.h"
#include "GraphicsUtility.h"
#include "input.h"
#pragma comment(lib,"d3d11.lib")


DXGI_SAMPLE_DESC MSAA;

bool DirectGraphics::Init()
{

	CreateDeviceAndSwapChain();

	CreateConstantBuffer();

	CreateBlendState();

	CreateTextureSampler();

	CreateRenderTargetView();

	CreateDepthAndStencilView();

	SetUpViewPort();

	CreateDepthStencil();

	CreateRasterizer();


	CreateRastrizerSM();

	CreateDepthStencilSM();

	CreateTextureSamplerSM();

	//ライト１につき１つ必要なので動的確保が好ましい
	//下４行
	CreateTexture2DSM();

	CreateDepthStencilViewSM();

	CreateShaderResourceSM();

	SetUpViewPortSM();



	
	return true;
}

void DirectGraphics::SetShader(VertexShader* m_VertexShader, GeometryShader* m_GeometryShader, PixelShader* m_PixelShader) {
	if (m_VertexShader != nullptr)
		m_Context->VSSetShader(m_VertexShader->GetShaderInterface(), nullptr, 0);
	else
		m_Context->VSSetShader(nullptr, nullptr, 0);

	if (m_GeometryShader != nullptr)
		m_Context->GSSetShader(m_GeometryShader->GetShaderInterface(), nullptr, 0);
	else
		m_Context->GSSetShader(nullptr, nullptr, 0);

	if (m_PixelShader != nullptr)
		m_Context->PSSetShader(m_PixelShader->GetShaderInterface(), nullptr, 0);
	else
		m_Context->PSSetShader(nullptr, nullptr, 0);
}
void DirectGraphics::SetWorldMatrix(Vector3 pos, Vector3 scale, Vector3 degree) {
	// ワールドマトリクス設定
	DirectX::XMMATRIX world_matrix;
	DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(pos.X, pos.Y, pos.Z);
	DirectX::XMMATRIX rotate_x = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(degree.X));
	DirectX::XMMATRIX rotate_y = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(degree.Y));
	DirectX::XMMATRIX rotate_z = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(degree.Z));
	DirectX::XMMATRIX scale_mat = DirectX::XMMatrixScaling(scale.X, scale.Y, scale.Z);

	world_matrix = scale_mat * rotate_x * rotate_y * rotate_z * translate;

	XMStoreFloat4x4(&m_ConstantBufferData.World, XMMatrixTranspose(world_matrix));

}
void DirectGraphics::SetCamera(Vector3 _pos, Vector3 _focs, Vector3 _up) {
	// Viewマトリクス設定
	DirectX::XMVECTOR eye = DirectX::XMVectorSet(_pos.X, _pos.Y, _pos.Z, 0.0f);
	DirectX::XMVECTOR focus = DirectX::XMVectorSet(_focs.X, _focs.Y, _focs.Z, 0.0f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(_up.X, _up.Y, _up.Z, 0.0f);
	DirectX::XMMATRIX view_matrix = DirectX::XMMatrixLookAtLH(eye, focus, up);

	XMStoreFloat4x4(&m_ConstantBufferData.View, XMMatrixTranspose(view_matrix));
	XMStoreFloat4(&m_ConstantBufferData.CameraPos, eye);




}
void DirectGraphics::SetProjection(float _fov, float _nearZ, float _farZ, bool perspective) {
	HWND window_handle = FindWindow(Window::ClassName, nullptr);
	RECT rect;
	GetClientRect(window_handle, &rect);
	// プロジェクションマトリクス設定
	float    fov = DirectX::XMConvertToRadians(_fov);
	float    aspect = (float)(rect.right - rect.left) / (rect.bottom - rect.top);
	float    nearZ = _nearZ;
	float    farZ = _farZ;

	DirectX::XMMATRIX proj_matrix;

	//射影、並行の選択
	if (perspective)
		proj_matrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);
	else
		proj_matrix = DirectX::XMMatrixOrthographicLH(rect.right - rect.left, rect.bottom - rect.top, nearZ, farZ);

	XMStoreFloat4x4(&m_ConstantBufferData.Projection, XMMatrixTranspose(proj_matrix));



}


void DirectGraphics::SetLight(Vector3 pos, Vector3 focus, Vector3 up, float _fov, Vector3 color, bool shadowMap) {

	DirectX::XMVECTOR light = DirectX::XMVectorSet(pos.X, pos.Y, pos.Z, 0.0f);

	XMStoreFloat4(&m_ConstantBufferData.LightVector, light);

	// ライトのカラー設定
	m_ConstantBufferData.LightColor = DirectX::XMFLOAT4(color.X, color.Y, color.Z, 1);

	if (shadowMap) {

		// ビュー変換行列(光源から見る)
		DirectX::XMVECTORF32 focusPosition = { focus.X, focus.Y,  focus.Z };
		DirectX::XMVECTORF32 upDirection = { up.X, up.Y,  up.Z };
		DirectX::XMFLOAT3	g_vLightPos(pos.X, pos.Y, pos.Z);   // 光源座標(ワールド座標系)

		DirectX::XMMATRIX matShadowMapView = XMMatrixLookAtLH(DirectX::XMLoadFloat3(&g_vLightPos), focusPosition, upDirection);
		XMStoreFloat4x4(&m_ConstantBufferData.View, XMMatrixTranspose(matShadowMapView));

		// 射影変換行列
		DirectX::XMMATRIX matShadowMapProj = DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XMConvertToRadians(_fov),		// 視野角45°
			g_ViewPortSM[0].Width / g_ViewPortSM[0].Height,	// アスペクト比
			0.1f,							// 前方投影面までの距離
			1000.0f);						// 後方投影面までの距離
		XMStoreFloat4x4(&m_ConstantBufferData.Projection, XMMatrixTranspose(matShadowMapProj));


	}
}


void DirectGraphics::Release()
{
	//シャドウマップ解放

	if (g_pRasterizerStateSM != nullptr) {
		g_pRasterizerStateSM->Release();
		g_pRasterizerStateSM = nullptr;
	}

	if (g_pDepthStencilStateSM != nullptr) {
		g_pDepthStencilStateSM->Release();
		g_pDepthStencilStateSM = nullptr;
	}


	if (m_SamplerStateSM != nullptr) {
		m_SamplerStateSM->Release();
		m_SamplerStateSM = nullptr;
	}
	if (g_pShadowMap != nullptr) {
		g_pShadowMap->Release();
		g_pShadowMap = nullptr;
	}
	if ( g_pShadowMapDSView != nullptr) {
		 g_pShadowMapDSView->Release();
		 g_pShadowMapDSView = nullptr;
	}
	if (g_pShadowMapSRView != nullptr) {
		g_pShadowMapSRView->Release();
		g_pShadowMapSRView = nullptr;
	}
	//////////////////////////////////

	if (m_SamplerState != nullptr) {
		m_SamplerState->Release();
		m_SamplerState = nullptr;
	}


	if (g_pRasterizerState != nullptr) {
		g_pRasterizerState->Release();
		g_pRasterizerState = nullptr;
	}
	
	if (g_pDepthStencilState != nullptr) {
		//delete g_pDepthStencilState;
		g_pDepthStencilState->Release();
		g_pDepthStencilState = nullptr;
	}
	if (g_pBlendState != nullptr) {
		delete g_pBlendState;
		g_pBlendState = nullptr;
	}

	if (m_Context != nullptr)
	{
		m_Context->ClearState();
		m_Context->Flush();
		m_Context->Release();
		m_Context = nullptr;
	}

	if (m_DepthStencilView != nullptr)
	{
		m_DepthStencilView->Release();
		m_DepthStencilView = nullptr;
	}

	if (m_RenderTargetView != nullptr)
	{
		m_RenderTargetView->Release();
		m_RenderTargetView = nullptr;
	}

	if (m_SwapChain != nullptr)
	{
		m_SwapChain->Release();
		m_SwapChain = nullptr;
	}

	if (m_Device != nullptr)
	{
		m_Device->Release();
		m_Device = nullptr;
	}
}

void DirectGraphics::StartRendering()
{
	float clear_color[ 4 ] = { 0.0f, 1.0f, 0.0f, 1.0f };	// RenderTarget塗りつぶしカラー(RGBA)

	// DirecX9ではRenderTargetとDepth、Stencilバッファのクリアは別々にする
    m_Context->ClearRenderTargetView( 
				m_RenderTargetView,							// クリア対象のView
				clear_color );								// クリアカラー

    m_Context->ClearDepthStencilView( 
				m_DepthStencilView,							// クリア対象のView	
				D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,	// クリアフラグ(今回はDepth、Stencilともにクリア)
				1.0f,										// 深度クリア値
				0 );										// ステンシルクリア値

	//SetUpContext(nullptr, nullptr, nullptr,nullptr,nullptr,nullptr);
}

void DirectGraphics::FinishRendering()
{
	// 描画コマンドを送信する
	m_SwapChain->Present(
			/*
				垂直同期のタイミング
					0なら即時描画
					1以上なら指定した値の垂直同期後に描画
			*/
			1,
			/*
				出力オプション
					フレーム出力を行うためのオプションで
					基本は0で問題ないと思われる
					その他のフラグはDXGI_PRESENTで確認可能
			*/
			0 );	// 出力オプション
}


void DirectGraphics::SetMaterial(ObjMaterial* material)
{
	m_ConstantBufferData.MaterialAmbient = DirectX::XMFLOAT4(material->Ambient[0], material->Ambient[1], material->Ambient[2], 1);
	m_ConstantBufferData.MaterialDiffuse = DirectX::XMFLOAT4(material->Diffuse[0], material->Diffuse[1], material->Diffuse[2], 1);
	m_ConstantBufferData.MaterialSpecular = DirectX::XMFLOAT4(material->Specular[0], material->Specular[1], material->Specular[2], 1);
}


void DirectGraphics::SetTexture(ID3D11ShaderResourceView* texture)
{
	// Samplerの設定
	m_Context->PSSetSamplers(
		0,					// スロット番号
		1,					// サンプラーの数
		&m_SamplerState);	// ID3D11SamplerState
	
	// PixelShaderで使用するテクスチャの設定
	m_Context->PSSetShaderResources(
		0,								// スロット番号
		1,								// リソースの数
		&texture);						// ID3D11ShaderResourceView

}


void DirectGraphics::SetUpContext(Vector3 pos, Vector3 scale, Vector3 degree,VertexShader* m_VertexShader ,GeometryShader* m_GeometryShader,PixelShader* m_PixelShader)
{


	//Renderのほうでやったほうがいいかも
	m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//ブレンドステート設定
	float blendFactor[4] = { D3D11_BLEND_ZERO,D3D11_BLEND_ZERO,D3D11_BLEND_ZERO,D3D11_BLEND_ZERO };
	m_Context->OMSetBlendState(g_pBlendState, blendFactor, 0xffffffff);

	//ビューポートの設定
	SetUpViewPort();

	//シェーダの設定
	SetShader(m_VertexShader, m_GeometryShader, m_PixelShader);

	//ワールド行列の設定
	SetWorldMatrix(pos, scale, degree);

	// OMに深度/ステンシル・ステート・オブジェクトを設定
	m_Context->OMSetDepthStencilState(g_pDepthStencilState, 0);

	m_Context->RSSetState(g_pRasterizerState);
	
	// (OutputManger)RnderTagetの指定
	m_Context->OMSetRenderTargets(1,&m_RenderTargetView,m_DepthStencilView);


	// コンスタントバッファ更新　map unmap　と同じ、map unmapのほうが早いかも
	m_Context->UpdateSubresource(m_ConstantBuffer, 0, NULL, &m_ConstantBufferData, 0, 0);

	// コンテキストにコンスタントバッファを設定
	m_Context->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	m_Context->GSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	m_Context->PSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	
}

void DirectGraphics::SetUpDxgiSwapChainDesc(DXGI_SWAP_CHAIN_DESC *dxgi)
{
	HWND window_handle = FindWindow(Window::ClassName, nullptr);
	RECT rect;
	GetClientRect(window_handle, &rect);

	/*
		DirectX11版PresentationParameter
			バッファの数やサイズ、カラーフォーマット等を設定する
	*/
	ZeroMemory(dxgi, sizeof(DXGI_SWAP_CHAIN_DESC));
	dxgi->BufferCount = 3;									// バッファの数
	dxgi->BufferDesc.Width = (rect.right - rect.left);		// バッファの横幅
	dxgi->BufferDesc.Height = (rect.bottom - rect.top);		// バッファの縦幅
	dxgi->BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// カラーフォーマット
	dxgi->BufferDesc.RefreshRate.Numerator = 60;			// リフレッシュレートの分母
	dxgi->BufferDesc.RefreshRate.Denominator = 1;			// リフレッシュレートの分子
	dxgi->BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;

	
	dxgi->BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;

	dxgi->BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// バッファの使い方 Usage => 使用方法
	dxgi->OutputWindow = window_handle;						// 出力対象のウィンドウハンドル
	dxgi->SampleDesc.Count = MSAA.Count;								// マルチサンプリングのサンプル数(未使用は1)
	dxgi->SampleDesc.Quality = MSAA.Quality;							// マルチサンプリングの品質(未使用は0)

	dxgi->Windowed = true;									// ウィンドウモード指定

	dxgi->Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // モード自動切り替え

}

bool DirectGraphics::CreateDeviceAndSwapChain()
{

	HRESULT hr;
	ID3D11Device* hpDevice = NULL;
	ID3D11DeviceContext* hpDeviceContext = NULL;

	HWND window_handle = FindWindow(Window::ClassName, nullptr);

	hr = D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&hpDevice,
		NULL,
		&hpDeviceContext);


	//使用可能なMSAAを取得

	for (int i = 0; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i++) {
		UINT Quality;
		if SUCCEEDED(hpDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, i, &Quality)) {
			if (0 < Quality) {
				MSAA.Count = i;
				MSAA.Quality = Quality - 1;
			}
		}
	}
	//インターフェース取得
	IDXGIDevice1* hpDXGI = NULL;
	hpDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&hpDXGI);

	//アダプター取得
	IDXGIAdapter* hpAdapter = NULL;
	hpDXGI->GetAdapter(&hpAdapter);

	//ファクトリー取得
	IDXGIFactory* hpDXGIFactory = NULL;
	hpAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&hpDXGIFactory);

	//スワップチェイン作成
	// スワップチェイン作成
	
	RECT rect;
	GetClientRect(window_handle, &rect);
	IDXGISwapChain* hpDXGISwpChain = NULL;
	DXGI_SWAP_CHAIN_DESC hDXGISwapChainDesc;
	hDXGISwapChainDesc.BufferDesc.Width = rect.right;
	hDXGISwapChainDesc.BufferDesc.Height = rect.bottom;
	hDXGISwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	hDXGISwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	hDXGISwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	hDXGISwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	hDXGISwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	hDXGISwapChainDesc.SampleDesc.Count = MSAA.Count;
	hDXGISwapChainDesc.SampleDesc.Quality = MSAA.Quality;

	hDXGISwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	hDXGISwapChainDesc.BufferCount = 1;
	hDXGISwapChainDesc.OutputWindow = window_handle;//直接つなぐ必要があるかも
	hDXGISwapChainDesc.Windowed = TRUE;
	hDXGISwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	hDXGISwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	hpDXGIFactory->CreateSwapChain(hpDevice, &hDXGISwapChainDesc, &hpDXGISwpChain);
	hpDXGIFactory->Release();
	hpAdapter->Release();
	hpDXGI->Release();

	DXGI_SWAP_CHAIN_DESC dxgi;
	SetUpDxgiSwapChainDesc(&dxgi);

	m_Device = hpDevice;
	m_Context = hpDeviceContext;
	m_SwapChain = hpDXGISwpChain;


	return true;




	//DXGI_SWAP_CHAIN_DESC dxgi;
	SetUpDxgiSwapChainDesc(&dxgi);

	D3D_FEATURE_LEVEL level;
	// デバイス生成とスワップチェーン作成を行う
	if (FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,					// ビデオアダプタ指定(nullptrは既定)
		D3D_DRIVER_TYPE_HARDWARE,	// ドライバのタイプ
		nullptr,					// D3D_DRIVER_TYPE_SOFTWARE指定時に使用
		0,							// フラグ指定
		nullptr,					// D3D_FEATURE_LEVEL指定で自分で定義した配列を指定可能
		0,							// 上のD3D_FEATURE_LEVEL配列の要素数
		D3D11_SDK_VERSION,			// SDKバージョン
		&dxgi,						// DXGI_SWAP_CHAIN_DESC
		&m_SwapChain,				// 関数成功時のSwapChainの出力先 
		&m_Device,					// 関数成功時のDeviceの出力先
		&level,						// 成功したD3D_FEATURE_LEVELの出力先
		&m_Context)))				// 関数成功時のContextの出力先
	{
		return false;
	}

	return true;
}


bool DirectGraphics::CreateRenderTargetView()
{
	// RenderTargetViewの対象となるBufferの取得
	ID3D11Texture2D* back_buffer;
	if (FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer)))
	{
		return false;
	}
	

	// BufferからRenderTargetViewの作成
	if (FAILED(m_Device->CreateRenderTargetView(back_buffer, NULL, &m_RenderTargetView)))
	{
		return false;
	}
	
	// Targetの取得終わったのでBufferを解放
	back_buffer->Release();

	return true;
}

bool DirectGraphics::CreateDepthAndStencilView()
{

	HWND window_handle = FindWindow(Window::ClassName, nullptr);
	RECT rect;
	GetClientRect(window_handle, &rect);

	//深度ステンシルバッファ作成
    D3D11_TEXTURE2D_DESC texture_desc;
    ZeroMemory( &texture_desc, sizeof( D3D11_TEXTURE2D_DESC ) );
   texture_desc.Width              = (rect.right - rect.left);			// 横幅
   texture_desc.Height             = (rect.bottom - rect.top);			// 縦幅
    texture_desc.MipLevels          = 1;
    texture_desc.ArraySize          = 1;
	//texture_desc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;	// テクスチャーフォーマット
	texture_desc.Format = DXGI_FORMAT_D32_FLOAT;  // フォーマット(深度のみ)
	texture_desc.SampleDesc.Count = MSAA.Count;
	texture_desc.SampleDesc.Quality = MSAA.Quality;

    texture_desc.Usage              = D3D11_USAGE_DEFAULT;				// テクスチャの使用方法(デフォルト)
    texture_desc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;			// Bind設定はDepth、Stencilに設定
    texture_desc.CPUAccessFlags     = 0;
    texture_desc.MiscFlags          = 0;

	// texture_descの情報でテクスチャを作成
	if (FAILED(m_Device->CreateTexture2D(&texture_desc, NULL, &m_DepthStencilTexture)))
	{
		return false;
	}

	// Depth、StencilViewの設定
	// DepthStencilView 

    D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
    ZeroMemory( &dsv_desc, sizeof( dsv_desc ) );
    dsv_desc.Format             = texture_desc.Format;				// Viewのフォーマット(Textureのものを使用)
    dsv_desc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2DMS;	
    dsv_desc.Texture2D.MipSlice = 0;								// 最初に使用するミップマップのレベルを指定 
	dsv_desc.Flags = 0;

	// CreateTexture2Dとdsv_descからDepthとStencilバッファを作る
	if (FAILED(m_Device->CreateDepthStencilView(
				m_DepthStencilTexture,				// DSVとして使用されるTexture
				&dsv_desc,							// DSVの設定
				&m_DepthStencilView)))				// ID3D11DepthStencilViewの出力先
	{
		return false;
	}




	return true;
}

void DirectGraphics::CreateBlendState() {
	D3D11_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
	BlendState.AlphaToCoverageEnable = TRUE;
	BlendState.IndependentBlendEnable = TRUE;
	BlendState.RenderTarget[0].BlendEnable = TRUE;
	BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_Device->CreateBlendState(&BlendState, &g_pBlendState);

	
}


void DirectGraphics::SetUpViewPort()
{
 	HWND window_handle = FindWindow(Window::ClassName, nullptr);
	RECT rect;
	GetClientRect(window_handle, &rect);

	//ビューポートの設定
	D3D11_VIEWPORT view_port;
	view_port.TopLeftX = 0;								// 左上X座標
	view_port.TopLeftY = 0;								// 左上Y座標
	view_port.Width = (float)(rect.right - rect.left);	// 横幅
	view_port.Height = (float)(rect.bottom - rect.top);	// 縦幅

	view_port.MinDepth = 0.0f;							// 最小深度
	view_port.MaxDepth = 1.0f;							// 最大深度

	m_Context->RSSetViewports(	
		1,					// 設定するビューポートの数
		&view_port );		// 設定するビューポート情報のポインタ
}

bool DirectGraphics::CreateTextureSampler()
{

	D3D11_SAMPLER_DESC sampler_desc;

	ZeroMemory(&sampler_desc, sizeof(D3D11_SAMPLER_DESC));
	sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
	// UVW値が0.0～1.0の範囲外になった場合の対応設定
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MipLODBias = 0.0f;
	sampler_desc.MaxAnisotropy = 2;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampler_desc.BorderColor[0] = 0.0f;
	sampler_desc.BorderColor[1] = 0.0f;
	sampler_desc.BorderColor[2] = 0.0f;
	sampler_desc.BorderColor[3] = 0.0f;
	sampler_desc.MinLOD = -FLT_MAX;
	sampler_desc.MaxLOD = FLT_MAX;

	if (FAILED(m_Device->CreateSamplerState(&sampler_desc, &m_SamplerState)))
	{
		return false;
	}
	return true;
}

bool DirectGraphics::CreateConstantBuffer()
{
	/*
		Constantバッファー作成
			コンスタントバッファーはCPU側のデータを
			まとめてGPU側に送信するためのバッファー

			バッファーには座標変換行列などを設定する
	*/
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = sizeof(ConstantBuffer);
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = 0 ;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;

	if (FAILED(m_Device->CreateBuffer(&buffer_desc, nullptr, &m_ConstantBuffer)))
	{
		return false;
	}

	return true;
}


bool DirectGraphics::CreateDepthStencil() {



	//これがないと２Dと３Dの同時描画の際に３DのZバッファが狂う
	// **********************************************************
	// 深度/ステンシル・ステート・オブジェクトの作成 
	D3D11_DEPTH_STENCIL_DESC DepthStencil;
	DepthStencil.DepthEnable = TRUE; // 深度テストあり
	DepthStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // 書き込む
	DepthStencil.DepthFunc = D3D11_COMPARISON_LESS; // 手前の物体を描画
	DepthStencil.StencilEnable = FALSE; // ステンシル・テストなし
	DepthStencil.StencilReadMask = 0;     // ステンシル読み込みマスク。
	DepthStencil.StencilWriteMask = 0;     // ステンシル書き込みマスク。
			// 面が表を向いている場合のステンシル・テストの設定
	DepthStencil.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;  // 維持
	DepthStencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;  // 維持
	DepthStencil.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;  // 維持
	DepthStencil.FrontFace.StencilFunc = D3D11_COMPARISON_NEVER; // 常に失敗
			// 面が裏を向いている場合のステンシル・テストの設定
	DepthStencil.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;   // 維持
	DepthStencil.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;   // 維持
	DepthStencil.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;   // 維持
	DepthStencil.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS; // 常に成功


	m_Device->CreateDepthStencilState(&DepthStencil, &g_pDepthStencilState);
	



	return true;
}


bool DirectGraphics::CreateRasterizer() {

	//// ラスタライザ・ステート・オブジェクトの作成
	D3D11_RASTERIZER_DESC RSDesc;
	RSDesc.FillMode = D3D11_FILL_SOLID;   // 普通に描画する
	RSDesc.CullMode = D3D11_CULL_NONE;    // 両面を描画する
	//RSDesc.CullMode = D3D11_CULL_FRONT;
	//RSDesc.CullMode = D3D11_CULL_BACK;
	RSDesc.FrontCounterClockwise = FALSE; // 時計回りが表面
	RSDesc.DepthBias = 0;
	RSDesc.DepthBiasClamp = 0;
	RSDesc.SlopeScaledDepthBias = 0;
	RSDesc.DepthClipEnable = TRUE;
	RSDesc.ScissorEnable = FALSE;
	RSDesc.MultisampleEnable = TRUE;
	RSDesc.AntialiasedLineEnable = TRUE;
	m_Device->CreateRasterizerState(&RSDesc, &g_pRasterizerState);


	return true;
}

bool DirectGraphics::CreateRastrizerSM() {

	//// ラスタライザ・ステート・オブジェクトの作成
	D3D11_RASTERIZER_DESC RSDesc;
	RSDesc.FillMode = D3D11_FILL_SOLID;   // 普通に描画する
	RSDesc.CullMode = D3D11_CULL_FRONT;
	//RSDesc.CullMode = D3D11_CULL_BACK;

	RSDesc.FrontCounterClockwise = FALSE; // 時計回りが表面
	RSDesc.DepthBias = 0;
	RSDesc.DepthBiasClamp = 0;
	RSDesc.SlopeScaledDepthBias = 0;
	RSDesc.DepthClipEnable = TRUE;
	RSDesc.ScissorEnable = FALSE;
	RSDesc.MultisampleEnable = TRUE;
	RSDesc.AntialiasedLineEnable = TRUE;
	RSDesc.DepthBias = 0;
	m_Device->CreateRasterizerState(&RSDesc, &g_pRasterizerStateSM);

	return 1;
}
bool DirectGraphics::CreateDepthStencilSM() {


	// 深度/ステンシル・ステート・オブジェクトの作成
	D3D11_DEPTH_STENCIL_DESC DepthStencil;
	DepthStencil.DepthEnable = TRUE; // 深度テストあり
	DepthStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // 書き込む
	DepthStencil.DepthFunc = D3D11_COMPARISON_LESS; // 手前の物体を描画
	DepthStencil.StencilEnable = FALSE; // ステンシル・テストなし
	DepthStencil.StencilReadMask = 0xff;     // ステンシル読み込みマスク。
	DepthStencil.StencilWriteMask = 0xff;     // ステンシル書き込みマスク。
			// 面が表を向いている場合のステンシル・テストの設定
	DepthStencil.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;  // 維持
	DepthStencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;  // 維持
	DepthStencil.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;  // 維持
	DepthStencil.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS; // 常に成功
			// 面が裏を向いている場合のステンシル・テストの設定
	DepthStencil.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;   // 維持
	DepthStencil.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;   // 維持
	DepthStencil.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;   // 維持
	DepthStencil.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS; // 常に成功
	m_Device->CreateDepthStencilState(&DepthStencil,
		&g_pDepthStencilStateSM);
	return 1;

}

bool DirectGraphics::CreateTextureSamplerSM() {
	// サンプラーの作成
	D3D11_SAMPLER_DESC descSampler;
	descSampler.Filter = D3D11_FILTER_ANISOTROPIC;
	descSampler.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	descSampler.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	descSampler.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	descSampler.MipLODBias = 0.0f;
	descSampler.MaxAnisotropy = 2;
	descSampler.ComparisonFunc = D3D11_COMPARISON_NEVER;
	descSampler.BorderColor[0] = 1.0f;
	descSampler.BorderColor[1] = 1.0f;
	descSampler.BorderColor[2] = 1.0f;
	descSampler.BorderColor[3] = 1.0f;
	descSampler.MinLOD = -FLT_MAX;
	descSampler.MaxLOD = FLT_MAX;
	m_Device->CreateSamplerState(&descSampler, &m_SamplerStateSM);

	return 1;
}
bool DirectGraphics::CreateTexture2DSM() {
	// シャドウ マップの作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = 700*3;   // 幅
	descDepth.Height = 700*3;  // 高さ
	descDepth.MipLevels = 1;       // ミップマップ レベル数
	descDepth.ArraySize = 1;       // 配列サイズ
	descDepth.Format = DXGI_FORMAT_R32_TYPELESS;  // フォーマット
	descDepth.SampleDesc.Count = MSAA.Count;
	descDepth.SampleDesc.Quality = MSAA.Quality;
	descDepth.Usage = D3D11_USAGE_DEFAULT;      // デフォルト使用法
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE; // 深度/ステンシル、シェーダ リソース ビューとして使用
	descDepth.CPUAccessFlags = 0;   // CPUからはアクセスしない
	descDepth.MiscFlags = 0;   // その他の設定なし
	m_Device->CreateTexture2D(
		&descDepth,         // 作成する2Dテクスチャの設定
		NULL,               // 
		&g_pShadowMap);     // 作成したテクスチャを受け取る変数

	return 1;
}
bool DirectGraphics::CreateDepthStencilViewSM() {
	// 深度/ステンシル ビューの作成
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;            // ビューのフォーマット
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Flags = 0;
	descDSV.Texture2D.MipSlice = 0;
	m_Device->CreateDepthStencilView(
		g_pShadowMap,         // 深度/ステンシル・ビューを作るテクスチャ
		&descDSV,             // 深度/ステンシル・ビューの設定
		&g_pShadowMapDSView); // 作成したビューを受け取る変数
	return 1;
}
bool DirectGraphics::CreateShaderResourceSM() {
	// シェーダ リソース ビューの作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT_R32_FLOAT; // フォーマット
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2DMS;  // 2Dテクスチャ
	srDesc.Texture2D.MostDetailedMip = 0;   // 最初のミップマップ レベル
	srDesc.Texture2D.MipLevels = -1;  // すべてのミップマップ レベル

	// シェーダ リソース ビューの作成
	m_Device->CreateShaderResourceView(
		g_pShadowMap,          // アクセスするテクスチャ リソース
		&srDesc,               // シェーダ リソース ビューの設定
		&g_pShadowMapSRView);  // 受け取る変数
	return 1;
}
bool  DirectGraphics::SetUpViewPortSM() {
	// ビューポートの設定
	g_ViewPortSM[0].TopLeftX = 0.0f;		// ビューポート領域の左上X座標。
	g_ViewPortSM[0].TopLeftY = 0.0f;		// ビューポート領域の左上Y座標。
	g_ViewPortSM[0].Width = 700*3;	// ビューポート領域の幅
	g_ViewPortSM[0].Height = 700*3;	// ビューポート領域の高さ
	g_ViewPortSM[0].MinDepth = 0.0f;		// ビューポート領域の深度値の最小値
	g_ViewPortSM[0].MaxDepth = 1.0f;		// ビューポート領域の深度値の最大値
	return 1;
}

void DirectGraphics::SetTextureSM(ID3D11ShaderResourceView* texture) {

	//ID3D11SamplerState* samp[2];
	//samp[0] = m_SamplerState;
	//samp[1] = m_SamplerStateSM;
	//m_Context->PSSetSamplers(0, 2, samp);

	ID3D11ShaderResourceView* srv[2];
	srv[0] = texture;
	srv[1] = g_pShadowMapSRView;
				// ID3D11ShaderResourceView
	// PixelShaderで使用するテクスチャの設定
	m_Context->PSSetShaderResources(
		0,								// スロット番号
		2,								// リソースの数
		srv);						// ID3D11ShaderResourceView

}


void DirectGraphics::SetUpContextSM(Vector3 pos, Vector3 scale, Vector3 degree, VertexShader* m_VertexShader, Vector3 lightPos, Vector3 lightFocus, Vector3 lightUp, float lightFov) {
	//深度情報リセット
	m_Context->ClearDepthStencilView(g_pShadowMapDSView, D3D11_CLEAR_DEPTH, 1.0f, 0);


	// プリミティブの形状を指定
	m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//ブレンドステート設定
	float blendFactor[4] = { D3D11_BLEND_ZERO,D3D11_BLEND_ZERO,D3D11_BLEND_ZERO,D3D11_BLEND_ZERO };
	m_Context->OMSetBlendState(g_pBlendState, blendFactor, 0xffffffff);

	m_Context->RSSetViewports(1, g_ViewPortSM);

	SetShader(m_VertexShader, nullptr, nullptr);

	SetWorldMatrix(pos, scale, degree);

	// OMに深度/ステンシル・ステート・オブジェクトを設定
	m_Context->OMSetDepthStencilState(g_pDepthStencilStateSM, 0);

	m_Context->RSSetState(g_pRasterizerStateSM);

	ID3D11RenderTargetView* pRender[1] = { NULL };
	m_Context->OMSetRenderTargets(1, pRender, g_pShadowMapDSView);



	SetLight( lightPos,  lightFocus,  lightUp, lightFov, Vector3(1, 1, 1), true);
	

	// コンスタントバッファ更新　map unmap　と同じ、map unmapのほうが早いかも
	m_Context->UpdateSubresource(m_ConstantBuffer, 0, NULL, &m_ConstantBufferData, 0, 0);

	// コンテキストにコンスタントバッファを設定
	m_Context->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	m_Context->GSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	m_Context->PSSetConstantBuffers(0, 1, &m_ConstantBuffer);


	DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(pos.X, pos.Y, pos.Z);
	DirectX::XMMATRIX rotate_x = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(degree.X));
	DirectX::XMMATRIX rotate_y = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(degree.Y));
	DirectX::XMMATRIX rotate_z = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(degree.Z));
	DirectX::XMMATRIX scale_mat = DirectX::XMMatrixScaling(scale.X, scale.Y, scale.Z);
	DirectX::XMMATRIX matWorld = scale_mat * rotate_x * rotate_y * rotate_z * translate;

	// ビュー変換行列(光源から見る)
	DirectX::XMVECTORF32 focusPosition = {lightFocus.X,lightFocus.Y ,lightFocus.Z };
	DirectX::XMVECTORF32 upDirection = { lightUp.X,lightUp.Y ,lightUp.Z };
	DirectX::XMFLOAT3 g_vLightPos(lightPos.X, lightPos.Y, lightPos.Z);   // 光源座標(ワールド座標系)
	DirectX::XMMATRIX matShadowMapView = XMMatrixLookAtLH(DirectX::XMLoadFloat3(&g_vLightPos), focusPosition, upDirection);

	// 射影変換行列
	DirectX::XMMATRIX matShadowMapProj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(
lightFov),g_ViewPortSM[0].Width / g_ViewPortSM[0].Height,	0.1f,1000.0f);
	// シャドウマップの設定
	DirectX::XMMATRIX mat = DirectX::XMMatrixTranspose(matWorld * matShadowMapView * matShadowMapProj);
	DirectX::XMStoreFloat4x4(&m_ConstantBufferData.SMWorldViewProj, mat);


}

void DirectGraphics::SetUpContextSM2(Vector3 pos, Vector3 scale, Vector3 degree, VertexShader* m_VertexShader, GeometryShader* m_GeometryShader, PixelShader* m_PixelShader) {
	m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	
	//ブレンドステート設定
	float blendFactor[4] = { D3D11_BLEND_ZERO,D3D11_BLEND_ZERO,D3D11_BLEND_ZERO,D3D11_BLEND_ZERO };
	m_Context->OMSetBlendState(g_pBlendState, blendFactor, 0xffffffff);



	
	SetShader(m_VertexShader, m_GeometryShader, m_PixelShader);


	ID3D11SamplerState* samp[2];
	samp[0] = m_SamplerState;
	samp[1] = m_SamplerStateSM;
	m_Context->PSSetSamplers(0, 2, samp);




	SetUpViewPort();

	m_Context->RSSetState(g_pRasterizerState);


	m_Context->OMSetDepthStencilState(g_pDepthStencilState, 0);
	// OMに描画ターゲット ビューと深度/ステンシル・ビューを設定
	m_Context->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	//定数バッファをシェーダーに設定
	m_Context->UpdateSubresource(m_ConstantBuffer, 0, NULL, &m_ConstantBufferData, 0, 0);

	ID3D11Buffer* constant_buffer = GetConstantBuffer();
	m_Context->VSSetConstantBuffers(0, 1, &constant_buffer);
	m_Context->GSSetConstantBuffers(0, 1, &constant_buffer);
	m_Context->PSSetConstantBuffers(0, 1, &constant_buffer);



}