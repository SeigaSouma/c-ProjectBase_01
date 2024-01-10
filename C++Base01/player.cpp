//=============================================================================
// 
//  �v���C���[���� [player.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "player.h"
#include "game.h"
#include "camera.h"
#include "manager.h"
#include "debugproc.h"
#include "renderer.h"
#include "input.h"
#include "enemy.h"
#include "calculation.h"
#include "score.h"
#include "texture.h"
#include "Xload.h"
#include "model.h"
#include "hp_gauge.h"
#include "elevation.h"
#include "shadow.h"
#include "particle.h"
#include "3D_Effect.h"
#include "ballast.h"
#include "impactwave.h"
#include "sound.h"
#include "enemymanager.h"
#include "bullet.h"
#include "stage.h"
#include "objectX.h"
#include "instantfade.h"
#include "fade.h"
#include "listmanager.h"
#include "object_circlegauge2D.h"
#include "collisionobject.h"
#include "limitarea.h"
#include "beam.h"
#include "santabag.h"
#include "meshsphere.h"
#include "particle.h"
#include "MyEffekseer.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* CHARAFILE = "data\\TEXT\\character\\player\\tyuuniplayer\\setup_player.txt";	// �L�����N�^�[�t�@�C��
	const float JUMP = 20.0f * 1.5f;	// �W�����v�͏����l
	const int INVINCIBLE_INT = 2;		// ���G�̊Ԋu
	const int INVINCIBLE_TIME = 90;		// ���G�̎���
	const int DEADTIME = 120;			// ���S���̎���
	const int FADEOUTTIME = 60;			// �t�F�[�h�A�E�g�̎���
	const int MAX_ATKCOMBO = 2;			// �U���R���{�̍ő吔
	const int INTERVAL_ATK = 15;		// �U���̗P�\
	const int MAX_BUFFSTATUS = 100;		// �X�e�[�^�X�̃o�t�ő�l
	const float MAX_BALL_SIZE = 40.0f;	// ��ʂ̍ő�T�C�Y
	const float SPEED_GRAW_BALL = 1.1f;	// ��ʂ̐������x
	const float LINE_ICE = 30.0f;	// �X���ɂȂ郉�C��
	const float RADIUS_STAGE = 5000.0f;	// �X�e�[�W�̔��a
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayer::CPlayer(int nPriority) : CObjectChara(nPriority)
{
	// �l�̃N���A
	// ���L�ϐ�
	m_bJump = false;					// �W�����v�����ǂ���
	m_bLandOld = false;					// �ߋ��̒��n���
	m_bHitStage = false;				// �X�e�[�W�̓����蔻��
	m_bLandField = false;				// �t�B�[���h�̒��n����
	m_bHitWall = false;					// �ǂ̓����蔻��
	m_nCntWalk = 0;						// ���s�J�E���^�[
	m_state = STATE_NONE;				// ���
	m_sMotionFrag.bATK = false;		// ���[�V�����̃t���O
	m_sMotionFrag.bATK = false;		// ���[�V�����̃t���O
	m_sMotionFrag.bJump = false;		// ���[�V�����̃t���O
	m_sMotionFrag.bKnockBack = false;	// ���[�V�����̃t���O
	m_sMotionFrag.bDead = false;		// ���[�V�����̃t���O
	m_sMotionFrag.bMove = false;		// ���[�V�����̃t���O

	// �v���C�x�[�g�ϐ�
	m_Oldstate = STATE_NONE;						// �O��̏��
	m_mMatcol = mylib_const::DEFAULT_COLOR;			// �}�e���A���̐F
	m_posKnokBack = mylib_const::DEFAULT_VECTOR3;	// �m�b�N�o�b�N�̈ʒu
	m_KnokBackMove = mylib_const::DEFAULT_VECTOR3;	// �m�b�N�o�b�N�̈ړ���
	m_nCntState = 0;								// ��ԑJ�ڃJ�E���^�[
	m_nMyPlayerIdx = 0;								// �v���C���[�C���f�b�N�X�ԍ�
	m_pShadow = NULL;								// �e�̏��
	m_pTargetP = NULL;								// �ڕW�̒n�_
	WeaponHandle = 0;	// �G�t�F�N�g�̕���n���h��
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayer::~CPlayer()
{

}

//==========================================================================
// ��������
//==========================================================================
CPlayer *CPlayer::Create(int nIdx)
{
	// �����p�̃I�u�W�F�N�g
	CPlayer *pPlayer = NULL;

	if (pPlayer == NULL)
	{// NULL��������

		// �������̊m��
		pPlayer = DEBUG_NEW CPlayer;

		if (pPlayer != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// �v���C���[�C���f�b�N�X�ԍ�
			pPlayer->m_nMyPlayerIdx = nIdx;

			// ����������
			pPlayer->Init();
		}

		return pPlayer;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CPlayer::Init(void)
{
	// ��ނ̐ݒ�
	SetType(TYPE_PLAYER);

	m_state = STATE_NONE;	// ���
	m_nCntState = 0;		// ��ԑJ�ڃJ�E���^�[
	m_bLandOld = true;		// �O��̒��n���

	// �L�����쐬
	HRESULT hr = SetCharacter(CHARAFILE);
	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	// �e�̐���
	//m_pShadow = CShadow::Create(pos, 50.0f);
	m_pObjX = CObjectX::Create("data\\MODEL\\aaaa.x");

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayer::Uninit(void)
{

	// �e������
	if (m_pShadow != NULL)
	{
		//m_pShadow->Uninit();
		m_pShadow = NULL;
	}

	// �I������
	CObjectChara::Uninit();

	// ���[�h�ʏI������
	UninitByMode();
}

//==========================================================================
// ���[�h�ʏI������
//==========================================================================
void  CPlayer::UninitByMode(void)
{
	CScene *pScene = CManager::GetInstance()->GetScene();
	if (pScene != NULL)
	{
		// �v���C���[��NULL
		CManager::GetInstance()->GetScene()->UninitPlayer(m_nMyPlayerIdx);
	}
}

//==========================================================================
// �I������
//==========================================================================
void CPlayer::Kill(void)
{

	my_particle::Create(GetPosition(), my_particle::TYPE_ENEMY_FADE);

	// �e������
	if (m_pShadow != NULL)
	{
		m_pShadow->Uninit();
		m_pShadow = NULL;
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayer::Update(void)
{
	if (IsDeath() == true)
	{
		return;
	}

	// �L�[�{�[�h���擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	if (pInputKeyboard->GetTrigger(DIK_F5) == true)
	{// F5�Ń��Z�b�g
		SetPosition(MyLib::Vector3(0.0f, 0.0f, -1000.0f));
		SetMove(MyLib::Vector3(0.0f, 0.0f, 0.0f));
	}

	// �G�f�B�b�g���͔�����
	if (CGame::GetElevation()->IsEdit())
	{
		return;
	}

	// �G�f�B�b�g���͔�����
	if (CGame::GetEditType() != CGame::EDITTYPE_OFF)
	{
		return;
	}

	// �ߋ��̈ʒu�ۑ�
	SetOldPosition(GetPosition());

	// �e�̍X�V����
	CObjectChara::Update();

	// ����
	Controll();

	// ���[�V�����̐ݒ菈��
	MotionSet();

	// ��ԍX�V
	UpdateState();

	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 posCenter = GetCenterPosition();

	// �ړ��ʎ擾
	MyLib::Vector3 move = GetMove();

	// �����擾
	MyLib::Vector3 rot = GetRotation();

	// �J�����̏��擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	pCamera->SetTargetPosition(pos);
	pCamera->SetTargetRotation(rot);

	// �e�̈ʒu�X�V
	if (m_pShadow != NULL)
	{
		m_pShadow->SetPosition(MyLib::Vector3(pos.x, m_pShadow->GetPosition().y, pos.z));
	}



	int nCntEffect = 0;
	int nNumEffect = GetEffectParentNum();
	for (int i = 0; i < mylib_const::MAX_OBJ; i++)
	{
		CEffect3D* pEffect = GetEffectParent(i);
		if (pEffect == NULL)
		{// NULL��������
			continue;
		}

		// �G�t�F�N�g�̈ʒu�X�V
		pEffect->UpdatePosition(GetRotation());
		nCntEffect++;
		if (nNumEffect <= nCntEffect)
		{
			break;
		}
	}

	// �ʒu�̐���
	LimitPos();

#if 0
	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(
		"------------------[�v���C���[�̑���]------------------\n"
		"�ʒu�F�yX�F%f, Y�F%f, Z�F%f�z�yX�F%f, Y�F%f, Z�F%f�z �yW / A / S / D�z\n"
		"�����F�yX�F%f, Y�F%f, Z�F%f�z �yZ / C�z\n"
		"�ړ��ʁF�yX�F%f, Y�F%f, Z�F%f�z\n"
		"�̗́F�y%d�z\n", pos.x, pos.y, pos.z, posCenter.x, posCenter.y, posCenter.z, rot.x, rot.y, rot.y, move.x, move.y, move.z, GetLife());
#endif

}

//==========================================================================
// ���쏈��
//==========================================================================
void CPlayer::Controll(void)
{

	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	// �J�����̏��擾
	CCamera *pCamera = CManager::GetInstance()->GetCamera();

	// �J�����̌����擾
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 newPosition = GetPosition();
	MyLib::Vector3 sakiPos = GetPosition();

	// �ړ��ʎ擾
	MyLib::Vector3 move = GetMove();

	// �����擾
	MyLib::Vector3 rot = GetRotation();

	// �ڕW�̌����擾
	float fRotDest = GetRotDest();

	// ���݂̎�ގ擾
	CMotion* pMotion = GetMotion();
	int nMotionType = pMotion->GetType();

	// �ړ��ʎ擾
	float fMove = GetVelocity();

	// �o�ߎ��Ԏ擾
	float fCurrentTime = CManager::GetInstance()->GetDeltaTime();
	

	if (CGame::GetGameManager()->IsControll())
	{// �s���ł���Ƃ�

		if (pMotion->IsGetMove(nMotionType) == 1 &&
			m_state != STATE_DMG &&
			m_state != STATE_KNOCKBACK &&
			m_state != STATE_DEAD &&
			m_state != STATE_FADEOUT)
		{// �ړ��\���[�V�����̎�

			if (pInputKeyboard->GetPress(DIK_A) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).x < 0)
			{//���L�[�������ꂽ,���ړ�

				// �ړ����ɂ���
				m_sMotionFrag.bMove = true;

				if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y > 0)
				{//A+W,����ړ�

					move.x += sinf(-D3DX_PI * 0.25f + Camerarot.y) * fMove;
					move.z += cosf(-D3DX_PI * 0.25f + Camerarot.y) * fMove;
					fRotDest = D3DX_PI * 0.75f + Camerarot.y;
				}
				else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y < 0)
				{//A+S,�����ړ�

					move.x += sinf(-D3DX_PI * 0.75f + Camerarot.y) * fMove;
					move.z += cosf(-D3DX_PI * 0.75f + Camerarot.y) * fMove;
					fRotDest = D3DX_PI * 0.25f + Camerarot.y;
				}
				else
				{//A,���ړ�

					move.x += sinf(-D3DX_PI * 0.5f + Camerarot.y) * fMove;
					move.z += cosf(-D3DX_PI * 0.5f + Camerarot.y) * fMove;
					fRotDest = D3DX_PI * 0.5f + Camerarot.y;
				}
			}
			else if (pInputKeyboard->GetPress(DIK_D) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).x > 0)
			{//D�L�[�������ꂽ,�E�ړ�

				// �ړ����ɂ���
				m_sMotionFrag.bMove = true;

				if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y > 0)
				{//D+W,�E��ړ�

					move.x += sinf(D3DX_PI * 0.25f + Camerarot.y) * fMove;
					move.z += cosf(D3DX_PI * 0.25f + Camerarot.y) * fMove;
					fRotDest = -D3DX_PI * 0.75f + Camerarot.y;
				}
				else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y < 0)
				{//D+S,�E���ړ�

					move.x += sinf(D3DX_PI * 0.75f + Camerarot.y) * fMove;
					move.z += cosf(D3DX_PI * 0.75f + Camerarot.y) * fMove;
					fRotDest = -D3DX_PI * 0.25f + Camerarot.y;
				}
				else
				{//D,�E�ړ�

					move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
					move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
					fRotDest = -D3DX_PI * 0.5f + Camerarot.y;
				}
			}
			else if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y > 0)
			{//W�������ꂽ�A��ړ�

				// �ړ����ɂ���
				m_sMotionFrag.bMove = true;
				move.x += sinf(D3DX_PI * 0.0f + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 0.0f + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 1.0f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y < 0)
			{//S�������ꂽ�A���ړ�

				// �ړ����ɂ���
				m_sMotionFrag.bMove = true;
				move.x += sinf(D3DX_PI * 1.0f + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 1.0f + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 0.0f + Camerarot.y;
			}
			else
			{
				// �ړ������ǂ���
				m_sMotionFrag.bMove = false;
			}

		}
		else if (pMotion->IsGetMove(nMotionType) == 0 &&
			m_state != STATE_DEAD &&
			m_state != STATE_FADEOUT)
		{
			if (pInputKeyboard->GetPress(DIK_A) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).x < 0)
			{//���L�[�������ꂽ,���ړ�

				if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y > 0)
				{//A+W,����ړ�
					fRotDest = D3DX_PI * 0.75f + Camerarot.y;
				}
				else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y < 0)
				{//A+S,�����ړ�
					fRotDest = D3DX_PI * 0.25f + Camerarot.y;
				}
				else
				{//A,���ړ�
					fRotDest = D3DX_PI * 0.5f + Camerarot.y;
				}
			}
			else if (pInputKeyboard->GetPress(DIK_D) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).x > 0)
			{//D�L�[�������ꂽ,�E�ړ�

				if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y > 0)
				{//D+W,�E��ړ�
					fRotDest = -D3DX_PI * 0.75f + Camerarot.y;
				}
				else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y < 0)
				{//D+S,�E���ړ�
					fRotDest = -D3DX_PI * 0.25f + Camerarot.y;
				}
				else
				{//D,�E�ړ�
					fRotDest = -D3DX_PI * 0.5f + Camerarot.y;
				}
			}
			else if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y > 0)
			{//W�������ꂽ�A��ړ�
				fRotDest = D3DX_PI * 1.0f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y < 0)
			{//S�������ꂽ�A���ړ�
				fRotDest = D3DX_PI * 0.0f + Camerarot.y;
			}
		}
	}

	// �ړ��ʉ��Z
	newPosition.x += move.x;
	newPosition.z += move.z;

	sakiPos.x = newPosition.x + sinf(D3DX_PI + rot.y) * GetRadius();
	sakiPos.z = newPosition.z + cosf(D3DX_PI + rot.y) * GetRadius();

	// �p�x�̐��K��
	UtilFunc::Transformation::RotNormalize(fRotDest);
	SetRotDest(fRotDest);

	// ���݂ƖڕW�̍��������߂�
	float fRotDiff = fRotDest - rot.y;

	// �p�x�̐��K��
	UtilFunc::Transformation::RotNormalize(fRotDiff);

	// �p�x�̕␳������
	rot.y += fRotDiff * 0.15f;
	UtilFunc::Transformation::RotNormalize(rot.y);

	// �����ݒ�
	SetRotation(rot);

	// �d�͏���
	if (m_state != STATE_KNOCKBACK && m_state != STATE_DMG && m_state != STATE_DEAD && m_state != STATE_FADEOUT)
	{
		move.y -= mylib_const::GRAVITY;

		// �ʒu�X�V
		newPosition.y += move.y;
		sakiPos.y = newPosition.y;
	}

	//**********************************
	// �����蔻��
	//**********************************
	bool bLandStage = Collision(sakiPos, move);

	bool bMove = false;
	if (m_bLandOld == false && bLandStage == true)
	{// �O��͒��n���Ă��Ȃ��āA����͒��n���Ă���ꍇ

		bMove = false;
	}

	if (m_bLandOld == true && bLandStage == true)
	{// �O�����������n���Ă���ꍇ
		bMove = true;
	}

	if (m_bHitWall == false && 
		(bLandStage == false || bMove == true || m_bLandField == true || m_bJump == true || m_sMotionFrag.bKnockBack == true || m_sMotionFrag.bDead == true))
	{
		pos.x = newPosition.x;
		pos.z = newPosition.z;
		pos.y = sakiPos.y;
		Collision(pos, move);

		// �O��͏���Ă����Ƃɂ���
		m_bLandOld = true;
	}
	else
	{
		MyLib::Vector3 posOld = GetOldPosition();
		pos.x = posOld.x;
		pos.z = posOld.z;
		pos = posOld;
		move.x = 0.0f;
		pos.y -= mylib_const::GRAVITY * 7.0f;

		if (m_bJump == false)
		{
			m_bJump = true;
		}

		Collision(pos, move);
	}

	// �����␳
	if (m_state != STATE_KNOCKBACK && m_state != STATE_DMG && m_state != STATE_DEAD && m_state != STATE_FADEOUT)
	{
		move.x += (0.0f - move.x) * 0.25f;
		move.z += (0.0f - move.z) * 0.25f;
	}

	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		m_sMotionFrag.bATK = true;		// �U������OFF

		MyLib::Vector3 weponpos = pos;

		WeaponHandle = CMyEffekseer::GetInstance()->SetEffect(
			"data/Effekseer/debugline_green.efkefc",
			weponpos, rot, 0.0f, 40.0f);

		// �f�o�b�O�\��
		CManager::GetInstance()->GetDebugProc()->Print(
			"�킠��������������������������������������������������������������������������������������\n");
	}

	if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
	{
		MyLib::Vector3 weponpos = pos;
		weponpos.y += 150.0f;

		WeaponHandle = CMyEffekseer::GetInstance()->SetEffect(
			"data/Effekseer/MyLine.efkefc",
			weponpos, rot, 0.0f, 40.0f);
	}

	if (pInputKeyboard->GetRepeat(DIK_O, 4) == true)
	{
		MyLib::Vector3 weponpos = pos;
		weponpos.y += 150.0f;

		MyLib::Vector3 spawnpos = UtilFunc::Transformation::GetRandomPositionSphere(weponpos, 300.0f);

		CMyEffekseer::GetInstance()->SetEffect(
			"data/Effekseer/Laser01.efkefc",
			spawnpos, UtilFunc::Transformation::GetRandomVecSphere() * D3DX_PI, mylib_const::DEFAULT_VECTOR3, 10.0f);
	}

	static float fff = 1.0f;
	if (pInputKeyboard->GetTrigger(DIK_UP) == true)
	{
		fff += 0.1f;
		CManager::GetInstance()->GetSound()->SetFrequency(CSound::LABEL_BGM_GAME, fff);
	}
	if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
	{
		fff -= 0.1f;
		CManager::GetInstance()->GetSound()->SetFrequency(CSound::LABEL_BGM_GAME, fff);
	}

	if (CMyEffekseer::GetInstance()->IsDeath(WeaponHandle))
	{
		WeaponHandle = CMyEffekseer::GetInstance()->SetEffect(
			"data/Effekseer/debugline_green.efkefc",
			pos, rot, 0.0f, 40.0f);
	}

	//if (WeaponHandle != 0)
	{

		// ����̈ʒu
		MyLib::Vector3 weponpos = UtilFunc::Transformation::WorldMtxChangeToPosition(GetModel()[16]->GetWorldMtx());

		// ����̃}�g���b�N�X�擾
		D3DXMATRIX weaponWorldMatrix = GetModel()[16]->GetWorldMtx();

		// ����̕����x�N�g��
		MyLib::Vector3 weaponDirection(weaponWorldMatrix._31, weaponWorldMatrix._32, weaponWorldMatrix._33);

		// �O�Ղ̃}�g���b�N�X�擾
		Effekseer::Matrix43 efcmtx;
		efcmtx = CMyEffekseer::GetInstance()->GetMatrix(WeaponHandle);

		// 4x3�s��Ɍ�����ݒ�
		efcmtx.Value[2][0] = weaponDirection.x;
		efcmtx.Value[2][1] = weaponDirection.y;
		efcmtx.Value[2][2] = weaponDirection.z;

		//// �I�C���[�p�̌v�Z
		//float pitch = atan2f(-weaponDirection.y, sqrtf(weaponDirection.x * weaponDirection.x + weaponDirection.z * weaponDirection.z));
		//float yaw = atan2f(weaponDirection.x, weaponDirection.z);

		//// �I�C���[�p����}�g���b�N�X����
		//D3DXMATRIX rotationMatrix;
		//D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, 0.0f);

		//// 4x3�s��Ɍ�����ݒ�
		//efcmtx.Value[2][0] = weaponDirection.x;
		//efcmtx.Value[2][1] = weaponDirection.y;
		//efcmtx.Value[2][2] = weaponDirection.z;

		CMyEffekseer::GetInstance()->SetMatrix(WeaponHandle, efcmtx);




		// �O�Ղ̃}�g���b�N�X�ݒ�
		//CMyEffekseer::GetInstance()->SetMatrix(WeaponHandle, efcmtx);
		CMyEffekseer::GetInstance()->SetPosition(WeaponHandle, weponpos);

		// �f�o�b�O�\��
		CManager::GetInstance()->GetDebugProc()->Print(
			"����I�I�I�I�I[%f][%f][%f]", weaponDirection.x, weaponDirection.y, weaponDirection.z);



		////CMyEffekseer::GetInstance()->SetMatrix(WeaponHandle, GetModel()[ATKInfo.nCollisionNum]->GetWorldMtx());

		//CMyEffekseer::GetInstance()->SetRotation(WeaponHandle, UtilFunc::Transformation::WorldMtxChangeToRotation(GetModel()[ATKInfo.nCollisionNum]->GetWorldMtx()).Invert());







		//MyLib::Vector3 weaponTip = {0.0f, 50.0f, 0.0f};
		//D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxRotOrigin;	// �v�Z�p�}�g���b�N�X�錾
		//D3DXMATRIX mtxnParent;			// �e�̃}�g���b�N�X
		//D3DXMATRIX m_mtxWorld;			// �e�̃}�g���b�N�X

		//bool bScale = false;

		//// �e�}�g���b�N�X�̏�����
		//D3DXMatrixIdentity(&mtxRotOrigin);
		//D3DXMatrixIdentity(&mtxnParent);

		//// ���[���h�}�g���b�N�X�̏�����
		//D3DXMatrixIdentity(&m_mtxWorld);

		//// �ʒu�𔽉f����
		//D3DXMatrixTranslation(&mtxTrans, weaponTip.x, weaponTip.y, weaponTip.z);
		//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		//// �e�̃}�g���b�N�X��n��
		//mtxnParent = GetModel()[ATKInfo.nCollisionNum]->GetParent()->GetWorldMtx();

		//// �����ɐe�̃��[���h�}�g���b�N�X���|����
		//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxnParent);

		//weaponTip = UtilFunc::Transformation::WorldMtxChangeToPosition(m_mtxWorld);

		//weaponDirection = weaponTip - UtilFunc::Transformation::WorldMtxChangeToPosition(GetModel()[ATKInfo.nCollisionNum]->GetWorldMtx());
		//D3DXVec3Normalize(&weaponDirection, &weaponDirection);

		//// �����x�N�g���̐������擾
		//float rotX = atan2(weaponDirection.y, sqrt(weaponDirection.x * weaponDirection.x + weaponDirection.z * weaponDirection.z));
		//float rotY = atan2(weaponDirection.x, weaponDirection.z);
		//float rotZ = 0.0f;  // ���̐����͕K�v�ɉ����Đݒ�

		// �������G�t�F�N�g�ɐݒ�
		//CMyEffekseer::GetInstance()->SetRotation(WeaponHandle, MyLib::Vector3(weaponDirection .x, weaponDirection .y, weaponDirection .z) * D3DX_PI + MyLib::Vector3(D3DX_PI, 0.0f, D3DX_PI));
		//CMyEffekseer::GetInstance()->SetTransform(WeaponHandle, weponpos, { rotX, rotY, rotZ });



		// �e�����ɑ΂��� atan2 ��K�p���āA�I�C���[�p���擾
		/*float pitch = atan2(weaponDirection.y, sqrt(weaponDirection.x * weaponDirection.x + weaponDirection.z * weaponDirection.z));
		float yaw = atan2(weaponDirection.x, weaponDirection.z);*/


		//// ���W�A���ɕϊ����� Vector3 �ɐݒ�
		//MyLib::Vector3 eulerRotation(pitch, yaw, 0.0f);
		//m_pObjX->SetRotation(eulerRotation);




		//// �I�C���[�p�̌v�Z
		float pitch = atan2f(-weaponDirection.y, sqrtf(weaponDirection.x * weaponDirection.x + weaponDirection.z * weaponDirection.z));
		float yaw = atan2f(-weaponDirection.x, -weaponDirection.z);





		// �e�����ɑ΂��� atan2 ��K�p���āA�I�C���[�p���擾
		/*float pitch = atan2(-weaponDirection.y, sqrt(weaponDirection.x * weaponDirection.x + weaponDirection.z * weaponDirection.z));
		float yaw = atan2(weaponDirection.x, weaponDirection.z);*/


		// �I�C���[�p���Z�b�g
		m_pObjX->SetRotation({ pitch, yaw, 0.0f });


		// �f�o�b�O�\��
		CManager::GetInstance()->GetDebugProc()->Print(
			"\n�����I�I�I�I�I[%f][%f][%f]", pitch, yaw, 0.0f);

		//m_pObjX->SetRotation(weaponDirection * D3DX_PI);
		m_pObjX->SetPosition(weponpos);
	}
}

