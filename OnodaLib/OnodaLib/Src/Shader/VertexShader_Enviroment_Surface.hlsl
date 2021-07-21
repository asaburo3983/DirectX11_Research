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

	float4 colorNormal : COLOR_NORMAL;
	// 追加
	float2 texture_pos : TEXTURE0;

	float3 PosSM: POSITION_SM; // 頂点座標(シャドウマップの透視座標系)

	//環境マップの反射計算で使用する
	float3 ViewWorld : POSWORLD;  // 頂点座標ベクトル(ワールド座標系)
	float3 NormWorld : NORMWORLD; // 法線ベクトル(ワールド座標系)
};


//渡す際にCPU側の書き方と順番が違うとメモリが書きつぶされて意図しない結果が出るので気をつけましょう
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

GS_IN main(VS_IN input)
{
	GS_IN output;

	output.pos = mul(input.pos, World);
	// ワールド座標 * ビュー座標変換行列
	output.pos = mul(output.pos, View);
	// ビュー座標 * プロジェクション座標変換行列
	output.pos = mul(output.pos, Projection);

	// シャドウマップ用頂点座標

	float4 pos4 = input.pos;

	// 頂点座標　モデル座標系→透視座標系(シャドウマップ)



	//output.pos= mul(input.pos, SMWorldViewProj);
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
	float3 light = LightVector - output.pos;
	float  leng = length(light);

	float bright = 45 * dot(normalize(light), normal) / pow(leng, 2);//ライトの強さ、ライトと法線、長さ
	output.color *= bright;



	float4x4 mtx = mul(World, SMWorldViewProj);
	pos4 = mul(input.pos, mtx);//ライトから見た頂点座標

	pos4.xyz = pos4.xyz / pos4.w;			//正規化
	output.PosSM.x = (pos4.x + 1.0) / 2.0;	//
	output.PosSM.y = (-pos4.y + 1.0) / 2.0;
	output.PosSM.z = pos4.z;



	float4 normal2;
	// 移動が計算に反映させない
	input.nor.w = 0.0;

	normal2 = mul(input.pos, World);
	output.colorNormal = normal2;//モデルの法線を表示しているだけ、カメラから見た法線が欲しい　できてる？

	float4 pos2;
	pos2 = input.pos;
	pos2 = mul(input.pos, World);
	pos2 = mul(pos2, View);
	pos2 = mul(pos2, Projection);

	
	normal2 = mul(input.nor, World);
	//normal2 = mul(input.nor, World);

	output.ViewWorld = pos2 - CameraPos;//  位置から視線ベクトルを減算している
	output.NormWorld = normal2;//法線を渡す

	return output;
}
