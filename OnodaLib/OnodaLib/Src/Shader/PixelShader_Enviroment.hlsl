struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float4 colorNormal : COLOR_NORMAL;
	// �ǉ�
	float2 texture_pos : TEXTURE0;

	float3 PosSM: POSITION_SM; // ���_���W(�V���h�E�}�b�v�̓������W�n)

		//���}�b�v�̔��ˌv�Z�Ŏg�p����
	float3 ViewWorld : POSWORLD;  // ���_���W�x�N�g��(���[���h���W�n)
	float3 NormWorld : NORMWORLD; // �@���x�N�g��(���[���h���W�n)
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
TextureCube  TexCube : register(t0[3]); // �e�N�X�`��(���}�b�v)


SamplerState Sampler : register(s0[0]); // Sampler���X���b�g0��0�Ԗڂ̃T���v�����W�X�^�ɐݒ�
SamplerState SamplerSM : register(s0[1]); // Sampler���X���b�g0��0�Ԗڂ̃T���v�����W�X�^�ɐݒ�



//�L���[�u�}�b�v��\��t����i���ۂɔ��˂��Ă���I�u�W�F�N�g�ɓ\��
float4 main(PS_IN input) : SV_Target
{
	// �e�N�X�`���J���[�̎擾
	float4 tex_color = Texture.Sample(Sampler, input.texture_pos);

	// �V���h�E�}�b�v
	float sm = TextureSM.Sample(SamplerSM, input.PosSM.xy);
	float sma = (input.PosSM.z < sm) ? 1.0 : 0.5;

	// ���}�b�v�v�Z
	float3 E = normalize(input.ViewWorld); // �����x�N�g��
	float3 N = normalize(input.NormWorld); // �@���x�N�g��
	float3 R = reflect(E, N);              // ���˃x�N�g��
	float4 envMap = TexCube.Sample(Sampler, R); // �L���[�u�E�e�N�X�`���ɃA�N�Z�X


	//�}�e���A���ƌ����Ή��i��Ԃ��ꂢ�j
	float ambient_factor = MaterialAmbient[3];
	float diffuse_factor = MaterialDiffuse[3];
	float specular_factor = MaterialSpecular[3];	// �X�y�L�����[
	float4 ambient_color = MaterialAmbient * ambient_factor;
	float4 diffuse_color = input.color * (LightColor * MaterialDiffuse) * diffuse_factor;

	//�X�؃L�����v�Z�i�t�H���V�F�[�f�B���O=�������������Ƃ��낪���˂��Č����j
	float  m_Specular = MaterialSpecular[0];       //�X�y�L�����[�͈̔�  ���f�����O�\�t�g�ŕς���邩�����Ă��Ȃ��A���������Ă��悢
	float  m_SpecularPower = MaterialSpecular[3];  //�X�y�L�����[�̋��x
	

	float3 NS = normalize(input.colorNormal);//�@���x�N�g���𐳋K������
	float3 H = normalize(LightVector + normalize(CameraPos));//���_ -> ���C�g�ʒu�x�N�g�� + ���_ -> ���_�x�N�g��
	float S = pow(max(0.0f, dot(NS, H)), m_Specular) * m_SpecularPower;//�X�y�L�����[�J���[���v�Z����

	float4 specular_color = S;
	specular_color.a = 0;

	//return envMap * tex_color;
	// �F
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
