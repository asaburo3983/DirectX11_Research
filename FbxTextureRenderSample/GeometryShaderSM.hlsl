// ジオメトリ シェーダの入力データ定義
struct GS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	// 追加
	float2 texture_pos : TEXTURE0;

};
struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;

	// 追加
	float2 texture_pos : TEXTURE0;

};

// ジオメトリ シェーダの関数
[maxvertexcount(3)]
void main(triangle GS_IN input[3],
	inout TriangleStream<PS_IN> TriStream) {
	PS_IN output;

	// 各頂点の計算
	for (int i = 0; i < 3; ++i) {
		output.pos = input[i].pos;
		output.color = input[i].color;
		output.texture_pos = input[i].texture_pos;
		// 出力
		TriStream.Append(output);
	}
	TriStream.RestartStrip();
}