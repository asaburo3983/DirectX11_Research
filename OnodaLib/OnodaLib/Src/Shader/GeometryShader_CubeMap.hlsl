// �W�I���g�� �V�F�[�_�̓��̓f�[�^��`
struct GS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float4 colorNormal : COLOR_NORMAL;
	// �ǉ�
	float2 texture_pos : TEXTURE0;

	float3 PosSM: POSITION_SM; // ���_���W(�V���h�E�}�b�v�̓������W�n)
};
struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float4 colorNormal : COLOR_NORMAL;
	// �ǉ�
	float2 texture_pos : TEXTURE0;

	float3 PosSM: POSITION_SM; // ���_���W(�V���h�E�}�b�v�̓������W�n)

	uint RTIndex : SV_RenderTargetArrayIndex; // �`��^�[�Q�b�g
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


	//�L���[�u�}�b�v�p�ϊ��s��
	float4x4 CubeMapView[6];
	float4x4 CubeMapProjection;



	// ���[���h�~�r���[�~�����ϊ��s��(�V���h�E�}�b�v�p)
}
// �W�I���g�� �V�F�[�_�̊֐�
[maxvertexcount(18)]
void main(triangle GS_IN input[3],
	inout TriangleStream<PS_IN> TriStream) {


	//�����Œ��_�����炵�Ă邩�m��񂯂ǃ|���S���A���������Ȃ�イ
	// 
	// 
	// �e���_�̌v�Z
	for (int h = 0; h < 6; h++) {
		PS_IN output;
		output.RTIndex = h;  // ���̃v���~�e�B�u��`�悷��`��^�[�Q�b�g
		for (int i = 0; i < 3; ++i) {

			//�J�������ƂɃr���[�A�ˉe�ϊ����K�v
			output.pos = input[i].pos;
			output.pos = mul(output.pos, World);
			output.pos = mul(output.pos, CubeMapView[h]);	// ���[���h���W * �r���[���W�ϊ��s��
			output.pos = mul(output.pos, CubeMapProjection);// �r���[���W * �v���W�F�N�V�������W�ϊ��s��

			output.color = input[i].color;
			output.colorNormal = input[i].colorNormal;

			output.texture_pos = input[i].texture_pos;
			output.PosSM = input[i].PosSM;

			// �o��
			TriStream.Append(output);
		}
		TriStream.RestartStrip();
	}
	
}