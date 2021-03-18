// �W�I���g�� �V�F�[�_�̓��̓f�[�^��`
struct GS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	// �ǉ�
	float2 texture_pos : TEXTURE0;

};
struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;

	// �ǉ�
	float2 texture_pos : TEXTURE0;

};

// �W�I���g�� �V�F�[�_�̊֐�
[maxvertexcount(3)]
void main(triangle GS_IN input[3],
	inout TriangleStream<PS_IN> TriStream) {
	PS_IN output;

	// �e���_�̌v�Z
	for (int i = 0; i < 3; ++i) {
		output.pos = input[i].pos;
		output.color = input[i].color;
		output.texture_pos = input[i].texture_pos;
		// �o��
		TriStream.Append(output);
	}
	TriStream.RestartStrip();
}