//==========================================================================
// ���[�V�����̐ݒ�
//==========================================================================
void CPlayer::MotionSet(void)
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == NULL)
	{
		return;
	}

	if (pMotion->IsFinish() == true)
	{// �I�����Ă�����

		// ���݂̎�ގ擾
		int nType = pMotion->GetType();
		int nOldType = pMotion->GetOldType();

		if (m_sMotionFrag.bMove == true && m_sMotionFrag.bKnockBack == false && m_sMotionFrag.bDead == false && m_bJump == false &&
			m_sMotionFrag.bATK == false && m_sMotionFrag.bATK == false)
		{// �ړ����Ă�����

			m_sMotionFrag.bMove = false;	// �ړ�����OFF

			// �ړ����[�V����
			pMotion->Set(MOTION_WALK);
		}
		else if (m_sMotionFrag.bATK == true)
		{// �U��

			m_sMotionFrag.bATK = false;		// �U������OFF

			pMotion->Set(MOTION_ATK, true);
		}
		else
		{
			// �j���[�g�������[�V����
			pMotion->Set(MOTION_DEF);
		}
	}
}

//==========================================================================
// �U��������
//==========================================================================
void CPlayer::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	int nType = pMotion->GetType();

	switch (nType)
	{
	case MOTION_ATK:	// ��ʂ��E��
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_SNOWGET);
		break;

	case MOTION_WALK:
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_WALK);
		break;

	default:
		break;
	}
}

