Texture2D<float4> Texture : register(t0);
sampler TextureSampler : register(s0);

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


	// ワールド×ビュー×透視変換行列(シャドウマップ用)
}

float4 main(float4 color : COLOR0, float2 texCoord : TEXCOORD0) : SV_Target0
{


   float4 tex_color = Texture.Sample(TextureSampler, texCoord);

  float2 noise = (frac(sin(dot(texCoord, float2(12.9898, 78.233) * 2.0)) * 43758.5453));
 // return abs(noise.x + noise.y) * 0.5;

   return tex_color * tex_color.a* abs(noise.x + noise.y);
}