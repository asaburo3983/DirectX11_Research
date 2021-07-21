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

	// ���[���h�~�r���[�~�����ϊ��s��(�V���h�E�}�b�v�p)
}
Texture2D    Texture : register(t0[0]); // Texture���X���b�g0��0�Ԗڂ̃e�N�X�`�����W�X�^�ɐݒ�
Texture2D    TextureSM : register(t0[1]);
Texture2D    TextureView : register(t0[2]);//�r���[���e�N�X�`���ɏ������񂾂���


SamplerState Sampler : register(s0[0]); // Sampler���X���b�g0��0�Ԗڂ̃T���v�����W�X�^�ɐݒ�
SamplerState SamplerSM : register(s0[1]); // Sampler���X���b�g0��0�Ԗڂ̃T���v�����W�X�^�ɐݒ�




float4 main(PS_IN input) : SV_Target
{
	// �e�N�X�`���J���[�̎擾
	float4 tex_color = Texture.Sample(Sampler, input.texture_pos);

	//�e�N�X�`���̂�
	return tex_color;
}