//==========================================================================
// �U�����蒆����
//==========================================================================
void CPlayer::AttackInDicision(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();

	// ����̈ʒu
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), ATKInfo);

	CEffect3D* pEffect = NULL;

	switch (pMotion->GetType())
	{
	case MOTION_ATK:
#if 0
		if (WeaponHandle != 0)
		{


			// ����̃}�g���b�N�X�擾
			D3DXMATRIX weaponWorldMatrix = GetModel()[ATKInfo.nCollisionNum]->GetWorldMtx();

			// ����̕����x�N�g��
			MyLib::Vector3 weaponDirection(weaponWorldMatrix._31, weaponWorldMatrix._32, weaponWorldMatrix._33);

			// �O�Ղ̃}�g���b�N�X�擾
			Effekseer::Matrix43 efcmtx;
			efcmtx = CMyEffekseer::GetInstance()->GetMatrix(WeaponHandle);

			// 4x3�s��Ɍ�����ݒ�
			efcmtx.Value[2][0] = weaponDirection.x;
			efcmtx.Value[2][1] = weaponDirection.y;
			efcmtx.Value[2][2] = weaponDirection.z;

			// �O�Ղ̃}�g���b�N�X�ݒ�
			//CMyEffekseer::GetInstance()->SetMatrix(WeaponHandle, efcmtx);
			CMyEffekseer::GetInstance()->SetPosition(WeaponHandle, weponpos);

			// �f�o�b�O�\��
			CManager::GetInstance()->GetDebugProc()->Print(
				"����I�I�I�I�I[%f][%f][%f]", weaponDirection.x, weaponDirection.y, weaponDirection.z);



			////CMyEffekseer::GetInstance()->SetMatrix(WeaponHandle, GetModel()[ATKInfo.nCollisionNum]->GetWorldMtx());

			//CMyEffekseer::GetInstance()->SetRotation(WeaponHandle, UtilFunc::Transformation::WorldMtxChangeToRotation(GetModel()[ATKInfo.nCollisionNum]->GetWorldMtx()).Invert());

			//m_pObjX->SetPosition(weponpos);
			//m_pObjX->SetRotation(UtilFunc::Transformation::WorldMtxChangeToRotation(GetModel()[ATKInfo.nCollisionNum]->GetWorldMtx()));



			


			//MyLib::Vector3 weaponTip = {0.0f, 50.0f, 0.0f};
			//D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxRotOrigin;	// �v�Z�p�}�g���b�N�X�錾
			//D3DXMATRIX mtxnParent;			// �e�̃}�g���b�N�X
			//D3DXMATRIX m_mtxWorld;			// �e�̃}�g���b�N�X

			//bool bScale = false;

			//// �e�}�g���b�N�X�̏�����
			//D3DXMatrixIdentity(&mtxRotOrigin);
			//D3DXMatrixIdentity(&mtxnParent);

			//// ���[���h�}�g���b�N�X�̏�����
			//D3DXMatrixIdentity(&m_mtxWorld);

			//// �ʒu�𔽉f����
			//D3DXMatrixTranslation(&mtxTrans, weaponTip.x, weaponTip.y, weaponTip.z);
			//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

			//// �e�̃}�g���b�N�X��n��
			//mtxnParent = GetModel()[ATKInfo.nCollisionNum]->GetParent()->GetWorldMtx();

			//// �����ɐe�̃��[���h�}�g���b�N�X���|����
			//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxnParent);

			//weaponTip = UtilFunc::Transformation::WorldMtxChangeToPosition(m_mtxWorld);

			//weaponDirection = weaponTip - UtilFunc::Transformation::WorldMtxChangeToPosition(GetModel()[ATKInfo.nCollisionNum]->GetWorldMtx());
			//D3DXVec3Normalize(&weaponDirection, &weaponDirection);

			//// �����x�N�g���̐������擾
			//float rotX = atan2(weaponDirection.y, sqrt(weaponDirection.x * weaponDirection.x + weaponDirection.z * weaponDirection.z));
			//float rotY = atan2(weaponDirection.x, weaponDirection.z);
			//float rotZ = 0.0f;  // ���̐����͕K�v�ɉ����Đݒ�

			// �������G�t�F�N�g�ɐݒ�
			CMyEffekseer::GetInstance()->SetRotation(WeaponHandle, (weaponDirection.Invert() + D3DX_PI) * D3DX_PI);
			//CMyEffekseer::GetInstance()->SetTransform(WeaponHandle, weponpos, { rotX, rotY, rotZ });
		}
#endif
		break;
	}

