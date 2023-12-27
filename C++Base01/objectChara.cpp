//=============================================================================
// 
//  �I�u�W�F�N�g�L�����N�^�[���� [objectChara.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "objectChara.h"
#include "calculation.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "3D_effect.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObjectChara::CObjectChara(int nPriority) : CObjectHierarchy(nPriority)
{
	// �l�̃N���A
	m_fVelocity = 0.0f;		// �ړ����x
	m_fRotDest = 0.0f;		// �ڕW�̌���
	m_nLife = 0;			// �̗�
	m_nLifeOrigin = 0;		// ���̗̑�
	m_nMotionStartIdx = 0;	// ���[�V�����J�n�̃C���f�b�N�X�ԍ�
	m_nAddScore = 0;		// �X�R�A���Z��
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObjectChara::~CObjectChara()
{

}

//==========================================================================
// ��������
//==========================================================================
CObjectChara *CObjectChara::Create(const std::string pTextFile)
{
	// �����p�̃I�u�W�F�N�g
	CObjectChara *pObjChara = nullptr;

	if (pObjChara == nullptr)
	{

		// �������̊m��
		pObjChara = DEBUG_NEW CObjectChara;

		if (pObjChara != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			HRESULT hr = pObjChara->SetCharacter(pTextFile);
			if (FAILED(hr))
			{// ���s���Ă�����
				return nullptr;
			}
		}

		return pObjChara;
	}

	return nullptr;
}

//==========================================================================
// �L�����쐬
//==========================================================================
HRESULT CObjectChara::SetCharacter(const std::string pTextFile)
{
	// �L�����쐬
	CObjectHierarchy::SetCharacter(pTextFile);

	// ���[�V�����̐�������
	m_pMotion = CMotion::Create(pTextFile);

	// ���[�V�����̐ݒ�
	if (m_pMotion != nullptr)
	{
		m_pMotion->SetModel(GetModel(), GetNumModel(), this);

		// �|�[�Y�̃��Z�b�g
		m_pMotion->ResetPose(0);
	}

	return S_OK;
}

//==========================================================================
// �I�u�W�F�N�g���̃f�[�^���蓖��
//==========================================================================
void CObjectChara::BindObjectData(int nCntData)
{

	// �I�u�W�F�N�g���̃f�[�^���蓖��
	CObjectHierarchy::BindObjectData(nCntData);

	// �ړ����x
	m_fVelocity = m_aLoadData[nCntData].fVelocity;

	// �̗�
	m_nLife = m_aLoadData[nCntData].nLife;
	m_nLifeOrigin = m_nLife;

	// �X�R�A���Z��
	m_nAddScore = m_aLoadData[nCntData].nAddScore;

	// ���[�V�����X�^�[�g�̃C���f�b�N�X
	m_nMotionStartIdx = m_aLoadData[nCntData].nMotionStartIdx;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObjectChara::Init(void)
{
	// ����������
	CObjectHierarchy::Init();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CObjectChara::Uninit(void)
{
	if (m_pMotion != NULL)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = NULL;
	}

	// �I������
	CObjectHierarchy::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CObjectChara::Update(void)
{
	// ���[�V�����X�V
	if (m_pMotion != nullptr)
	{
		m_pMotion->Update();
	}

	// ���[�V�������̍s������
	MotionInProgressAction();
}

//==========================================================================
// ���[�V�������̍s������
//==========================================================================
void CObjectChara::MotionInProgressAction(void)
{
	if (m_pMotion == nullptr)
	{
		return;
	}

	// ���擾
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();
	CMotion::Info aInfo = m_pMotion->GetInfo(m_pMotion->GetType());
	int nType = m_pMotion->GetType();

	// �U�����̑����擾
	int nNumAttackInfo = aInfo.nNumAttackInfo;

	for (int nCntAttack = 0; nCntAttack < nNumAttackInfo; nCntAttack++)
	{
		if (aInfo.AttackInfo[nCntAttack] == nullptr)
		{
			continue;
		}

		// �U�����擾
		CMotion::AttackInfo AttackInfo = *aInfo.AttackInfo[nCntAttack];

		if (m_pMotion->IsImpactFrame(AttackInfo))
		{// �Ռ��̃J�E���g�Ɠ����Ƃ�

			// �U��������
			AttackAction(AttackInfo, nCntAttack);
		}

		// ���[�V�����J�E���^�[�擾
		float fAllCount = m_pMotion->GetAllCount();
		if (fAllCount > AttackInfo.nMinCnt && fAllCount <= AttackInfo.nMaxCnt)
		{// �U�����蒆

			// �U�����蒆����
			AttackInDicision(AttackInfo, nCntAttack);
		}
	}
}

//==========================================================================
// �U��������
//==========================================================================
void CObjectChara::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	// ����̈ʒu
	D3DXVECTOR3 weponpos = m_pMotion->GetAttackPosition(GetModel(), ATKInfo);
}

//==========================================================================
// �U�����蒆����
//==========================================================================
void CObjectChara::AttackInDicision(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	// ����̈ʒu
	D3DXVECTOR3 weponpos = m_pMotion->GetAttackPosition(GetModel(), ATKInfo);
}

//==========================================================================
// ���[�V�����t�@�C���ύX
//==========================================================================
void CObjectChara::ChangeMotion(const char* pMotionFile)
{
	if (m_pMotion != NULL)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = NULL;
	}

	// ���[�V�����̐�������
	m_pMotion = CMotion::Create(pMotionFile);

	// ���[�V�����̐ݒ�
	m_pMotion->SetModel(GetModel(), GetNumModel(), this);

	// �|�[�Y�̃��Z�b�g
	m_pMotion->ResetPose(0);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObjectChara::Draw(void)
{
	// �`�揈��
	CObjectHierarchy::Draw();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObjectChara::Draw(D3DXCOLOR col)
{
	// �`�揈��
	CObjectHierarchy::Draw(col);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObjectChara::Draw(float fAlpha)
{
	// �`�揈��
	CObjectHierarchy::Draw(fAlpha);
}

//==========================================================================
// �I�u�W�F�N�g���̃f�[�^�ǂݍ���
//==========================================================================
void CObjectChara::LoadObjectData(FILE* pFile, const char* pComment)
{
	// �I�u�W�F�N�g���̃f�[�^�ǂݍ���
	CObjectHierarchy::LoadObjectData(pFile, pComment);

	char hoge[MAX_COMMENT];	// �R�����g

	if (strcmp(pComment, "VELOCITY") == 0)
	{// VELOCITY�ňړ����x

		fscanf(pFile, "%s", &hoge[0]);	// =�̕�
		fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].fVelocity);	// �ړ����x
		m_fVelocity = m_aLoadData[m_nNumLoad].fVelocity;
	}

	if (strcmp(pComment, "LIFE") == 0)
	{// LIFE�ő̗�

		fscanf(pFile, "%s", &hoge[0]);	// =�̕�
		fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].nLife);	// �̗�
		m_nLife = m_aLoadData[m_nNumLoad].nLife;
		m_nLifeOrigin = m_nLife;	// ���̗̑�
	}

	if (strcmp(pComment, "MOTION_STARTPARTS") == 0)
	{// MOTION_STARTPARTS�Ń��[�V�����J�n�̃C���f�b�N�X�ԍ�

		fscanf(pFile, "%s", &hoge[0]);	// =�̕�
		fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].nMotionStartIdx);	// ���[�V�����J�n�̃C���f�b�N�X�ԍ�
		m_nMotionStartIdx = m_aLoadData[m_nNumLoad].nMotionStartIdx;
	}

	if (strcmp(pComment, "SCORE") == 0)
	{// LIFE�ő̗�

		fscanf(pFile, "%s", &hoge[0]);	// =�̕�
		fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].nAddScore);	// �̗�
		m_nAddScore = m_aLoadData[m_nNumLoad].nAddScore;
	}

}

