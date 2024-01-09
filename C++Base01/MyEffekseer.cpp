//=============================================================================
// 
//  �G�t�F�N�V�A���� [MyEffekseer.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "MyEffekseer.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include <Effekseer.h>

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CMyEffekseer* CMyEffekseer::m_pMyEffekseer = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMyEffekseer::CMyEffekseer()
{
	// �ϐ��̃N���A
	time = 0;
	efkHandle = 0;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMyEffekseer::~CMyEffekseer()
{

}

//==========================================================================
// ��������
//==========================================================================
CMyEffekseer* CMyEffekseer::Create(void)
{
	if (m_pMyEffekseer == NULL)
	{// �܂��������Ă��Ȃ�������

		// �C���X�^���X����
		m_pMyEffekseer = DEBUG_NEW CMyEffekseer;

		// ����������
		m_pMyEffekseer->Init();
	}
	else
	{
		// �C���X�^���X�擾
		m_pMyEffekseer->GetInstance();
	}

	return m_pMyEffekseer;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CMyEffekseer::Init(void)
{
	time = 0;
	efkHandle = 0;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// Effekseer�̃I�u�W�F�N�g�̓X�}�[�g�|�C���^�ŊǗ������B�ϐ����Ȃ��Ȃ�Ǝ����I�ɍ폜�����B

	// �G�t�F�N�g�̃}�l�[�W���[�̍쐬
	efkManager = ::Effekseer::Manager::Create(8000);

	// Effekseer�̃��W���[�����Z�b�g�A�b�v����
	SetupEffekseerModules(efkManager);
	auto efkRenderer = GetEffekseerRenderer();

	// ���W�n��ݒ肷��B�A�v���P�[�V�����ƈ�v������K�v������B
	efkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	// ���_�ʒu���m��
	viewerPosition = ::Effekseer::Vector3D(10.0f, 5.0f, 20.0f);

	// ���e�s���ݒ�
	projectionMatrix.PerspectiveFovRH(90.0f / 180.0f * 3.14f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 500.0f);

	// �J�����s���ݒ�
	cameraMatrix.LookAtRH(viewerPosition, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f));

	return S_OK;
}

//==========================================================================
// �G�t�F�N�g�̐ݒ�
//==========================================================================
void CMyEffekseer::SetEffect(std::string efkpath, MyLib::Vector3 pos, MyLib::Vector3 rot, MyLib::Vector3 move, float scale)
{
	MyEffekseerInfo loacalInfo = {};

	// char16_t�ɕϊ�
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	std::u16string string16t = converter.from_bytes(efkpath);

	// �G�t�F�N�g�̓Ǎ�
	auto effect = Effekseer::Effect::Create(efkManager, string16t.c_str());
	m_LoadEffect = effect;

	onLostDevice = [effect]() -> void
	{
		// �ǂݍ��񂾃G�t�F�N�g�̃��\�[�X�͑S�Ĕj������B
		if (effect != nullptr)
		{
			effect->UnloadResources();
		}
	};

	onResetDevice = [effect]() -> void
	{
		// �G�t�F�N�g�̃��\�[�X���ēǂݍ��݂���B
		if (effect != nullptr)
		{
			effect->ReloadResources();
		}
	};

	// �G�t�F�N�g�̍Đ�
	loacalInfo.handle = efkManager->Play(m_LoadEffect, 0.0f, 0.0f, 0.0f);

	// �������ݒ�
	loacalInfo.move = move;
	efkManager->SetLocation(loacalInfo.handle, pos.x, pos.y, pos.z);
	efkManager->SetRotation(loacalInfo.handle, rot.x, rot.y, rot.z);
	efkManager->SetScale(loacalInfo.handle, scale, scale, scale);

	// ���X�g�Ɋ��蓖��
	m_EffectObj.push_back(loacalInfo);

}

//==========================================================================
// �I������
//==========================================================================
void CMyEffekseer::Uninit(void)
{
	efkManager->StopAllEffects();

	delete m_pMyEffekseer;
	m_pMyEffekseer = nullptr;
}

//==========================================================================
// �X�V����
//==========================================================================
void CMyEffekseer::Update(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	float size = 10.0f;

	for (int i = 0; i < static_cast<int>(m_EffectObj.size()); i++)
	{
		Effekseer::Handle loacalhandle = m_EffectObj[i].handle;

		if (!efkManager->Exists(loacalhandle))
		{// �Đ��I��

			efkManager->StopEffect(loacalhandle);

			// �폜
			m_EffectObj.erase(m_EffectObj.begin() + i);
		}
		else
		{
			// �G�t�F�N�g�̈ړ�
			efkManager->AddLocation(
				loacalhandle,
				Effekseer::Vector3D(m_EffectObj[i].move.x, m_EffectObj[i].move.y, m_EffectObj[i].move.z));
		}
	}

	// ���C���[�p�����[�^�̐ݒ�
	Effekseer::Manager::LayerParameter layerParameter;
	::Effekseer::Matrix44 invEfkCameraMatrix;
	::Effekseer::Matrix44::Inverse(invEfkCameraMatrix, cameraMatrix);
	layerParameter.ViewerPosition = ::Effekseer::Vector3D(invEfkCameraMatrix.Values[3][0], invEfkCameraMatrix.Values[3][1], invEfkCameraMatrix.Values[3][2]);
	efkManager->SetLayerParameter(0, layerParameter);

	// �}�l�[�W���[�̍X�V
	Effekseer::Manager::UpdateParameter updateParameter;
	efkManager->Update(updateParameter);

	// �`�揈��
	Draw();
	time++;
}

//==========================================================================
// �`�揈��
//==========================================================================
void CMyEffekseer::Draw(void)
{
	// ���Ԃ��X�V����
	efkRenderer->SetTime(time / 60.0f);

	// ���e�s���ݒ�
	efkRenderer->SetProjectionMatrix(projectionMatrix);

	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	D3DXMATRIX appProjectionMatrix = pCamera->GetMtxProjection();

	// Effekseer��CameraMatrix�́A�J�����̎p���s��̋t�s����w���B�J�����s�񂪃J�����̎p���s��ł���ꍇ�́A�t�s�񉻂��Ă����B
	auto invAppCameraMatrix = pCamera->GetMtxView();

	// �����̊���Effekseer�́A���e(�ˉe)�s��ƃJ�����s��𓯊�
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			projectionMatrix.Values[i][j] = appProjectionMatrix.m[i][j];
			cameraMatrix.Values[i][j] = invAppCameraMatrix.m[i][j];
		}
	}

	// �J�����s���ݒ�
	efkRenderer->SetCameraMatrix(cameraMatrix);

	// �G�t�F�N�g�̕`��J�n�������s���B
	efkRenderer->BeginRendering();

	// �G�t�F�N�g�̕`����s���B
	Effekseer::Manager::DrawParameter drawParameter;
	drawParameter.ZNear = 0.0f;
	drawParameter.ZFar = 1.0f;
	drawParameter.ViewProjectionMatrix = efkRenderer->GetCameraProjectionMatrix();
	efkManager->Draw(drawParameter);

	// �G�t�F�N�g�̕`��I������
	efkRenderer->EndRendering();
}

