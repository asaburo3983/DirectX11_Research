// 頂点シェーダの関数
float4 main(uint id : SV_VertexID) : SV_POSITION{

	switch (id) {
	case 0: return float4(-1,  1, 1, 1);
	case 1: return float4(1,  1, 1, 1);
	case 2: return float4(-1, -1, 1, 1);
	default:
	case 3: return float4(1, -1, 1, 1);
	}
}