#pragma once
#include "GameMathLib.h"
#include <Windows.h>
#include <DirectXMath.h>
#include <vector>
#include <time.h>
#include <mmsystem.h>
#include <memory>
#include "Window.h"

#include "CameraAndLight.h"
#include "DirectGraphics.h"
#include "FbxMeshFile.h"
#define FBXSDK_SHARED
#include "DXTK11Lib.h"
#include "input.h"
#include "FbxMeshFile.h"
#include <string>

#include "FbxModel.h"
#include "MainManager.h"
using namespace GameMath;

//ƒVƒXƒeƒ€‚Ì‰Šú‰»ˆ—
void AllInit(const char* winName, int sizeX, int sizeY,bool fullScreen, HINSTANCE hInstance);
void SetAntialias(bool on);


