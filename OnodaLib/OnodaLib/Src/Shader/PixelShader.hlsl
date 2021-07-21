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
	if (tex_color.a <= 0.0)discard;//a�O�l��\������

	// �V���h�E�}�b�v
	float sm = TextureSM.Sample(SamplerSM, input.PosSM.xy);
	float sma = (input.PosSM.z < sm) ? 1.0 : 0.5;



	//�}�e���A���ƌ����Ή��i��Ԃ��ꂢ�j
	float ambient_factor = MaterialAmbient[3];
	float diffuse_factor = MaterialDiffuse[3];
	float specular_factor = MaterialSpecular[3];	// �X�y�L�����[

	float4 ambient_color = MaterialAmbient * ambient_factor;
	float4 diffuse_color = input.color * (LightColor * MaterialDiffuse) * diffuse_factor;

	//�X�؃L�����v�Z�i�t�H���V�F�[�f�B���O=�������������Ƃ��낪���˂��Č����j
	float  m_Specular = MaterialSpecular[0];       //�X�y�L�����[�͈̔�  ���f�����O�\�t�g�ŕς���邩�����Ă��Ȃ��A���������Ă��悢
	float  m_SpecularPower = MaterialSpecular[3];  //�X�y�L�����[�̋��x
	float3 N = normalize(input.colorNormal);//�@���x�N�g���𐳋K������
	float3 H = normalize(LightVector + normalize(CameraPos));//���_ -> ���C�g�ʒu�x�N�g�� + ���_ -> ���_�x�N�g��
	float S = pow(max(0.0f, dot(N, H)), m_Specular) * m_SpecularPower;//�X�y�L�����[�J���[���v�Z����
	
	ambient_color.a = 1;
	diffuse_color.a = 1;
	float4 specular_color=S;
	specular_color.a = 0;

		//�e�N�X�`���̂�
		if (0) {
			return tex_color;
		}
		//�e�̂�
		if (0) {
			return sma;
		}
		//�e�N�X�`���Ɖe �G�ȃg�D�[���V�F�[�f�B���O
		if (0) {
			tex_color = tex_color * sma;
			tex_color.a = 1;
			return tex_color;
		}
		//�}�e���A���ƌ����Ή� �z�̎����Ƃ����o��i�F�����኱�����������Ȃ̂�tex_color�ɒl����Z����Ƃ܂��ɂȂ�j
		if (0) {
			return (ambient_color + diffuse_color * tex_color) * sma;
		}
		//�����Ή�(�Â��ĉ����̂�*2���Ă���) �A�j�����A�F����������Əo��@�����͂Ȃ�
		if (0) {
			return input.color * tex_color * 2;
		}
		//�����Ή��@��������@���}�b�v�Ȃ� ����o����ō��̃O���t�B�b�N
		if (1) {

			return((ambient_color + diffuse_color) * tex_color * sma + specular_color);
		}
	
		//��O
		//return float4(0.5, 0.5, 0.5, 0.5);






}
