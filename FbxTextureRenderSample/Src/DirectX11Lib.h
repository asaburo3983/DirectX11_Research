#pragma once
#include "GameMathLib.h"
#include <Windows.h>
#include <DirectXMath.h>
#include <vector>
#include <time.h>
#include <mmsystem.h>
#include "Window.h"
#include "DirectGraphics.h"
#include "FbxMeshFile.h"
#define FBXSDK_SHARED
#include "DXTK11Lib.h"
#include "input.h"

using namespace GameMath;

void AllInit(const char* winName, int sizeX, int sizeY, HINSTANCE hInstance);

class FBX_MODEL : public FbxMeshFile {
public:
	Vector3 pos;
	Vector3 degree;
	Vector3 scale;
public:
	FBX_MODEL() {
		con = DirectGraphics::GetInstance()->GetContext();
		pos = Vector3(0.0f, 0.0f, 0.0f);
		degree = Vector3(0.0f, 0.0f, 0.0f);
		scale = Vector3(1.0f, 1.0f, 1.0f);
	}
	void SetScaleALL(float _scale) {
		scale = Vector3(_scale, _scale, _scale);
	}
};

class CAMERA {
private:
	Vector3 focs;
	const float length = 10.0f;
public:
	Vector3 pos;
	Vector3 angle;
public:
	CAMERA();
	void Updata();
};

