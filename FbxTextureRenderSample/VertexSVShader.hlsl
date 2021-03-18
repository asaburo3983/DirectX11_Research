// ���_�V�F�[�_�̓��̓f�[�^��`
struct VS_INPUT {

	float4 Pos : POSITION0;
	float4 Norm : NORMAL0;
	float2 Tex : TEXTURE0;
	float4 Color : COLOR0;

	int4 boneIndex :  BONEINDEX;
	float4 boneWeight : BONEWEIGHT;
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
// ���_�V�F�[�_�̊֐�
float3 main(VS_INPUT input) : POSITION{
	// ���_���W�@���f�����W�n���r���[���W�n
	input.Pos.w = 1.0;
	float4 Pos = input.Pos;
	Pos = mul(Pos, World);
	Pos = mul(Pos, View);
	return Pos.xyz / Pos.w;
}
