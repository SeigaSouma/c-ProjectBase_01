//=============================================================================
// 
//  �G���A�����w�b�_�[ [limitarea.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _LIMITAREA_H_
#define _LIMITAREA_H_	// ��d�C���N���[�h�h�~

#include "meshwall.h"
#include "listmanager.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �G���A�����N���X��`
class CLimitArea : public CObject
{
public:

	// ��ԗ�
	enum STATE
	{
		STATE_NONE = 0,
		STATE_APPEARANCE,	// �o��
		STATE_FADEOUT,		// ����
		STATE_MAX
	};

	// �G���A�\����
	struct sLimitEreaInfo
	{
		float fMinX;	// �ŏ�X
		float fMaxX;	// �ő�X
		float fMinZ;	// �ŏ�Z
		float fMaxZ;	// �ő�Z
	};

	CLimitArea(int nPriority = 7);
	~CLimitArea();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;

	void Kill(void);	// �폜����

	static CLimitArea *Create(sLimitEreaInfo info);	// ��������
	STATE GetState(void);		// ��Ԏ擾
	void SetState(STATE state);	// ��Ԑݒ�
	sLimitEreaInfo GetLimitEreaInfo(void);	// �G���A���擾
	static CListManager<CLimitArea> GetListObj(void) { return m_List; }	// ���X�g�擾

protected:

private:

	// �ǈʒu�̗�
	enum eWallTag
	{
		WALL_UP = 0,	// ��
		WALL_RIGHT,		// �E
		WALL_DOWN,		// ��O
		WALL_LEFT,		// ��
		WALL_MAX
	};

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CLimitArea::*STATE_FUNC)(void);
	static STATE_FUNC m_StateFuncList[];	// ��Ԃ̊֐����X�g

	//=============================
	// �����o�֐�
	//=============================
	void StateNone(void);		// �ʏ�
	void StateAppearance(void);	// �o�����
	void StateFadeout(void);	// �t�F�[�h�A�E�g

	//=============================
	// �����o�ϐ�
	//=============================
	STATE m_state;						// ���
	float m_fTimeState;					// ��ԃJ�E���^�[
	sLimitEreaInfo m_sLimitEreaInfo;	// �G���A�������
	CMeshWall *m_pMeshWall[mylib_const::SHAPE_LIMITEREA];	// ���b�V���E�H�[���̃I�u�W�F�N�g
	static CListManager<CLimitArea> m_List;	// ���X�g
};



#endif