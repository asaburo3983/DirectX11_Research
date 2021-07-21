#include "FbxModel.h"
#include "DXTK11Lib.h"
#include "input.h"

void FbxModel::Load(const char* modelfile_name, const char* texfile_name) {
	m_VertexShader = md.vs.m_VertexShader;

	LoadModel(modelfile_name, texfile_name);
}
FbxModel::FbxModel() {
	con = DirectGraphics::GetInstance()->GetContext();
	pos = Vector3(0.0f, 0.0f, 0.0f);
	degree = Vector3(0.0f, 0.0f, 0.0f);
	scale = Vector3(1.0f, 1.0f, 1.0f);

	axis = Vector3(1.0f, 1.0f, 1.0f);
	axisAngle = 0;
}
void FbxModel::SetScaleALL(float _scale) {
	scale = Vector3(_scale, _scale, _scale);
}
void FbxModel::RenderShadowMap() {
	SetModelData();
	RenderS_SM();
};
void FbxModel::RenderShadowModel() {
	SetModelData();
	RenderS();
};
void FbxModel::RenderEdge(Vector3 edgeSize, Vector3 edgeColor) {
	SetModelData();
	RenderE(edgeSize, edgeColor);
}
void FbxModel::RenderTextureView() {
	SetModelData();
	RenderTV();
}
void FbxModel::RenderCubeMap() {
	SetModelData();
	RenderCM();
}
void FbxModel::RenderModel() {
	SetModelData();
	Render();
}
void FbxModel::SetModelData() {
	CreateInputLayout(DirectGraphics::GetInstance()->GetDevice());//‚¿‚å‚¢d‚¢‚©‚à
	pos = md.pos;
	scale = md.scale;
	degree = md.degree;
	axis = md.axis;
	axisAngle = md.axisAngle;
	axisR = md.axisR;

	m_VertexShader = md.vs.m_VertexShader;
	m_GeometryShader = md.gs.m_GeometryShader;
	m_PixelShader = md.ps.m_PixelShader;

}


ModelData::ModelData() {
	pos = Vector3(0.0f, 0.0f, 0.0f);
	degree = Vector3(0.0f, 0.0f, 0.0f);
	scale = Vector3(1.0f, 1.0f, 1.0f);
	axis = Vector3(1.0f, 1.0f, 1.0f);
	axisAngle = 0;
	axisR = Vector3(0.0f, 0.0f, 0.0f);

}
void ModelData::SetShader(VertexShaderData _vs, GeometryShaderData _gs, PixelShaderData _ps) {
	vs = _vs;
	gs = _gs;
	ps = _ps;

}
void ModelData::DebugMode(float speed, bool quaternion, int strX, int strY) {
	KeyInput* KEY = KeyInput::GetInstance();

	char buf[256];
	WCHAR wbuf[256];
	sprintf_s(buf, sizeof(buf), "POS:X:%.2f:Y:%.2f:Z:%.2f", pos.X, pos.Y, pos.Z);
	mbstowcs_s(0, wbuf, 256, buf, _TRUNCATE);
	DXTK11LIB::DrawString(wbuf, strX + 0, strY + 30, DXTK11LIB::StrColor{ 1,0,0,1 });
	sprintf_s(buf, sizeof(buf), "SCALE:X:%.2f:Y:%.2f:Z:%.2f", scale.X, scale.Y, scale.Z);
	mbstowcs_s(0, wbuf, 256, buf, _TRUNCATE);
	DXTK11LIB::DrawString(wbuf, strX + 0, strY + 60, DXTK11LIB::StrColor{ 1,0,0,1 });
	sprintf_s(buf, sizeof(buf), "DEGREE:X:%.2f:Y:%.2f:Z:%.2f", degree.X, degree.Y, degree.Z);
	mbstowcs_s(0, wbuf, 256, buf, _TRUNCATE);
	DXTK11LIB::DrawString(wbuf, strX + 0, strY + 90, DXTK11LIB::StrColor{ 1,0,0,1 });

	//Šgk
	if (KEY->key[DIK_LCONTROL] >= 1 && KEY->key[DIK_LSHIFT] >= 1) {
		if (KEY->key[DIK_W] >= 1) {
			scale.Y += speed;
		}
		if (KEY->key[DIK_S] >= 1) {
			scale.Y -= speed;
		}
		if (KEY->key[DIK_D] >= 1) {
			scale.X += speed;
		}
		if (KEY->key[DIK_A] >= 1) {
			scale.X -= speed;
		}
		if (KEY->key[DIK_X] >= 1) {
			scale.Z += speed;
		}
		if (KEY->key[DIK_Z] >= 1) {
			scale.Z -= speed;
		}
		if (KEY->key[DIK_UP] >= 1) {
			scale.X += speed;
			scale.Y += speed;
			scale.Z += speed;
		}
		if (KEY->key[DIK_DOWN] >= 1) {
			scale.X -= speed;
			scale.Y -= speed;
			scale.Z -= speed;
		}
		return;
	}
	//ˆÚ“®
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
	//‰ñ“]
	if (quaternion == false) {
		if (KEY->key[DIK_LCONTROL] >= 1) {
			if (KEY->key[DIK_D] >= 1) {

				degree.Y++;
			}
			if (KEY->key[DIK_A] >= 1) {
				degree.Y--;
			}

			if (KEY->key[DIK_W] >= 1) {
				degree.X++;
			}
			if (KEY->key[DIK_S] >= 1) {
				degree.X--;
			}

			if (KEY->key[DIK_X] >= 1) {
				degree.Z++;
			}
			if (KEY->key[DIK_Z] >= 1) {
				degree.Z--;
			}
			return;
		}
	}
	else {
		if (KEY->key[DIK_LCONTROL] >= 1) {
			if (KEY->key[DIK_D] >= 1) {

				axisAngle++;
			}
			if (KEY->key[DIK_A] >= 1) {
				axisAngle--;
			}
			return;
		}

	}
}
