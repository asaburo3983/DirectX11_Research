struct GS_INPUT_SV {
	float3 Pos : SV_POSITION;
};

struct PS_INPUT_SV {
	float4 Pos : SV_POSITION;
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

// ジオメトリ・シェーダの関数②
[maxvertexcount(12)]
void main(triangle float3 input[3] : POSITION,
	inout TriangleStream<PS_INPUT_SV> TriStream) {
	PS_INPUT_SV output;

	// 法線ベクトルの計算
	float3 faceNorm = cross(input[1] - input[0], input[2] - input[0]);

	float3 Light = LightVector + (input[0] + input[1] + input[2]) / 3.0;

	// 光源ベクトルの計算
	float3 lightVec = LightVector;

	// 光源を向いている？
	if (dot(faceNorm, lightVec) > 0.0)
		return;

	// 各辺を引き延ばす
	for (uint i1 = 0; i1 < 3; ++i1) {
		uint i2 = (i1 + 1) % 3, i3 = (i1 + 2) % 3;
		// シャドウ・ボリュームを生成
		output.Pos = mul(float4(input[i1], 1.0), Projection);
		TriStream.Append(output);
		output.Pos = mul(float4(input[i3], 1.0), Projection);
		TriStream.Append(output);
		output.Pos = mul(float4(input[i1] + (input[i1] - Light) * 100, 1.0), Projection);
		TriStream.Append(output);
		output.Pos = mul(float4(input[i3] + (input[i3] - Light) * 100, 1.0), Projection);
		TriStream.Append(output);
		TriStream.RestartStrip();
	}
}
