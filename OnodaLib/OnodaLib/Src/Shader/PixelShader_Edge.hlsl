struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float4 colorNormal : COLOR_NORMAL;
	// 追加
	float2 texture_pos : TEXTURE0;

	float3 PosSM: POSITION_SM; // 頂点座標(シャドウマップの透視座標系)
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

	matrix SMWorldViewProj;
	float4x4 boneMatrix[50];

	float4 edgeSize;
	float4 edgeColor;

}

float4 main(PS_IN input) : SV_Target
{
	float4 colorR;
	colorR.r=   edgeColor.r;
	colorR.g = edgeColor.g;
	colorR.b = edgeColor.b;
	colorR.a=1;


	return colorR;
}
