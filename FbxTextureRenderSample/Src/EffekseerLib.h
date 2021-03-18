#pragma once

#ifdef _DEBUG
#pragma comment(lib, "Effekseerd.lib")
#pragma comment(lib, "EffekseerRendererDX11d.lib")
#else
#pragma comment(lib, "Effekseer.lib")
#pragma comment(lib, "EffekseerRendererDX11.lib")
#endif

#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include "DirectGraphics.h"

void EffekseerInit();
void EffekseerDraw();
void EffekseerUnInit();

class Effekseer_Effect {


};