#if _DEBUG
	CEffect3D::Create(weponpos, MyLib::Vector3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), ATKInfo.fRangeSize, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
#endif

	// �G�̃��X�g�擾
	CListManager<CEnemy> enemyList = CEnemy::GetListObj();
	CEnemy* pEnemy = nullptr;

	// ���X�g���[�v
	while (enemyList.ListLoop(&pEnemy))
	{
		// �G�̈ʒu�擾
		MyLib::Vector3 TargetPos = pEnemy->GetPosition();

		// ����T�C�Y�擾
		float fTargetRadius = pEnemy->GetRadius();

		if (UtilFunc::Collision::SphereRange(weponpos, TargetPos, ATKInfo.fRangeSize, fTargetRadius))
		{// ���̔���

			if (pEnemy->Hit(ATKInfo.nDamage) == true)
			{// �������Ă���

			}
		}
	}
}

//==========================================================================
// �ʒu����
//==========================================================================
void CPlayer::LimitPos(void)
{
	MyLib::Vector3 pos = GetPosition();

	float fLength = sqrtf(pos.x * pos.x + pos.z * pos.z);

	if (fLength > RADIUS_STAGE)
	{// �␳
		D3DXVec3Normalize(&pos, &pos);

		pos *= RADIUS_STAGE;

		SetPosition(pos);
	}
}

