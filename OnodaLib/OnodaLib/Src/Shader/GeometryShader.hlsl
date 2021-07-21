// ジオメトリ シェーダの入力データ定義
struct GS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float4 colorNormal : COLOR_NORMAL;
	// 追加
	float2 texture_pos : TEXTURE0;

	float3 PosSM: POSITION_SM; // 頂点座標(シャドウマップの透視座標系)

		//環境マップの反射計算で使用する
	float3 ViewWorld : POSWORLD;  // 頂点座標ベクトル(ワールド座標系)
	float3 NormWorld : NORMWORLD; // 法線ベクトル(ワールド座標系)
};
struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float4 colorNormal : COLOR_NORMAL;
	// 追加
	float2 texture_pos : TEXTURE0;

	float3 PosSM: POSITION_SM; // 頂点座標(シャドウマップの透視座標系)

		//環境マップの反射計算で使用する
	float3 ViewWorld : POSWORLD;  // 頂点座標ベクトル(ワールド座標系)
	float3 NormWorld : NORMWORLD; // 法線ベクトル(ワールド座標系)
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


	// ワールド×ビュー×透視変換行列(シャドウマップ用)
}


// ジオメトリ シェーダの関数
[maxvertexcount(3)]
void main(triangle GS_IN input[3],
	inout TriangleStream<PS_IN> TriStream) {
	PS_IN output;

	// 各頂点の計算
	for (int i = 0; i < 3; ++i) {
		output.pos = input[i].pos;
		output.color=input[i].color;
		output.colorNormal=input[i].colorNormal;

		output.texture_pos = input[i].texture_pos;
		output.PosSM = input[i].PosSM;
		output.ViewWorld = input[i].ViewWorld;
		output.NormWorld = input[i].NormWorld;

		// 出力
		TriStream.Append(output);
	}
	TriStream.RestartStrip();
}