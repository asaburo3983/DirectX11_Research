#ifndef FBX_MESH_FILE_H_
#define FBX_MESH_FILE_H_
#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }

#include <fbxsdk.h>
#include <vector>
#include <map>
#include <d3d11.h>
#include "DirectGraphics.h"
#include "VertexShader.h"
#include "GeometryShader.h"
#include "GraphicsUtility.h"

#include "GameMathLib.h"
class FbxMeshFile
{
public:
	/** Constructor */
	FbxMeshFile() :
		m_InputLayout(nullptr)
	{
		m_MeshList.clear();
	}

	/** Destructor */
	~FbxMeshFile()
	{
		
		SAFE_RELEASE(g_pRasterizerStateSV);
		SAFE_RELEASE(g_pDepthStencilStateSV);
		SAFE_RELEASE(g_pBlendStateShadow);
		SAFE_RELEASE(g_pDepthStencilStateShadow);

		//本体のモデル
		if (m_VertexShader != nullptr) {
			delete m_VertexShader;
			m_VertexShader = nullptr;
		}
		if (m_GeometryShader != nullptr) {
			delete m_GeometryShader;
			m_GeometryShader = nullptr;
		}
		if (m_PixelShader != nullptr) {
			delete m_PixelShader;
			m_PixelShader = nullptr;
		}
		//シャドウボリューム
		if (m_VertexSVShader != nullptr) {
			delete m_VertexSVShader;
			m_VertexSVShader = nullptr;
		}
		if (m_GeometrySVShader != nullptr) {
			delete m_GeometrySVShader;
			m_GeometrySVShader = nullptr;
		}
		//シャドウ
		if (m_VertexSShader != nullptr) {
			delete m_VertexSShader;
			m_VertexSShader = nullptr;
		}
		if (m_PixelSShader != nullptr) {
			delete m_PixelSShader;
			m_PixelSShader = nullptr;
		}

		for (auto texture : m_Textures)
		{
			if (texture.second != nullptr)
			{
				texture.second->Release();
			}
		}
		m_Textures.clear();
		m_MaterialLinks.clear();

		for (auto mesh : m_MeshList)
		{
			if (mesh.m_VertexBuffer != nullptr)
			{
				mesh.m_VertexBuffer->Release();
			}

			if (mesh.m_IndexBuffer != nullptr)
			{
				mesh.m_IndexBuffer->Release();
			}
			mesh.m_Indices.clear();
			mesh.m_Vertices.clear();
		}

		if (m_InputLayout != nullptr)
		{
			m_InputLayout->Release();
			m_InputLayout = nullptr;
		}


		if (FrameTime != nullptr) {
			delete FrameTime;
			FrameTime = nullptr;
		}
		if (timeCount != nullptr) {
			delete timeCount;
			timeCount = nullptr;
		}
		if (start != nullptr) {
			delete start;
			start = nullptr;
		}
		if (stop != nullptr) {
			delete stop;
			stop = nullptr;
		}
	}

	/**
	* @brief モデル読み込み@n
	* モデルデータを読み込む
	* @return 作成の成否 成功(true)
	* @param[in] device デバイス
	* @param[in] vertex_shader モデルと関連付けるVertexShader
	*/
	bool Load(const char* modelfile_name, const char* texfile_name, const char* VertexShaderFilePath, const char* GeometryShaderFilePath, const char* PixelShaderFilePath);

	/**
	* @brief モデル描画関数@n
	* 指定された情報でモデルを描画する
	* @return 作成の成否 成功(true)
	* @param[in] graphics 描画管理クラス
	* @param[in] pos 描画座標
	* @param[in] scale 拡縮率
	* @param[in] degree 角度
	*/
	void Render(Vector3 pos, Vector3 scale, Vector3 degree);

private:
	struct MeshData
	{
		ID3D11Buffer* m_VertexBuffer;			//!< @brief 頂点バッファ(Shader送信用)
		ID3D11Buffer* m_IndexBuffer;			//!< @brief インデックスバッファ(Shader送信用)
		std::vector<CustomVertex> m_Vertices;
		std::vector<UINT> m_Indices;
		std::string m_MaterialName;
		std::string m_UVSetName;



	};
public:
	ID3D11DeviceContext* con;
private:

	/**
	* @brief メッシュ生成@n
	* ファイルからメッシュを生成する
	* @return 作成の成否 成功(true)
	* @param[in] file_name ファイル名
	*/
	bool LoadFbxFile(const char* modelfile_name,const char* texfile_name);

	/**
	* @brief VertexBuffer作成@n
	* VertexBufferを作成する
	* @return 作成の成否 成功(true)
	* @param[in] device DirectX11グラフィクスデバイス
	*/
	bool CreateVertexBuffer(ID3D11Device* device);

	/**
	* @brief IndexBuffer作成@n
	* IndexBufferを作成する
	* @return 作成の成否 成功(true)
	* @param[in] device DirectX11グラフィクスデバイス
	*/
	bool CreateIndexBuffer(ID3D11Device* device);

