#include "EffekseerLib.h"
#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }	// ����}�N��

EffekseerRendererDX11::Renderer* renderer;
Effekseer::Manager* manager;
Effekseer::Effect* effect;
Effekseer::Handle handle = 0;

ID3D11Device* tmpDevice;
ID3D11DeviceContext* tmpDeviceCon;
void EffekseerInit() {
	// �G�t�F�N�g�̃����_���[�̍쐬
	tmpDevice=DirectGraphics::GetInstance()->GetDevice();
	tmpDeviceCon=DirectGraphics::GetInstance()->GetContext();
	renderer = ::EffekseerRendererDX11::Renderer::Create(tmpDevice, tmpDeviceCon, 8000);
	// �G�t�F�N�g�̃}�l�[�W���[�̍쐬
	manager = ::Effekseer::Manager::Create(8000);
	// �`�惂�W���[���̐ݒ�
	manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
	manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
	manager->SetRingRenderer(renderer->CreateRingRenderer());
	manager->SetTrackRenderer(renderer->CreateTrackRenderer());
	manager->SetModelRenderer(renderer->CreateModelRenderer());
	// �e�N�X�`���A���f���A�}�e���A�����[�_�[�̐ݒ肷��B
	// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
	manager->SetTextureLoader(renderer->CreateTextureLoader());
	manager->SetModelLoader(renderer->CreateModelLoader());
	manager->SetMaterialLoader(renderer->CreateMaterialLoader());

	// Specify a position of view
	// ���_�ʒu���m��
	auto g_position = ::Effekseer::Vector3D(0.0f, 0.0f, 20.0f);

	// Specify a projection matrix
	// ���e�s���ݒ�
	renderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovLH(
		90.0f / 180.0f * 3.14f, (float)640 / (float)480, 0.1f, 1000.0f));

	// Specify a camera matrix
	// �J�����s���ݒ�
	renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(g_position, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));

	// �G�t�F�N�g�̓Ǎ�
	char16_t s16[] = u"Laser.efk";
	effect = Effekseer::Effect::Create(manager, s16);

	int32_t time = 0;

}

int time = 0;
void EffekseerDraw() {


	if (time % 120 == 0)
	{
		// Play an effect
		// �G�t�F�N�g�̍Đ�
		handle = manager->Play(effect, 0, 0, 0);
	}

	if (time % 120 == 119)
	{
		// Stop effects
		// �G�t�F�N�g�̒�~
		manager->StopEffect(handle);
	}
	manager->AddLocation(handle, Effekseer::Vector3D(0.2f, 0.0f, 0.0f));
	// Update the manager
	// �}�l�[�W���[�̍X�V
	manager->Update();

	// Begin to rendering effects
	// �G�t�F�N�g�̕`��J�n�������s���B
	renderer->BeginRendering();

	// Render effects
	// �G�t�F�N�g�̕`����s���B
	manager->Draw();

	// Finish to rendering effects
	// �G�t�F�N�g�̕`��I���������s���B
	renderer->EndRendering();

	time++;
}

void EffekseerUnInit() {
	renderer->Destroy();
	// Release effects
// �G�t�F�N�g�̉��
	SAFE_RELEASE(effect);

	// Dispose the manager
	// �}�l�[�W���[�̔j��
	manager->Destroy();
}