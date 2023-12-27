//=============================================================================
// 
//  �e�w�b�_�[ [bullet.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _BULLET_H_
#define _BULLET_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "meshsphere.h"

class CThunderRing;
class CObjectBillboard;

//==========================================================================
// �N���X��`
//==========================================================================
// �e�N���X��`
class CBullet : public CMeshSphere
{
public:

	// �񋓌^��`
	enum TYPE
	{
		TYPE_PLAYER = 0,	// �v���C���[�̒e
		TYPE_ENEMY,		// �G�̒e
		TYPE_MAX
	};

	enum MOVETYPE
	{
		MOVETYPE_NORMAL = 0,	// �ʏ�
		MOVETYPE_PARABOLA,		// ������
		MOVETYPE_MAX
	};

	enum STATE
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ�
		STATE_DMG,		// ���˕Ԃ�
		STATE_MAX
	};

	CBullet(int nPriority = 6);
	~CBullet();


	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	void SetVtx(void);

	void SetType(TYPE type);	// ��ސݒ�
	TYPE GetType(void);			// ��ގ擾
	void SetState(STATE state, int nCntState);	// ��Ԑݒ�
	STATE GetState(void);	// ��Ԏ擾
	void SetTargetPosition(D3DXVECTOR3 pos);	// �ڕW�̈ʒu
	void SetParabolaHeight(float fHeight);		// �������̍ő卂��

	void SetReverseAutoDeath(void) { m_bAutoDeath = m_bAutoDeath ?  false : true; }	// �����폜�̔���폜
	bool IsFinish(void) { return m_bFinish; }					// �I���̔���

	static int GetNumAll(void);
	static CBullet *Create(TYPE type, MOVETYPE movetype, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 move, const float fSize);

private:
	void UpdatePos(void);		// �ړ�
	void UpdateTypePlayer(void);	// �v���C���[�e�̍X�V
	void CollisionPlayer(void);		// �v���C���[�Ƃ̔���
	void CollisionEnemy(void);		// �G�Ƃ̔���
	
	void StateNone(void);		// �����Ȃ����
	void StateDamage(void);		// �_���[�W��ԏ���


	TYPE m_type;				// �e�̎��
	STATE m_state;				// ���
	MOVETYPE m_MoveType;		// �ړ��̎��
	D3DXVECTOR3 m_OriginPosition;	// ���̈ʒu
	D3DXVECTOR3 m_TargetPosition;	// �ڕW�̈ʒu
	int m_nCntState;			// ��ԑJ�ڃJ�E���^�[
	int m_nLife;				// ����
	int m_nLifeMax;				// �����̍ő�l
	int m_nTexIdx;				// �e�N�X�`���̃C���f�b�N�X�ԍ�
	int m_nCntEmission;			// �������̃J�E���^�[
	float m_fMaxParabolaHeight;	// �������̍ő卂��
	bool m_bAutoDeath;		// �����폜�̃t���O
	bool m_bFinish;			// �I���������
	static int m_nNumAll;		// �e�̑���
	static const char *m_apTextureFile[TYPE_MAX];	// �e�N�X�`���̃t�@�C��

	typedef void(CBullet::*STATE_FUNC)(void);
	typedef void(CBullet::*COLLISION_FUNC)(void);
	static STATE_FUNC m_FuncList[];
	static COLLISION_FUNC m_CollisionFuncList[];	// �����蔻��̃��X�g
};


#endif