//==========================================================================
// �p�[�c���̃f�[�^�ǂݍ���
//==========================================================================
void CObjectChara::LoadPartsData(FILE* pFile, const char* pComment, int *pCntParts)
{
	// �p�[�c���̃f�[�^�ǂݍ���
	CObjectHierarchy::LoadPartsData(pFile, pComment, pCntParts);
}

//==========================================================================
// �ړ����x�ݒ�
//==========================================================================
void CObjectChara::SetVelocity(const float fVelocity)
{
	m_fVelocity = fVelocity;
}

//==========================================================================
// �ړ����x�ݒ�
//==========================================================================
float CObjectChara::GetVelocity(void) const
{
	return m_fVelocity;
}

//==========================================================================
// �ڕW�̌����ݒ�
//==========================================================================
void CObjectChara::SetRotDest(const float fRotDest)
{
	m_fRotDest = fRotDest;
}

//==========================================================================
// �ڕW�̌����ݒ�
//==========================================================================
float CObjectChara::GetRotDest(void) const
{
	return m_fRotDest;
}

//==========================================================================
// �̗͐ݒ�
//==========================================================================
void CObjectChara::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//==========================================================================
// �̗͎擾
//==========================================================================
int CObjectChara::GetLife(void) const
{
	return m_nLife;
}

//==========================================================================
// ���̗͎̑擾
//==========================================================================
int CObjectChara::GetLifeOrigin(void) const
{
	return m_nLifeOrigin;
}

//==========================================================================
// ���[�V�����J�n�̃C���f�b�N�X�ԍ��擾
//==========================================================================
int CObjectChara::GetMotionStartIdx(void) const
{
	return m_nMotionStartIdx;
}

//==========================================================================
// �X�R�A���Z�ʎ擾
//==========================================================================
int CObjectChara::GetAddScoreValue(void) const
{
	return m_nAddScore;
}

//==========================================================================
// ���[�V�����I�u�W�F�N�g�擾
//==========================================================================
CMotion* CObjectChara::GetMotion(void)
{
	return m_pMotion;
}

//==========================================================================
// �I�u�W�F�N�g�L�����N�^�[�I�u�W�F�N�g�̎擾
//==========================================================================
CObjectChara* CObjectChara::GetObjectChara(void)
{
	return this;
}
