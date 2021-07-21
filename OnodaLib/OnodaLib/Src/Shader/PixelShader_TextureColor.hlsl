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

	// ワールド×ビュー×透視変換行列(シャドウマップ用)
}
Texture2D    Texture : register(t0[0]); // Textureをスロット0の0番目のテクスチャレジスタに設定
Texture2D    TextureSM : register(t0[1]);
Texture2D    TextureView : register(t0[2]);//ビューをテクスチャに書き込んだもの


SamplerState Sampler : register(s0[0]); // Samplerをスロット0の0番目のサンプラレジスタに設定
SamplerState SamplerSM : register(s0[1]); // Samplerをスロット0の0番目のサンプラレジスタに設定




float4 main(PS_IN input) : SV_Target
{
	// テクスチャカラーの取得
	float4 tex_color = Texture.Sample(Sampler, input.texture_pos);

	//テクスチャのみ
	return tex_color;
}