//==========================================================================
// �����蔻��
//==========================================================================
bool CPlayer::Collision(MyLib::Vector3 &pos, MyLib::Vector3 &move)
{
	// �����擾
	MyLib::Vector3 rot = GetRotation();

	// ���n�������ǂ���
	bool bLand = false;
	float fHeight = 0.0f;
	m_bLandField = false;
	m_bHitWall = false;			// �ǂ̓����蔻��

	// �����擾
	if (m_state != STATE_KNOCKBACK && m_state != STATE_DMG && m_state != STATE_DEAD && m_state != STATE_FADEOUT)
	{
		fHeight = CManager::GetInstance()->GetScene()->GetElevation()->GetHeight(pos, &bLand);
	}
	else
	{
		fHeight = pos.y;
	}

	if (fHeight > pos.y)
	{// �n�ʂ̕���������荂��������

		// �n�ʂ̍����ɕ␳
		pos.y = fHeight;
		m_bLandField = true;

		if (bLand == true)
		{// ���n���Ă���

			// �W�����v�g�p�\�ɂ���
			m_bJump = false;
			move.y = 0.0f;
			m_bLandOld = true;
		}
	}


	// X�t�@�C���Ƃ̔���
	CStage *pStage = CGame::GetStage();
	if (pStage == NULL)
	{// NULL��������
		return false;
	}

	bool bNowLand = false;

	// �X�e�[�W�ɓ�����������
	m_bHitStage = false;
	for (int nCntStage = 0; nCntStage < pStage->GetNumAll(); nCntStage++)
	{
		// �I�u�W�F�N�g�擾
		CObjectX *pObjX = pStage->GetObj(nCntStage);

		if (pObjX == NULL)
		{// NULL��������
			continue;
		}

		// �����擾
		bool bLand = false;
		fHeight = pObjX->GetHeight(pos, bLand);

		if (bLand == true && fHeight > pos.y)
		{// �n�ʂ̕���������荂��������

		 // �n�ʂ̍����ɕ␳
			if (pos.y + 50.0f <= fHeight)
			{// �������ǂ���������
				m_bHitWall = true;
			}
			else
			{
				pos.y = fHeight;
			}

			m_bHitStage = true;
			m_bLandField = false;

			if (bLand == true)
			{// ���n���Ă���

				if ((m_sMotionFrag.bKnockBack || m_bJump == true) && GetPosition().y >= fHeight)
				{
					m_bLandOld = true;
				}

				if (m_bJump == true)
				{// �W�����v����������

					// ���[�V�����擾
					CMotion* pMotion = GetMotion();
					pMotion->ToggleFinish(true);
				}

				// �W�����v�g�p�\�ɂ���
				m_bJump = false;
				move.y = 0.0f;
				bNowLand = true;
				m_sMotionFrag.bJump = false;
			}
		}
	}

	// �G���A�������擾
	CListManager<CLimitArea> limitareaList = CLimitArea::GetListObj();
	CLimitArea* pLimitArea = nullptr;

	while (limitareaList.ListLoop(&pLimitArea))
	{
		CLimitArea::sLimitEreaInfo info = pLimitArea->GetLimitEreaInfo();

		// ��l�̕ǂ�K�p
		if (pos.x + GetRadius() >= info.fMaxX) { pos.x = info.fMaxX - GetRadius(); }
		if (pos.x - GetRadius() <= info.fMinX) { pos.x = info.fMinX + GetRadius(); }
		if (pos.z + GetRadius() >= info.fMaxZ) { pos.z = info.fMaxZ - GetRadius(); }
		if (pos.z - GetRadius() <= info.fMinZ) { pos.z = info.fMinZ + GetRadius(); }
	}

	// �����ݒ�
	SetRotation(rot);

	return bNowLand;
}

