// 頂点シェーダの入力データ定義
struct VS_INPUT {

	float4 Pos : POSITION0;
	float4 Norm : NORMAL0;
	float2 Tex : TEXTURE0;
	float4 Color : COLOR0;

	int4 boneIndex :  BONEINDEX;
	float4 boneWeight : BONEWEIGHT;
};

cbuffer ConstantBuffer
{
	float4x4	World;				// ワールド変換行列
	float4x4	View;				// ビュー変換行列
	float4x4	Projection;			// 透視射影変換行列
	float4		CameraPos;			// カメラ座標
	float4		LightVector;		// ライト方向
	float4		LightColor;			// ライトカラー
	float4		MaterialAmbient;	// アンビエント
	float4		MaterialDiffuse;	// ディフューズ
	float4		MaterialSpecular;	// スペキュラー

	float4x4 boneMatrix[50];
}
// 頂点シェーダの関数
float3 main(VS_INPUT input) : POSITION{
	// 頂点座標　モデル座標系→ビュー座標系
	input.Pos.w = 1.0;
	float4 Pos = input.Pos;
	Pos = mul(Pos, World);
	Pos = mul(Pos, View);
	return Pos.xyz / Pos.w;
}
