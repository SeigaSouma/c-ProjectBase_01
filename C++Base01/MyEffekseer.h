//=============================================================================
// 
//  �G�t�F�N�V�A�w�b�_�[ [MyEffekseer.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MYEFFEKSEER_H_
#define _MYEFFEKSEER_H_	// ��d�C���N���[�h�h�~

#include <EffekseerRendererDX9.h>
#include <XAudio2.h>
#include <d3d9.h>
#include <functional>
#include <wrl/client.h>
#include "listmanager.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �G�t�F�N�V�A�N���X��`
class CMyEffekseer
{
public:
	
	struct MyEffekseerInfo
	{
		MyLib::Vector3 move;	// �ړ���
		Effekseer::Handle handle;	// �G�t�F�N�g�̃I�u�W�F�N�g
	};

	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	CMyEffekseer();
	~CMyEffekseer();

	void Uninit(void);
	void Update(void);

	/**
	@brief	�G�t�F�N�g�̐ݒ�
	@details �p�X�̊g���q��[.efkefc]�ł�
	@param	efkpath	[in]	�ǂݍ��ރG�t�F�N�g�̃p�X
	@param	pos		[in]	�ʒu
	@param	rot		[in]	����
	@param	move	[in]	�ړ���
	@param	scale	[in]	�g�嗦
	@return	void
	*/
	void SetEffect(std::string efkpath, MyLib::Vector3 pos, MyLib::Vector3 rot, MyLib::Vector3 move, float scale);
	static CMyEffekseer* GetInstance() { return m_pMyEffekseer; }	// �C���X�^���X�擾
	static CMyEffekseer* Create(void);								// �C���X�^���X����

private:

	// �����o�֐�
	HRESULT Init(void);
	void Draw(void);
	void SetupEffekseerModules(::Effekseer::ManagerRef efkManager);	// ���W���[���̃Z�b�g�A�b�v
	::EffekseerRendererDX9::RendererRef GetEffekseerRenderer() { return efkRenderer; }	// �G�t�F�N�V�A�̃����_���[�擾
	
	// �����o�ϐ�
	int32_t time = 0;
	Effekseer::Handle efkHandle = 0;
	EffekseerRendererDX9::RendererRef efkRenderer;
	Effekseer::ManagerRef efkManager;
	Effekseer::Vector3D viewerPosition;
	Effekseer::Matrix44 projectionMatrix;
	Effekseer::Matrix44 cameraMatrix;
	std::function<void()> onLostDevice;
	std::function<void()> onResetDevice;

	// ����ϐ�
	Effekseer::EffectRef m_LoadEffect;				// �ǂݍ��񂾃G�t�F�N�g�̃X�}�[�g�|�C���^
	std::vector<MyEffekseerInfo> m_EffectObj;		// �G�t�F�N�g�̃I�u�W�F�N�g
	static CMyEffekseer* m_pMyEffekseer;			// ���g�̃|�C���^
};



#endif