//==========================================================================
// �q�b�g����
//==========================================================================
bool CPlayer::Hit(const int nValue)
{
	// �̗͎擾
	int nLife = GetLife();

	if (m_state != STATE_DMG &&
		m_state != STATE_KNOCKBACK &&
		m_state != STATE_INVINCIBLE &&
		m_state != STATE_DEAD &&
		m_state != STATE_FADEOUT)
	{// �_���[�W��t��Ԃ̎�

		// �̗͌��炷
		nLife -= nValue;


		// �Q�[���p�b�h���擾
		CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();
		pInputGamepad->SetVibration(CInputGamepad::VIBRATION_STATE_DMG, 0);

		m_KnokBackMove.y += 18.0f;
		m_bHitStage = false;

		// �̗͐ݒ�
		SetLife(nLife);

		if (nLife <= 0)
		{// �̗͂��Ȃ��Ȃ�����

			// �����
			m_state = STATE_DEAD;

			m_KnokBackMove.y = 8.0f;

			// �J�ڃJ�E���^�[�ݒ�
			m_nCntState = DEADTIME;

			// �̗͐ݒ�
			SetLife(0);

			// �m�b�N�o�b�N����ɂ���
			m_sMotionFrag.bKnockBack = true;

			// ���ꃂ�[�V����
			//pMotion->Set(MOTION_KNOCKBACK);

			// �m�b�N�o�b�N�̈ʒu�X�V
			MyLib::Vector3 pos = GetPosition();
			MyLib::Vector3 rot = GetRotation();
			m_posKnokBack = pos;

			// �Ռ��g����
			CImpactWave::Create
			(
				MyLib::Vector3(pos.x, pos.y + 80.0f, pos.z),	// �ʒu
				MyLib::Vector3(D3DX_PI * 0.5f, D3DX_PI + rot.y, D3DX_PI),				// ����
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f),			// �F
				80.0f,										// ��
				80.0f,										// ����
				0.0f,										// ���S����̊Ԋu
				20,											// ����
				10.0f,										// ���̈ړ���
				CImpactWave::TYPE_GIZAWHITE,				// �e�N�X�`���^�C�v
				false										// ���Z�������邩
			);

			CManager::GetInstance()->SetEnableHitStop(18);

			// �U��
			CManager::GetInstance()->GetCamera()->SetShake(21, 30.0f, 0.0f);

			// ����
			return true;
		}

		// �ߋ��̏�ԕۑ�
		m_Oldstate = m_state;

		// �F�ݒ�
		m_mMatcol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		// �_���[�W��Ԃɂ���
		m_state = STATE_DMG;

		// �J�ڃJ�E���^�[�ݒ�
		m_nCntState = 0;

		// �m�b�N�o�b�N�̈ʒu�X�V
		MyLib::Vector3 pos = GetPosition();
		MyLib::Vector3 rot = GetRotation();
		m_posKnokBack = pos;

		// �m�b�N�o�b�N����ɂ���
		m_sMotionFrag.bKnockBack = true;

		// ���ꃂ�[�V����
		///pMotion->Set(MOTION_KNOCKBACK);

		// �Ռ��g����
		CImpactWave::Create
		(
			MyLib::Vector3(pos.x, pos.y + 80.0f, pos.z),	// �ʒu
			MyLib::Vector3(D3DX_PI * 0.5f, D3DX_PI + rot.y, D3DX_PI),				// ����
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f),			// �F
			80.0f,										// ��
			80.0f,										// ����
			0.0f,										// ���S����̊Ԋu
			20,											// ����
			10.0f,										// ���̈ړ���
			CImpactWave::TYPE_GIZAWHITE,				// �e�N�X�`���^�C�v
			false										// ���Z�������邩
		);

		CManager::GetInstance()->SetEnableHitStop(12);

		// �U��
		CManager::GetInstance()->GetCamera()->SetShake(12, 25.0f, 0.0f);

		// �T�E���h�Đ�
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_PLAYERDMG);
	}

	// ����łȂ�
	return false;
}

