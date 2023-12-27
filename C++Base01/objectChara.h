//=============================================================================
// 
//  �I�u�W�F�N�g�L�����N�^�[�w�b�_�[ [objectChara.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECTCHARA_H_
#define _OBJECTCHARA_H_	// ��d�C���N���[�h�h�~

#include "objectHierarchy.h"
#include "motion.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �I�u�W�F�N�g�L�����N�^�[�N���X��`
class CObjectChara : public CObjectHierarchy
{
public:

	CObjectChara(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CObjectChara();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Draw(D3DXCOLOR col) override;
	void Draw(float fAlpha) override;

	void SetVelocity(const float fVelocity);	// �ړ����x�ݒ�
	float GetVelocity(void) const;				// �ړ����x�擾
	void SetRotDest(const float fRotDest);		// �ڕW�̌����ݒ�
	float GetRotDest(void) const;				// �ڕW�̌����擾
	void SetLife(const int nLife);				// �̗͐ݒ�
	int GetLife(void) const;					// �̗͎擾
	int GetLifeOrigin(void) const;				// ���̗͎̑擾
	int GetMotionStartIdx(void) const;			// ���[�V�����J�n�̃C���f�b�N�X�ԍ��擾
	int GetAddScoreValue(void) const;			// �X�R�A���Z�ʎ擾

	HRESULT SetCharacter(const std::string pTextFile) override;	// �L�����N�^�[�ݒ�
	CObjectChara *GetObjectChara(void);
	static CObjectChara *Create(const std::string pTextFile);

protected:

	void LoadObjectData(FILE* pFile, const char* pComment) override;	// �I�u�W�F�N�g���̃f�[�^�ǂݍ���
	void LoadPartsData(FILE* pFile, const char* pComment, int *pCntParts) override;		// �p�[�c���̃f�[�^�ǂݍ���
	void BindObjectData(int nCntData) override;							// �I�u�W�F�N�g���̃f�[�^���蓖��
	virtual void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK);	// �U��������
	virtual void AttackInDicision(CMotion::AttackInfo ATKInfo, int nCntATK);			// �U�����蒆����
	void ChangeMotion(const char* pMotionFile);	// ���[�V�����t�@�C���ύX
	CMotion* GetMotion(void);	// ���[�V�����I�u�W�F�N�g�擾
private:

	// �����o�֐�
	void MotionInProgressAction(void);	// ���[�V�������̍s������

	// �����o�ϐ�
	float m_fVelocity;			// �ړ����x
	float m_fRotDest;			// �ڕW�̌���
	int m_nLife;				// �̗�
	int m_nLifeOrigin;			// ���̗̑�
	int m_nMotionStartIdx;		// ���[�V�����J�n�̃C���f�b�N�X�ԍ�
	int m_nAddScore;			// �X�R�A���Z��
	CMotion *m_pMotion;			// ���[�V�����̏��
};



#endif