#pragma once
#include "FbxMeshFile.h"

class VertexShaderData {
public:
	VertexShader* m_VertexShader;	//!< @brief VertexShader保持用
public:
	VertexShaderData() {}
	VertexShaderData(const char* path) {
		m_VertexShader = CreateShader_V(path);
	}
	void Load(const char* path) {
		m_VertexShader = CreateShader_V(path);
	}

};
class GeometryShaderData {
public:
	GeometryShader* m_GeometryShader;		//!< @brief GeometryShader保持用
public:
	GeometryShaderData() {}
	GeometryShaderData(const char* path) {
		m_GeometryShader = CreateShader_G(path);
	}
	void Load(const char* path) {
		m_GeometryShader = CreateShader_G(path);
	}
};
class PixelShaderData {
public:
	PixelShader* m_PixelShader;		//!< @brief PixelShader保持用
public:
	PixelShaderData() {}
	PixelShaderData(const char* path) {
		m_PixelShader = CreateShader_P(path);
	}
	void Load(const char* path) {
		m_PixelShader = CreateShader_P(path);
	}
};

class ModelData {
public:
	Vector3 pos;
	Vector3 degree;
	Vector3 scale;
	Vector3 axis;
	float axisAngle;
	Vector3 axisR;

	VertexShaderData vs;
	GeometryShaderData gs;
	PixelShaderData ps;
public:
	ModelData();

	void DebugMode(float speed = 0.01, bool quaternion = false, int strX = 0, int strY = 0);
	void SetShader(VertexShaderData _vs, GeometryShaderData _gs, PixelShaderData _ps);
};

class FbxModel : public FbxMeshFile {
public:
	ModelData md;//継承して楽にしたいが、同じモデルを使用する際にMDだけ入れ替えて使いたいからこの方式
	int animeNum = NULL;
public:
	FbxModel();
	void Load(const char* modelfile_name, const char* texfile_name);
	void SetScaleALL(float _scale);
	void RenderShadowMap();
	void RenderShadowModel();
	void RenderEdge(Vector3 edgeSize, Vector3 edgeColor);
	void RenderTextureView();//テクスチャに描画
	void RenderCubeMap();
	void RenderModel();
	void SetModelData();
};