//==========================================================================
// ��ԍX�V����
//==========================================================================
void CPlayer::UpdateState(void)
{
	switch (m_state)
	{
	case STATE_NONE:
		// �F�ݒ�
		m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		break;

	case STATE_INVINCIBLE:
		Invincible();
		break;

	case STATE_DMG:
		Damage();
		break;

	case STATE_DEAD:
		Dead();
		break;

	case STATE_FADEOUT:
		FadeOut();
		break;

	case STATE_KNOCKBACK:
		KnockBack();
		break;
	}
}

//==========================================================================
// ���G
//==========================================================================
void CPlayer::Invincible(void)
{
	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState--;

	if (m_nCntState % INVINCIBLE_INT == 0)
	{// �K��Ԋu
		
		// �F�ݒ�
		if (m_mMatcol.a == 1.0f)
		{
			m_mMatcol.a = 0.4f;
		}
		else
		{
			m_mMatcol.a = 1.0f;
		}
	}

	if (m_nCntState <= 0)
	{// �J�ڃJ�E���^�[��0�ɂȂ�����

		// �Ȃɂ��Ȃ���Ԃɂ���
		m_state = STATE_NONE;
		m_nCntState = 0;

		// �F�ݒ�
		m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

}

//==========================================================================
// �_���[�W
//==========================================================================
void CPlayer::Damage(void)
{
	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	// �ړ��ʎ擾
	MyLib::Vector3 move = GetMove();

	// �����擾
	MyLib::Vector3 rot = GetRotation();

	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState++;

	// �F�ݒ�
	m_mMatcol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	// �ʒu�X�V
	pos.y = (-0.4f * (float)(m_nCntState * m_nCntState) + m_KnokBackMove.y * (float)m_nCntState) + m_posKnokBack.y;
	pos.x += move.x;
	pos.z += move.z;

	// �N���Ƃ̔���
	if ((CManager::GetInstance()->GetScene()->GetElevation()->IsHit(pos) || m_bHitStage) && m_nCntState >= 10)
	{// �n�ʂƓ������Ă�����
		m_state = STATE_INVINCIBLE;
		m_nCntState = INVINCIBLE_TIME;
		m_KnokBackMove.y = 0.0f;	// �ړ��ʃ[��
		m_bLandOld = true;

		// �F�ݒ�
		m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �m�b�N�o�b�N�������
		m_sMotionFrag.bKnockBack = false;

		// ���[�V�����擾
		CMotion* pMotion = GetMotion();
		if (pMotion == NULL)
		{// ���[�V������NULL��������
			return;
		}
		pMotion->ToggleFinish(true);


		// X�t�@�C���Ƃ̔���
		CStage *pStage = CGame::GetStage();
		if (pStage == NULL)
		{// NULL��������
			return;
		}

		// �X�e�[�W�ɓ�����������
		for (int nCntStage = 0; nCntStage < pStage->GetNumAll(); nCntStage++)
		{
			// �I�u�W�F�N�g�擾
			CObjectX *pObjX = pStage->GetObj(nCntStage);

			if (pObjX == NULL)
			{// NULL��������
				continue;
			}

			// �����擾
			bool bLand = false;
			pos.y = pObjX->GetHeight(pos, bLand);
		}


	}
	else if (m_nCntState >= 30)
	{// �J�ڃJ�E���^�[��30�ɂȂ�����

		// �m�b�N�o�b�N��Ԃɂ���
		m_state = STATE_KNOCKBACK;
	}


	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);

	// �����ݒ�
	SetRotation(rot);
}

