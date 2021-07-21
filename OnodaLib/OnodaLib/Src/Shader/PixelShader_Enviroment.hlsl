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

	// ワールド×ビュー×透視変換行列(シャドウマップ用)
}
Texture2D    Texture : register(t0[0]); // Textureをスロット0の0番目のテクスチャレジスタに設定
Texture2D    TextureSM : register(t0[1]);
Texture2D    TextureView : register(t0[2]);//ビューをテクスチャに書き込んだもの
TextureCube  TexCube : register(t0[3]); // テクスチャ(環境マップ)


SamplerState Sampler : register(s0[0]); // Samplerをスロット0の0番目のサンプラレジスタに設定
SamplerState SamplerSM : register(s0[1]); // Samplerをスロット0の0番目のサンプラレジスタに設定



//キューブマップを貼り付ける（実際に反射しているオブジェクトに貼る
float4 main(PS_IN input) : SV_Target
{
	// テクスチャカラーの取得
	float4 tex_color = Texture.Sample(Sampler, input.texture_pos);

	// シャドウマップ
	float sm = TextureSM.Sample(SamplerSM, input.PosSM.xy);
	float sma = (input.PosSM.z < sm) ? 1.0 : 0.5;

	// 環境マップ計算
	float3 E = normalize(input.ViewWorld); // 視線ベクトル
	float3 N = normalize(input.NormWorld); // 法線ベクトル
	float3 R = reflect(E, N);              // 反射ベクトル
	float4 envMap = TexCube.Sample(Sampler, R); // キューブ・テクスチャにアクセス


	//マテリアルと光源対応（一番きれい）
	float ambient_factor = MaterialAmbient[3];
	float diffuse_factor = MaterialDiffuse[3];
	float specular_factor = MaterialSpecular[3];	// スペキュラー
	float4 ambient_color = MaterialAmbient * ambient_factor;
	float4 diffuse_color = input.color * (LightColor * MaterialDiffuse) * diffuse_factor;

	//スぺキュラ計算（フォンシェーディング=光があたったところが反射して光るよ）
	float  m_Specular = MaterialSpecular[0];       //スペキュラーの範囲  モデリングソフトで変えれるか試していない、実数を入れてもよい
	float  m_SpecularPower = MaterialSpecular[3];  //スペキュラーの強度
	

	float3 NS = normalize(input.colorNormal);//法線ベクトルを正規化する
	float3 H = normalize(LightVector + normalize(CameraPos));//頂点 -> ライト位置ベクトル + 頂点 -> 視点ベクトル
	float S = pow(max(0.0f, dot(NS, H)), m_Specular) * m_SpecularPower;//スペキュラーカラーを計算する

	float4 specular_color = S;
	specular_color.a = 0;

	//return envMap * tex_color;
	// 色
	//envMap = envMap / 2;
	//envMap.a = 1;

	ambient_color.a = 1;
	diffuse_color.a = 1;

	return((ambient_color + diffuse_color) * tex_color * envMap) +specular_color;

	return envMap*tex_color;
	return envMap;
	return saturate(envMap + tex_color);

	return tex_color;
}
