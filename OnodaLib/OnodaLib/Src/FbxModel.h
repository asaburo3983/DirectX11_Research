#pragma once
#include "FbxMeshFile.h"

class VertexShaderData {
public:
	VertexShader* m_VertexShader;	//!< @brief VertexShader�ێ��p
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
	GeometryShader* m_GeometryShader;		//!< @brief GeometryShader�ێ��p
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
	PixelShader* m_PixelShader;		//!< @brief PixelShader�ێ��p
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
	ModelData md;//�p�����Ċy�ɂ��������A�������f�����g�p����ۂ�MD��������ւ��Ďg���������炱�̕���
	int animeNum = NULL;
public:
	FbxModel();
	void Load(const char* modelfile_name, const char* texfile_name);
	void SetScaleALL(float _scale);
	void RenderShadowMap();
	void RenderShadowModel();
	void RenderEdge(Vector3 edgeSize, Vector3 edgeColor);
	void RenderTextureView();//�e�N�X�`���ɕ`��
	void RenderCubeMap();
	void RenderModel();
	void SetModelData();
};