	/**
	* @brief InputLayout作成@n
	* InputLayoutを作成する
	* @return 作成の成否 成功(true)
	* @param[in] device DirectX11グラフィクスデバイス
	* @param[in] vertex_shader InputLayoutと関連付ける頂点シェーダ
	*/
	bool CreateInputLayout(ID3D11Device* device);

	/**
	* @brief メッシュデータを作成する
	* @param[in] node_name ノードの名前
	* @param[in] mesh Meshを作るためのNodeのMeshデータ
	*/
	void CreateMesh(FbxMesh* mesh);

	/**
	* @brief メッシュデータを作成する
	* @param[in] node_name ノードの名前
	* @param[in] mesh Meshを作るためのNodeのMeshデータ
	*/
	public:
		//アニメーションで利用
		FbxTime* FrameTime;
		FbxTime* timeCount;
		FbxTime* start;
		FbxTime* stop;
		int nowAnimeNum = -1;
		FbxMesh* fMesh[50];
		FbxScene* fbxSceneAnime;
		int meshNum = 0;
		void Animation(int _num,float speed);
	private:
	void CreateAnimation(FbxScene* fbxScene);
	/**
	* @brief 頂点インデックスデータを読み込む
	* @param[out] mesh_data バッファ保存用
	* @param[in] mesh バッファ取得をするMeshデータ
	*/
	void LoadIndices(MeshData& mesh_data, FbxMesh* mesh);

	/**
	* @brief 頂点データを読み込む
	* @param[out] mesh_data バッファ保存用
	* @param[in] mesh バッファ取得をするMeshデータ
	*/
	void LoadVertices(MeshData& mesh_data, FbxMesh* mesh);

	/**
	* @brief 法線データを読み込む
	* @param[out] mesh_data バッファ保存用
	* @param[in] mesh バッファ取得をするMeshデータ
	*/
	void LoadNormals(MeshData& mesh_data, FbxMesh* mesh);

	/**
	* @brief 頂点カラーデータを読み込む
	* @param[out] mesh_data バッファ保存用
	* @param[in] mesh バッファ取得をするMeshデータ
	*/
	void LoadColors(MeshData& mesh_data, FbxMesh* mesh);

	void LoadUV(MeshData& mesh_data, FbxMesh* mesh);

	/**
	* @brief マテリアルデータを読み込む
	* @param[out] mesh_data バッファ保存用
	* @param[in] mesh バッファ取得をするMeshデータ
	*/
	void LoadMaterial(FbxSurfaceMaterial* material,const char* texfile_name);

	bool LoadTexture(FbxFileTexture* material, std::string& keyword, const char* texfile_name);

	void SetMaterialName(MeshData& mesh_data, FbxMesh* mesh);
	void SetMaterialColor(DirectGraphics* graphics, ObjMaterial& material);

	//シェーダーのロード
	void CreateShader(const char* VertexShaderFilePath=nullptr, const char* GeometryShaderFilePath=nullptr, const char* PixelShaderFilePath=nullptr);

public:
	//影のシェーダーロード
	void CreateSVShader(const char* VertexShaderFilePath, const char* GeometryShaderFilePath);
	void CreateSShader(const char* VertexShaderFilePath, const char* PixelShaderFilePath);

	//void RenderSV(DirectGraphics* graphics);
	//void RenderS(DirectGraphics* graphics);

	void RenderS(Vector3 pos, Vector3 scale, Vector3 degree);
	void RenderS_SM(Vector3 pos, Vector3 scale, Vector3 degree);

private:
	std::vector<MeshData> m_MeshList;
	std::map<std::string, ObjMaterial> m_Materials;
	ID3D11InputLayout* m_InputLayout;								//!< @brief 入力レイアウト
	std::map< std::string, ID3D11ShaderResourceView*> m_Textures;
	std::map<std::string, ID3D11ShaderResourceView*> m_MaterialLinks;

	VertexShader* m_VertexShader;	//!< @brief VertexShader保持用
	PixelShader* m_PixelShader;		//!< @brief PixelShader保持用
	GeometryShader* m_GeometryShader;		//!< @brief GeometryShader保持用

	//シャドウボリューム用シェーダ―
	VertexShader* m_VertexSVShader;	//!< @brief VertexShader保持用
	GeometryShader* m_GeometrySVShader;		//!< @brief GeometryShader保持用
	ID3D11BlendState*         g_pBlendStateSV = NULL;		// ブレンド・ステート・オブジェクト
	ID3D11RasterizerState*	  g_pRasterizerStateSV = NULL;	// ラスタライザ・ステート・オブジェクト
	ID3D11DepthStencilState*  g_pDepthStencilStateSV = NULL;	// 深度/ステンシル・ステート・オブジェクト

	//シャドウ用シェーダー
	VertexShader* m_VertexSShader;	//!< @brief VertexShader保持用
	PixelShader* m_PixelSShader;		//!< @brief PixelShader保持用
	ID3D11BlendState*         g_pBlendStateShadow = NULL;		// ブレンド・ステート・オブジェクト
	ID3D11DepthStencilState*  g_pDepthStencilStateShadow = NULL;	// 深度/ステンシル・ステート・オブジェクト


};
#endif