//==========================================================================
// ���S
//==========================================================================
void CPlayer::Dead(void)
{
	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	// �ړ��ʎ擾
	MyLib::Vector3 move = GetMove();

	// �ʒu�X�V
	if (m_nCntState > 0)
	{
		int nCnt = DEADTIME - m_nCntState;
		pos.y = (-0.1f * (float)(nCnt * nCnt) + m_KnokBackMove.y * (float)nCnt) + m_posKnokBack.y;
		pos.x += move.x;
	}

	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState--;

	// �N���Ƃ̔���
	if ((CManager::GetInstance()->GetScene()->GetElevation()->IsHit(pos) || m_bHitStage) && m_nCntState >= 10)
	{// �n�ʂƓ������Ă�����

		m_state = STATE_FADEOUT;	// �t�F�[�h�A�E�g
		m_nCntState = FADEOUTTIME;
		m_KnokBackMove.y = 0.0f;	// �ړ��ʃ[��
		m_bLandOld = true;
		move.x = 0.0f;

		// �F�ݒ�
		m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �m�b�N�o�b�N�������
		m_sMotionFrag.bKnockBack = false;
		m_sMotionFrag.bDead = true;
		//pMotion->ToggleFinish(true);

		// �Ԃ��|�ꃂ�[�V����
		//pMotion->Set(MOTION_DEAD);

		// X�t�@�C���Ƃ̔���
		CStage *pStage = CGame::GetStage();
		if (pStage == NULL)
		{// NULL��������
			return;
		}

		// �X�e�[�W�ɓ�����������
		for (int nCntStage = 0; nCntStage < pStage->GetNumAll(); nCntStage++)
		{
			// �I�u�W�F�N�g�擾
			CObjectX *pObjX = pStage->GetObj(nCntStage);

			if (pObjX == NULL)
			{// NULL��������
				continue;
			}

			// �����擾
			bool bLand = false;
			pos.y = pObjX->GetHeight(pos, bLand);
		}
	}

	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CPlayer::FadeOut(void)
{

	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState--;

	// �F�ݒ�
	m_mMatcol.a = (float)m_nCntState / (float)FADEOUTTIME;

	if (m_nCntState <= 0)
	{// �J�ڃJ�E���^�[��0�ɂȂ�����

		// ���S����
		Kill();

		// �I������
		Uninit();
		return;
	}
}

//==========================================================================
// �m�b�N�o�b�N
//==========================================================================
void CPlayer::KnockBack(void)
{
	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	// �ړ��ʎ擾
	MyLib::Vector3 move = GetMove();

	// �����擾
	MyLib::Vector3 rot = GetRotation();

	// �ڕW�̌����擾
	float fRotDest = GetRotDest();

	// �ړ��ʎ擾
	float fMove = GetVelocity();

	// ���݂ƖڕW�̍���
	float fRotDiff = 0.0f;

	// �����̔���
	bool bLen = false;


	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState++;

	// �ʒu�X�V
	pos.y = (-0.4f * (float)(m_nCntState * m_nCntState) + m_KnokBackMove.y * (float)m_nCntState) + m_posKnokBack.y;
	pos.x += move.x;
	pos.z += move.z;

	// �N���Ƃ̔���
	if ((CManager::GetInstance()->GetScene()->GetElevation()->IsHit(pos) || m_bHitStage))
	{// �n�ʂƓ������Ă�����
		m_state = STATE_INVINCIBLE;
		m_nCntState = INVINCIBLE_TIME;
		m_KnokBackMove.y = 0.0f;	// �ړ��ʃ[��
		m_bLandOld = true;

		// �F�ݒ�
		m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		
		// �m�b�N�o�b�N�������
		m_sMotionFrag.bKnockBack = false;

		// ���[�V�����擾
		CMotion* pMotion = GetMotion();
		if (pMotion == NULL)
		{// ���[�V������NULL��������
			return;
		}
		pMotion->ToggleFinish(true);

		// X�t�@�C���Ƃ̔���
		CStage *pStage = CGame::GetStage();
		if (pStage == NULL)
		{// NULL��������
			return;
		}

		// �X�e�[�W�ɓ�����������
		for (int nCntStage = 0; nCntStage < pStage->GetNumAll(); nCntStage++)
		{
			// �I�u�W�F�N�g�擾
			CObjectX *pObjX = pStage->GetObj(nCntStage);

			if (pObjX == NULL)
			{// NULL��������
				continue;
			}

			// �����擾
			bool bLand = false;
			pos.y = pObjX->GetHeight(pos, bLand);
		}
	}


	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);

	// �����ݒ�
	SetRotation(rot);

	// �ڕW�̌����ݒ�
	SetRotDest(fRotDest);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CPlayer::Draw(void)
{

	// �`�揈��
	if (m_state == STATE_DMG)
	{
		CObjectChara::Draw(m_mMatcol);
	}
	else if (m_state == STATE_INVINCIBLE || m_state == STATE_FADEOUT)
	{
		CObjectChara::Draw(m_mMatcol.a);
	}
	else
	{
		CObjectChara::Draw();
	}
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CPlayer::SetState(STATE state, int nCntState)
{
	m_state = state;
	m_nCntState = nCntState;
}

//==========================================================================
// ��Ԏ擾
//==========================================================================
CPlayer::STATE CPlayer::GetState(void)
{
	return m_state;
}

