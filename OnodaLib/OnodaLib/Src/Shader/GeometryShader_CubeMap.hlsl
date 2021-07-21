// ジオメトリ シェーダの入力データ定義
struct GS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float4 colorNormal : COLOR_NORMAL;
	// 追加
	float2 texture_pos : TEXTURE0;

	float3 PosSM: POSITION_SM; // 頂点座標(シャドウマップの透視座標系)
};
struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float4 colorNormal : COLOR_NORMAL;
	// 追加
	float2 texture_pos : TEXTURE0;

	float3 PosSM: POSITION_SM; // 頂点座標(シャドウマップの透視座標系)

	uint RTIndex : SV_RenderTargetArrayIndex; // 描画ターゲット
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


	//キューブマップ用変換行列
	float4x4 CubeMapView[6];
	float4x4 CubeMapProjection;



	// ワールド×ビュー×透視変換行列(シャドウマップ用)
}
// ジオメトリ シェーダの関数
[maxvertexcount(18)]
void main(triangle GS_IN input[3],
	inout TriangleStream<PS_IN> TriStream) {


	//ここで頂点を減らしてるか知らんけどポリゴンアおかしくなりゅう
	// 
	// 
	// 各頂点の計算
	for (int h = 0; h < 6; h++) {
		PS_IN output;
		output.RTIndex = h;  // このプリミティブを描画する描画ターゲット
		for (int i = 0; i < 3; ++i) {

			//カメラごとにビュー、射影変換が必要
			output.pos = input[i].pos;
			output.pos = mul(output.pos, World);
			output.pos = mul(output.pos, CubeMapView[h]);	// ワールド座標 * ビュー座標変換行列
			output.pos = mul(output.pos, CubeMapProjection);// ビュー座標 * プロジェクション座標変換行列

			output.color = input[i].color;
			output.colorNormal = input[i].colorNormal;

			output.texture_pos = input[i].texture_pos;
			output.PosSM = input[i].PosSM;

			// 出力
			TriStream.Append(output);
		}
		TriStream.RestartStrip();
	}
	
}