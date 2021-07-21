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

	// ワールド×ビュー×透視変換行列(シャドウマップ用)
}
GS_IN main(VS_IN input)
{
	GS_IN output;

	////ボーンによる移動
// 頂点ブレンドの処理
	float4x4 comb = (float4x4)0;
	for (int i = 0; i < 4; i++)
	{
		if (input.boneWeight[i] > 0.0 && input.boneIndex[i] != 39)
			comb += boneMatrix[input.boneIndex[i]] * input.boneWeight[i];
	}
	// ローカル座標 * ワールド座標変換行列

	//MultNormalize
	input.pos.w = 1.0;

	float4 combX;
	combX[0] = -comb[0][0] * input.pos.x;
	combX[1] = -comb[1][0] * input.pos.x;
	combX[2] = -comb[2][0] * input.pos.x;
	combX[3] = -comb[3][0] * input.pos.x;

	float4 combY;
	combY[0] = comb[0][1] * input.pos.y;
	combY[1] = comb[1][1] * input.pos.y;
	combY[2] = comb[2][1] * input.pos.y;
	combY[3] = comb[3][1] * input.pos.y;

	float4 combZ;
	combZ[0] = comb[0][2] * input.pos.z;
	combZ[1] = comb[1][2] * input.pos.z;
	combZ[2] = comb[2][2] * input.pos.z;
	combZ[3] = comb[3][2] * input.pos.z;

	float4 combW;
	combW[0] = comb[0][3] * input.pos.w;
	combW[1] = comb[1][3] * input.pos.w;
	combW[2] = comb[2][3] * input.pos.w;
	combW[3] = comb[3][3] * input.pos.w;

	output.pos.x = combX[0] + combY[0] + combZ[0] + combW[0];
	output.pos.y = combX[1] + combY[1] + combZ[1] + combW[1];
	output.pos.z = combX[2] + combY[2] + combZ[2] + combW[2];
	output.pos.w = combX[3] + combY[3] + combZ[3] + combW[3];

	output.pos.x /= output.pos.w;
	output.pos.y /= output.pos.w;
	output.pos.z /= output.pos.w;
	output.pos.w /= output.pos.w;

	float4 animeChangePos = output.pos;
	output.pos = mul(output.pos, World);
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

	pos4 = animeChangePos;
	//頂点カラー
	output.color = input.color;

	// Texture座標指定
	output.texture_pos = input.texture_pos;

	float4 normal;
	// 移動が計算に反映させない
	input.nor.w = 0.0;
	float4  animeChangeNor;
	combX[0] = -comb[0][0] * input.nor.x;
	combX[1] = -comb[1][0] * input.nor.x;
	combX[2] = -comb[2][0] * input.nor.x;
	combX[3] = -comb[3][0] * input.nor.x;
	combY[0] = comb[0][1] * input.nor.y;
	combY[1] = comb[1][1] * input.nor.y;
	combY[2] = comb[2][1] * input.nor.y;
	combY[3] = comb[3][1] * input.nor.y;
	combZ[0] = comb[0][2] * input.nor.z;
	combZ[1] = comb[1][2] * input.nor.z;
	combZ[2] = comb[2][2] * input.nor.z;
	combZ[3] = comb[3][2] * input.nor.z;
	combW[0] = comb[0][3] * input.nor.w;
	combW[1] = comb[1][3] * input.nor.w;
	combW[2] = comb[2][3] * input.nor.w;
	combW[3] = comb[3][3] * input.nor.w;
	animeChangeNor.x = combX[0] + combY[0] + combZ[0] + combW[0];
	animeChangeNor.y = combX[1] + combY[1] + combZ[1] + combW[1];
	animeChangeNor.z = combX[2] + combY[2] + combZ[2] + combW[2];
	animeChangeNor.w = combX[3] + combY[3] + combZ[3] + combW[3];

	animeChangeNor.x /= animeChangeNor.w;
	animeChangeNor.y /= animeChangeNor.w;
	animeChangeNor.z /= animeChangeNor.w;
	animeChangeNor.w /= animeChangeNor.w;
	// 頂点の法線にワールド行列を掛け合わせて
	// ワールド座標上での法線の向きに変換する
	normal = mul(animeChangeNor, World).xyzw;
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
	pos4 = mul(animeChangePos, mtx);//ライトから見た頂点座標

	pos4.xyz = pos4.xyz / pos4.w;			//正規化
	output.PosSM.x = (pos4.x + 1.0) / 2.0;	//
	output.PosSM.y = (-pos4.y + 1.0) / 2.0;
	output.PosSM.z = pos4.z;


	return output;
}
