struct GS_INPUT_SV {
	float3 Pos : SV_POSITION;
};

struct PS_INPUT_SV {
	float4 Pos : SV_POSITION;
};
cbuffer ConstantBuffer
{
	float4x4	World;				// ���[���h�ϊ��s��
	float4x4	View;				// �r���[�ϊ��s��
	float4x4	Projection;			// �����ˉe�ϊ��s��
	float4		CameraPos;			// �J�������W
	float4		LightVector;		// ���C�g����
	float4		LightColor;			// ���C�g�J���[
	float4		MaterialAmbient;	// �A���r�G���g
	float4		MaterialDiffuse;	// �f�B�t���[�Y
	float4		MaterialSpecular;	// �X�y�L�����[

	float4x4 boneMatrix[50];
}

// �W�I���g���E�V�F�[�_�̊֐��A
[maxvertexcount(12)]
void main(triangle float3 input[3] : POSITION,
	inout TriangleStream<PS_INPUT_SV> TriStream) {
	PS_INPUT_SV output;

	// �@���x�N�g���̌v�Z
	float3 faceNorm = cross(input[1] - input[0], input[2] - input[0]);

	float3 Light = LightVector + (input[0] + input[1] + input[2]) / 3.0;

	// �����x�N�g���̌v�Z
	float3 lightVec = LightVector;

	// �����������Ă���H
	if (dot(faceNorm, lightVec) > 0.0)
		return;

	// �e�ӂ��������΂�
	for (uint i1 = 0; i1 < 3; ++i1) {
		uint i2 = (i1 + 1) % 3, i3 = (i1 + 2) % 3;
		// �V���h�E�E�{�����[���𐶐�
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
