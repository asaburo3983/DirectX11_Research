Texture2D<float4> Texture : register(t0);
sampler TextureSampler : register(s0);

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

float4 main(float4 color : COLOR0, float2 texCoord : TEXCOORD0) : SV_Target0
{


   float4 tex_color = Texture.Sample(TextureSampler, texCoord);

  float2 noise = (frac(sin(dot(texCoord, float2(12.9898, 78.233) * 2.0)) * 43758.5453));
 // return abs(noise.x + noise.y) * 0.5;

   return tex_color * tex_color.a* abs(noise.x + noise.y);
}