#if 0
void CMyEffekseer::ClearScreen()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	pDevice->BeginScene();
}

void CMyEffekseer::PresentDevice()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	pDevice->EndScene();

	{
		HRESULT hr;
		hr = pDevice->Present(NULL, NULL, NULL, NULL);

		switch (hr)
		{
		case D3DERR_DEVICELOST:
			while (FAILED(hr = pDevice->TestCooperativeLevel()))
			{
				switch (hr)
				{
				case D3DERR_DEVICELOST:
					::SleepEx(1000, true);
					break;

				case D3DERR_DEVICENOTRESET:

					// Call it before device lost
					// �f�o�C�X���X�g�̏������s���O�Ɏ��s����
					if (onLostDevice)
					{
						onLostDevice();
					}

					efkRenderer->OnLostDevice();

					D3DPRESENT_PARAMETERS d3dpParams{};
					d3dpParams.BackBufferWidth = SCREEN_WIDTH;
					d3dpParams.BackBufferHeight = SCREEN_HEIGHT;
					d3dpParams.BackBufferFormat = D3DFMT_X8R8G8B8;
					d3dpParams.BackBufferCount = 1;
					d3dpParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
					d3dpParams.Windowed = TRUE;
					//d3dpParams.hDeviceWindow = (HWND)window->GetNativePtr(0);
					d3dpParams.EnableAutoDepthStencil = TRUE;
					d3dpParams.AutoDepthStencilFormat = D3DFMT_D16;
					pDevice->Reset(&d3dpParams);

					// Call it after device lost
					// �f�o�C�X���X�g�̏����̌�Ɏ��s����
					efkRenderer->OnResetDevice();

					if (onResetDevice)
					{
						onResetDevice();
					}

					break;
				}
			}
			break;
		}
	}
}
#endif

//==========================================================================
// ���W���[���̃Z�b�g�A�b�v
//==========================================================================
void CMyEffekseer::SetupEffekseerModules(::Effekseer::ManagerRef efkManager)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �`��f�o�C�X�̍쐬
	auto graphicsDevice = ::EffekseerRendererDX9::CreateGraphicsDevice(pDevice);

	// �G�t�F�N�g�̃����_���[�̍쐬
	efkRenderer = ::EffekseerRendererDX9::Renderer::Create(graphicsDevice, 8000);

	// �`�惂�W���[���̐ݒ�
	efkManager->SetSpriteRenderer(efkRenderer->CreateSpriteRenderer());
	efkManager->SetRibbonRenderer(efkRenderer->CreateRibbonRenderer());
	efkManager->SetRingRenderer(efkRenderer->CreateRingRenderer());
	efkManager->SetTrackRenderer(efkRenderer->CreateTrackRenderer());
	efkManager->SetModelRenderer(efkRenderer->CreateModelRenderer());

	// �e�N�X�`���A���f���A�J�[�u�A�}�e���A�����[�_�[�̐ݒ肷��B
	// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
	efkManager->SetTextureLoader(efkRenderer->CreateTextureLoader());
	efkManager->SetModelLoader(efkRenderer->CreateModelLoader());
	efkManager->SetMaterialLoader(efkRenderer->CreateMaterialLoader());
	efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

	//// Specify sound modules
	//// �T�E���h���W���[���̐ݒ�
	//efkSound = ::EffekseerSound::Sound::Create(GetIXAudio2(), 16, 16);

	//// Specify a metho to play sound from an instance of efkSound
	//// ���Đ��p�C���X�^���X����Đ��@�\���w��
	//efkManager->SetSoundPlayer(efkSound->CreateSoundPlayer());

	//// Specify a sound data loader
	//// It can be extended by yourself. It is loaded from a file on now.
	//// �T�E���h�f�[�^�̓Ǎ��@�\��ݒ肷��B
	//// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
	//efkManager->SetSoundLoader(efkSound->CreateSoundLoader());
}
