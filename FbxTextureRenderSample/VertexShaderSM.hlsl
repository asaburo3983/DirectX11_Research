struct VS_IN
{
	float4 pos : POSITION0;
	float4 nor : NORMAL0;
	float4 color : COLOR0;
	// 追加
	float2 texture_pos : TEXTURE0;

	int4 boneIndex :  BONEINDEX;
	float4 boneWeight : BONEWEIGHT;
};
struct GS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	// 追加
	float2 texture_pos : TEXTURE0;



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
GS_IN main(VS_IN input)
{
	GS_IN output;

	output.pos = mul(input.pos, World);
	// ワールド座標 * ビュー座標変換行列
	output.pos = mul(output.pos, View);
	float4 posView = output.pos;
	// ビュー座標 * プロジェクション座標変換行列
	output.pos = mul(output.pos, Projection);

	// シャドウマップ用頂点座標

	float4 pos4 = input.pos;
	pos4.w = 1.0;
	pos4 = mul(pos4, World);
	pos4 = mul(pos4, View);
	pos4 = mul(pos4, Projection);


	//頂点カラー
	output.color = input.color;

	// Texture座標指定
	output.texture_pos = input.texture_pos;

	float4 normal;
	// 移動が計算に反映させない
	input.nor.w = 0.0;
	// 頂点の法線にワールド行列を掛け合わせて
	// ワールド座標上での法線の向きに変換する
	normal = mul(input.nor, World).xyzw;
	normal = normalize(normal);
	// saturate => 引数で指定した値を0～1間での範囲に収める
	// dot => 内積計算
	//output.color *= saturate(dot(normal, LightVector));

	//ライトの距離も判定
	float3 light = LightVector - posView;
	float  leng = length(light);
	float bright = 45 * dot(normalize(light), normal) / pow(leng, 2);//ライトの強さ、ライトと法線、長さ
	output.color *= bright;





	return output;
}
