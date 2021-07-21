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
	if (tex_color.a <= 0.0)discard;//a０値非表示処理

	// シャドウマップ
	float sm = TextureSM.Sample(SamplerSM, input.PosSM.xy);
	float sma = (input.PosSM.z < sm) ? 1.0 : 0.5;



	//マテリアルと光源対応（一番きれい）
	float ambient_factor = MaterialAmbient[3];
	float diffuse_factor = MaterialDiffuse[3];
	float specular_factor = MaterialSpecular[3];	// スペキュラー

	float4 ambient_color = MaterialAmbient * ambient_factor;
	float4 diffuse_color = input.color * (LightColor * MaterialDiffuse) * diffuse_factor;

	//スぺキュラ計算（フォンシェーディング=光があたったところが反射して光るよ）
	float  m_Specular = MaterialSpecular[0];       //スペキュラーの範囲  モデリングソフトで変えれるか試していない、実数を入れてもよい
	float  m_SpecularPower = MaterialSpecular[3];  //スペキュラーの強度
	float3 N = normalize(input.colorNormal);//法線ベクトルを正規化する
	float3 H = normalize(LightVector + normalize(CameraPos));//頂点 -> ライト位置ベクトル + 頂点 -> 視点ベクトル
	float S = pow(max(0.0f, dot(N, H)), m_Specular) * m_SpecularPower;//スペキュラーカラーを計算する
	
	ambient_color.a = 1;
	diffuse_color.a = 1;
	float4 specular_color=S;
	specular_color.a = 0;

		//テクスチャのみ
		if (0) {
			return tex_color;
		}
		//影のみ
		if (0) {
			return sma;
		}
		//テクスチャと影 雑なトゥーンシェーディング
		if (0) {
			tex_color = tex_color * sma;
			tex_color.a = 1;
			return tex_color;
		}
		//マテリアルと光源対応 布の質感とかも出る（色見が若干きもいかもなのでtex_colorに値を乗算するとましになる）
		if (0) {
			return (ambient_color + diffuse_color * tex_color) * sma;
		}
		//光源対応(暗くて汚いので*2している) アニメ調、色がくっきりと出る　質感はない
		if (0) {
			return input.color * tex_color * 2;
		}
		//光源対応　質感あり　環境マップなし 現状出せる最高のグラフィック
		if (1) {

			return((ambient_color + diffuse_color) * tex_color * sma + specular_color);
		}
	
		//例外
		//return float4(0.5, 0.5, 0.5, 0.5);






}
