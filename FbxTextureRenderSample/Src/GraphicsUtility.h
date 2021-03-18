#ifndef GRAPHICS_UTILITY_H_
#define GRAPHICS_UTILITY_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

struct ConstantBuffer
{
	DirectX::XMFLOAT4X4 World;
	DirectX::XMFLOAT4X4 View;
	DirectX::XMFLOAT4X4 Projection;
	DirectX::XMFLOAT4	CameraPos;
	DirectX::XMFLOAT4	LightVector;
	DirectX::XMFLOAT4   LightColor;
	DirectX::XMFLOAT4	MaterialAmbient;
	DirectX::XMFLOAT4	MaterialDiffuse;
	DirectX::XMFLOAT4	MaterialSpecular;
	

	DirectX::XMFLOAT4X4 SMWorldViewProj; // ワールド×ビュー×透視変換行列(シャドウマップ用)

	DirectX::XMMATRIX mBoneMatrix[50];
};



class Vector3
{
public:
	Vector3()
	{
		this->X = 0.0f;
		this->Y = 0.0f;
		this->Z = 0.0f;
	}

	Vector3(float x, float y, float z)
	{
		this->X = x;
		this->Y = y;
		this->Z = z;
	}
	Vector3 operator*(float tmp) {
		Vector3 v3;
		v3.X = X * tmp;
		v3.Y = Y * tmp;
		v3.Z = Z * tmp;
		return v3;
	};
	Vector3 operator/(float tmp) {
		Vector3 v3;
		v3.X = X / tmp;
		v3.Y = Y / tmp;
		v3.Z = Z / tmp;
		return v3;
	};
	float X;
	float Y;
	float Z;
};


class Vector2
{
public:
	Vector2()
	{
		this->X = 0.0f;
		this->Y = 0.0f;
	}

	Vector2(float x, float y)
	{
		this->X = x;
		this->Y = y;
	}

	float X;
	float Y;
};

class Color
{
public:
	float Red;
	float Green;
	float Blue;
	float Alpha;

	Color(float red, float green, float blue, float alpha)
	{
		Red = red;
		Green = green;
		Blue = blue;
		Alpha = alpha;
	}

	Color()
	{
		Red = Green = Blue = Alpha = 1.0f;
	}

};

//=====================================================================//
//! ポリゴン出力用カスタムバーテックス構造体
//=====================================================================//

struct CustomVertex
{
	Vector3 Posision;		// 座標(x, y, z)
	Vector3 Normal;			// 法線
	Color Color;			// 頂点カラー
	Vector2 TexturePos;		// テクスチャ座標(u, v)
	int BoneIndex[4];
	float BoneWeight[4];
};


struct ObjMaterial
{
	ObjMaterial()
	{
		for (int i = 0; i < 4; i++)
		{
			Ambient[i] = 1.0f;
			Diffuse[i] = 1.0f;
			Specular[i] = 1.0f;
		}
		//TextureKeyWord = "";
		//TextureName = "";
	}

	void SetAmbient(float r, float g, float b, float factor)
	{
		Ambient[0] = r;
		Ambient[1] = g;
		Ambient[2] = b;
		Ambient[3] = factor;
	}

	void SetDiffuse(float r, float g, float b, float factor)
	{
		Diffuse[0] = r;
		Diffuse[1] = g;
		Diffuse[2] = b;
		Diffuse[3] = factor;
	}

	void SetSpecular(float r, float g, float b, float factor)
	{
		Specular[0] = r;
		Specular[1] = g;
		Specular[2] = b;
		Specular[3] = factor;
	}

	float Ambient[4];
	float Diffuse[4];
	float Specular[4];
	float Alpha;
	//std::string TextureKeyWord;
	//std::string TextureName;
};

#endif
