#ifndef FBX_MESH_FILE_H_
#define FBX_MESH_FILE_H_
#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }

#include <fbxsdk.h>
#include <vector>
#include <map>
#include <d3d11.h>
#include "DirectGraphics.h"
#include "ShaderBase.h"
#include "GraphicsUtility.h"

#include "GameMathLib.h"
class FbxMeshFile
{
public://�Q�[�����Ŏg���f�[�^
	Vector3 pos;
	Vector3 degree;
	Vector3 scale;
	Vector3 axis;	
	float axisAngle;
	Vector3 axisR;
	VertexShader* m_VertexShader;	//!< @brief VertexShader�ێ��p
	PixelShader* m_PixelShader;		//!< @brief PixelShader�ێ��p
	GeometryShader* m_GeometryShader;		//!< @brief GeometryShader�ێ��p


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
		

		//�{�̂̃��f��



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

	bool LoadModel(const char* modelfile_name, const char* texfile_name);

	void Render();

private:
	struct MeshData
	{
		ID3D11Buffer* m_VertexBuffer;			//!< @brief ���_�o�b�t�@(Shader���M�p)
		ID3D11Buffer* m_IndexBuffer;			//!< @brief �C���f�b�N�X�o�b�t�@(Shader���M�p)
		std::vector<CustomVertex> m_Vertices;
		std::vector<UINT> m_Indices;
		std::string m_MaterialName;
		std::string m_UVSetName;



	};
public:
	ID3D11DeviceContext* con;
private:

	bool LoadFbxFile(const char* modelfile_name,const char* texfile_name);

	bool CreateVertexBuffer(ID3D11Device* device);

	bool CreateIndexBuffer(ID3D11Device* device);
protected:
	bool CreateInputLayout(ID3D11Device* device);
private:
	void CreateMesh(FbxMesh* mesh);

public:
		//�A�j���[�V�����ŗ��p
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

	void LoadIndices(MeshData& mesh_data, FbxMesh* mesh);

	void LoadVertices(MeshData& mesh_data, FbxMesh* mesh);

	void LoadNormals(MeshData& mesh_data, FbxMesh* mesh);

	void LoadColors(MeshData& mesh_data, FbxMesh* mesh);

	void LoadUV(MeshData& mesh_data, FbxMesh* mesh);

	void LoadMaterial(FbxSurfaceMaterial* material,const char* texfile_name);

	bool LoadTexture(FbxFileTexture* material, std::string& keyword, const char* texfile_name);

	void SetMaterialName(MeshData& mesh_data, FbxMesh* mesh);
	void SetMaterialColor(DirectGraphics* graphics, ObjMaterial& material);

public:

	//void RenderSV(DirectGraphics* graphics);
	//void RenderS(DirectGraphics* graphics);

private:
	void DrawVertex();//���_�n�������̊֐��i���֐��̃R�[�h�Z�k�̂��߁j
public:
	void RenderS();//�V���h�E�t���̃��f����`��
	void RenderS_SM();//�V���h�E�}�b�v�`�� (�V���h�E�`�揈���Ŏg��)
	void RenderE(Vector3 edgeSize = {1.01,1.01,1.01}, Vector3 edgeColor = { 1,1,1 });//�m�[�}���}�b�v�`�� (�G�b�W�����Ŏg��)
	void RenderTV();//�e�N�X�`���ɕ`�悷��
	void RenderCM();//�L���[�u�}�b�v�`��
private:
	std::vector<MeshData> m_MeshList;
	std::map<std::string, ObjMaterial> m_Materials;
	ID3D11InputLayout* m_InputLayout;								//!< @brief ���̓��C�A�E�g
	std::map< std::string, ID3D11ShaderResourceView*> m_Textures;
	std::map<std::string, ID3D11ShaderResourceView*> m_MaterialLinks;

};

//�d�l�p�r�����̃t�@�C���ɂ��y�Ԃ��߁A�֐���
VertexShader* CreateShader_V(const char* VertexShaderFilePath);
GeometryShader* CreateShader_G(const char* GeometryShaderFilePath);
PixelShader* CreateShader_P(const char* PixelShaderFilePath);
#endif
