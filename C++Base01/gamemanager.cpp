//=============================================================================
// 
// �Q�[���}�l�[�W������ [gamemanager.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "gamemanager.h"
#include "game.h"
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "instantfade.h"
#include "player.h"
#include "camera.h"
#include "sound.h"
#include "blackframe.h"
#include "enemybase.h"
#include "enemymanager.h"
#include "stage.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CGameManager::CGameManager()
{
	// �l�̃N���A
	m_SceneType = SCENE_MAIN;	// �V�[���̎��
	m_bEndRush = false;			// ���b�V�����I��������
	m_bControll = false;		// ����ł��邩
	m_bEndNormalStage = false;	// �ʏ�X�e�[�W���I��������
	m_bSetEvolusion = false;	// �i���ݒ肵�Ă邩
	m_nNowStage = 0;			// ���݂̃X�e�[�W
	m_nNumStage = 0;			// �X�e�[�W�̑���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CGameManager::~CGameManager()
{

}

//==========================================================================
// ��������
//==========================================================================
CGameManager *CGameManager::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CGameManager *pManager = NULL;

	if (pManager == NULL)
	{// NULL��������

		// �������̊m��
		pManager = DEBUG_NEW CGameManager;

		if (pManager != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			HRESULT hr = pManager->Init();

			if (FAILED(hr))
			{// ���s���Ă�����
				return NULL;
			}
		}

		return pManager;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CGameManager::Init(void)
{
	m_bControll = true;			// ����ł��邩
	m_bEndNormalStage = false;	// �ʏ�X�e�[�W���I��������
	m_nNowStage = 0;			// ���݂̃X�e�[�W
	m_bSetEvolusion = false;	// �i���ݒ肵�Ă邩
	m_SceneType = SCENE_MAIN;	// �V�[���̎��

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CGameManager::Uninit(void)
{
	
}

//==========================================================================
// �X�V����
//==========================================================================
void CGameManager::Update(void)
{
	if (CGame::GetEnemyBase()->GetNumStage() - 1 <= m_nNowStage)
	{// ���X�e�[�W���𒴂�����
		m_nNowStage = CGame::GetEnemyBase()->GetNumStage() - 1;

		// �ʏ�X�e�[�W���I������
		m_bEndNormalStage = true;
	}

	// ������
	switch (m_SceneType)
	{
	case CGameManager::SCENE_MAIN:
		m_bControll = true;
		break;

	case CGameManager::SCENE_TRANSITIONWAIT:
		m_bControll = false;
		break;

	case CGameManager::SCENE_TRANSITION:
		m_bControll = false;
		break;

	default:
		break;
	}


	// �e�L�X�g�̕`��
	CManager::GetInstance()->GetDebugProc()->Print(
		"---------------- �Q�[���}�l�[�W����� ----------------\n"
		"�y���̃��[�h�z[%d]\n", m_SceneType);


	if (m_SceneType == SCENE_TRANSITION)
	{// �J�ڒ�

		// �J�ڂȂ��t�F�[�h�̏�Ԏ擾
		CInstantFade::STATE fadestate = CManager::GetInstance()->GetInstantFade()->GetState();

		if (fadestate == CInstantFade::STATE_FADECOMPLETION)
		{// ���������u��

			// �J�����擾
			CCamera *pCamera = CManager::GetInstance()->GetCamera();
			if (pCamera != NULL)
			{
				pCamera->SetEnableFollow(true);
			}

			if (m_bEndNormalStage == false)
			{// �ʏ�X�e�[�W���I����Ă��Ȃ�������
				SetEnemy();
			}
			else
			{// �{�X�X�e�[�W
				//SetBoss();
			}

		}
	}

}

//==========================================================================
// �G�ݒ�
//==========================================================================
void CGameManager::SetEnemy(void)
{
	// �v���C���[���
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(nCntPlayer);
		if (pPlayer == NULL)
		{
			continue;
		}

		// �ʒu�ݒ�
		pPlayer->SetPosition(D3DXVECTOR3(-500.0f + nCntPlayer * 250.0f, 0.0f, 0.0f));
	}

	// �J�����̏��擾
	CCamera *pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera != NULL)
	{
		pCamera->Reset(CScene::MODE_GAME);
	}


	// ��ސݒ�
	m_SceneType = SCENE_MAIN;

	// �G�̍Ĕz�u
	CEnemyManager *pEnemyManager = CGame::GetEnemyManager();
	if (pEnemyManager != NULL)
	{
		// �G�̍Ĕz�u
		pEnemyManager->SetStageEnemy();

		// �ύX������Ȃ�����
		pEnemyManager->SetEnableChangeStage(false);
	}
}

//==========================================================================
// �X�e�[�W�̉��Z
//==========================================================================
void CGameManager::AddNowStage(void)
{
	// ���Z
	m_nNowStage++;

	if (CGame::GetEnemyBase()->GetNumStage() <= m_nNowStage)
	{// ���X�e�[�W���𒴂�����
		m_nNowStage = CGame::GetEnemyBase()->GetNumStage();

		// �ʏ�X�e�[�W���I������
		m_bEndNormalStage = true;
	}

}

//==========================================================================
// ���݂̃X�e�[�W�擾
//==========================================================================
int CGameManager::GetNowStage(void)
{
	return m_nNowStage;
}

//==========================================================================
// �V�[���̎�ސݒ�
//==========================================================================
void CGameManager::SetType(SceneType type)
{
	m_SceneType = type;
}

//==========================================================================
// �V�[���̎�ގ擾
//==========================================================================
CGameManager::SceneType CGameManager::GetType(void)
{
	return m_SceneType;
}