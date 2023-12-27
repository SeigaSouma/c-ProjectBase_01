//=============================================================================
// 
//  �v���C���[�w�b�_�[ [player.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_	// ��d�C���N���[�h�h�~

#include "gamemanager.h"
#include "objectChara.h"

//==========================================================================
// �O���錾
//==========================================================================
class CShadow;
class CTargetPoint;
class CHP_GaugePlayer;
class CMeshSphere;

//==========================================================================
// �N���X��`
//==========================================================================
// �v���C���[�N���X��`
class CPlayer : public CObjectChara
{
protected:
	// �񋓌^��`
	enum MOTION
	{
		MOTION_DEF = 0,		// �j���[�g�������[�V����
		MOTION_WALK,		// �ړ����[�V����
		MOTION_ATK,			// �U��
		MOTION_JUMP,		// �W�����v
		MOTION_FALL,		// ������
		MOTION_KNOCKBACK,	// �m�b�N�o�b�N
		MOTION_DEAD,		// ���S
		MOTION_MAX
	};

	// ���[�V�����̔���
	struct SMotionFrag
	{
		bool bJump;			// �W�����v�����ǂ���
		bool bATK;			// �U�������ǂ���
		bool bKnockBack;	// �m�b�N�o�b�N�����ǂ���
		bool bDead;			// ���S�����ǂ���
		bool bMove;			// �ړ������ǂ���
	};

public:

	// ��Ԓ�`
	enum STATE
	{
		STATE_NONE = 0,		// �Ȃɂ��Ȃ�
		STATE_INVINCIBLE,	// ���G
		STATE_DMG,			// �_���[�W
		STATE_KNOCKBACK,	// �m�b�N�o�b�N
		STATE_DEAD,			// ��
		STATE_FADEOUT,		// �t�F�[�h�A�E�g
		STATE_ATTACK,		// �U������
		STATE_MAX
	};

	CPlayer(int nPriority = mylib_const::PRIORITY_DEF2D);
	~CPlayer();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	bool Hit(const int nValue);	// �q�b�g����
	STATE GetState(void);		// ��Ԏ擾

	void SetState(STATE state, int nCntState = 0);	// ��Ԑݒ�
	static CPlayer *Create(int nIdx);	// ����
	void UninitByMode(void);			// ���[�h�ʏI��
	virtual void Kill(void);			// ���S����

protected:

	bool Collision(D3DXVECTOR3 &pos, D3DXVECTOR3 &move);	// �����蔻��

	void UpdateState(void);	// ��ԍX�V����
	void MotionSet(void);	// ���[�V�����̐ݒ�

	bool m_bJump;				// �W�����v�����ǂ���
	bool m_bLandOld;			// �ߋ��̒��n���
	bool m_bHitStage;			// �X�e�[�W�̓����蔻��
	bool m_bLandField;			// �t�B�[���h�̒��n����
	bool m_bHitWall;			// �ǂ̓����蔻��
	int m_nMyPlayerIdx;			// �v���C���[�C���f�b�N�X�ԍ�
	int m_nCntWalk;				// ���s�J�E���^�[
	STATE m_state;				// ���
	CShadow *m_pShadow;			// �e�̏��
	SMotionFrag m_sMotionFrag;	// ���[�V�����̃t���O
private:

	// �����o�֐�
	void KnockBack(void);	// �m�b�N�o�b�N
	void Damage(void);		// �_���[�W
	void Dead(void);		// ���S
	void FadeOut(void);		// �t�F�[�h�A�E�g
	void Invincible(void);	// ���G
	virtual void Controll(void);	// ����
	void LimitPos(void);	// �ʒu����

	void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK) override;	// �U��������
	void AttackInDicision(CMotion::AttackInfo ATKInfo, int nCntATK) override;			// �U�����蒆����

	STATE m_Oldstate;			// �O��̏��
	D3DXCOLOR m_mMatcol;		// �}�e���A���̐F
	D3DXVECTOR3 m_posKnokBack;	// �m�b�N�o�b�N�̈ʒu
	D3DXVECTOR3 m_KnokBackMove;	// �m�b�N�o�b�N�̈ړ���
	int m_nCntState;			// ��ԑJ�ڃJ�E���^�[
	CTargetPoint *m_pTargetP;	// �ڕW�̒n�_
};



#endif