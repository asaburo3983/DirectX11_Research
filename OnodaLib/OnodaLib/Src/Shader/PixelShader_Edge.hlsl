struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float4 colorNormal : COLOR_NORMAL;
	// �ǉ�
	float2 texture_pos : TEXTURE0;

	float3 PosSM: POSITION_SM; // ���_���W(�V���h�E�}�b�v�̓������W�n)
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

	matrix SMWorldViewProj;
	float4x4 boneMatrix[50];

	float4 edgeSize;
	float4 edgeColor;

}

float4 main(PS_IN input) : SV_Target
{
	float4 colorR;
	colorR.r=   edgeColor.r;
	colorR.g = edgeColor.g;
	colorR.b = edgeColor.b;
	colorR.a=1;


	return colorR;
}
