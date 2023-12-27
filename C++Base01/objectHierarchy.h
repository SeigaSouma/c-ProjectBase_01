//=============================================================================
// 
//  �K�w�I�u�W�F�N�g�w�b�_�[ [objectHierarchy.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECTHIERARCHY_H_
#define _OBJECTHIERARCHY_H_	// ��d�C���N���[�h�h�~

#include "object.h"

class CModel;

//==========================================================================
// �N���X��`
//==========================================================================
// �K�w�I�u�W�F�N�g�N���X��`
class CObjectHierarchy : public CObject
{
public:

	// �\���̒�`
	struct LoadData
	{
		std::string pModelFile;	// ���f���t�@�C����
		int nType;				// ���f�����
		int nParent;			// �e�̃C���f�b�N�X
		int nStart;				// �ŏ����炠�邩�ǂ���
		int nSwitchType;		// �؂�ւ��̎��
		int nIDSwitchModel;		// �؂�ւ����f����ID
		D3DXVECTOR3 pos;		// �ʒu
		D3DXVECTOR3 rot;		// ����
	};

	struct Load
	{
		std::string sTextFile;	// �e�L�X�g�t�@�C����
		int nNumModel;			// ���f����
		float fVelocity;		// �ړ����x
		float fRadius;			// ���a
		int nLife;				// �̗�
		int nMotionStartIdx;	// ���[�V�����J�n�̃C���f�b�N�X�ԍ�
		int nAddScore;			// �X�R�A���Z��
		D3DXVECTOR3 posOrigin;	// �ŏ��̈ʒu
		LoadData LoadData[mylib_const::MAX_MODEL];
	};

	// �񋓌^��`
	typedef enum
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ�
		STATE_MAX
	}STATE;

	CObjectHierarchy(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CObjectHierarchy();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	virtual void Draw(D3DXCOLOR col);
	virtual void Draw(float fAlpha);

	void SetmtxWorld(const D3DXMATRIX mtxWorld);
	D3DXMATRIX GetmtxWorld(void) const;			// ���[���h�}�g���b�N�X�擾
	D3DXVECTOR3 GetCenterPosition(void) const;		// ���S�̈ʒu�擾
	void SetOriginPosition(const D3DXVECTOR3 pos);	// �ŏ��̈ʒu�ݒ�
	D3DXVECTOR3 GetOriginPosition(void) const;		// �ŏ��̈ʒu�擾
	void SetRadius(const float fRadius);		// ���a�ݒ�
	float GetRadius(void) const;				// ���a�擾

	HRESULT ReadText(const std::string pTextFile);	// �O���t�@�C���ǂݍ��ݏ���
	virtual HRESULT SetCharacter(const std::string pTextFile);	// �L�����N�^�[�ݒ�

	void ChangeObject(int nDeleteParts, int nNewParts);
	void ChangeObject(int nSwitchType);		// �؂�ւ��̎��
	void SetObject(int nNewParts);			// ���f���̐ݒ�
	void DeleteObject(int nSwitchType);		// ���f���폜

	int GetNumModel(void);					// ���f�����擾
	void SetNumModel(int nNumModel);		// ���f�����ݒ�
	int GetIdxFile(void);					// �t�@�C���̃C���f�b�N�X�ԍ��擾
	Load GetLoadData(int nIdx);				// �ǂݍ��ݏ��擾
	CModel **GetModel(void);				// ���f���擾
	CObjectHierarchy* GetObjectHierarchy(void);	// �K�w�I�u�W�F�N�g�擾
	static CObjectHierarchy *Create(const std::string pTextFile);

protected:

	virtual void LoadObjectData(FILE* pFile, const char* pComment);	// �I�u�W�F�N�g���̃f�[�^�ǂݍ���
	virtual void LoadPartsData(FILE* pFile, const char* pComment, int *pCntParts);	// �p�[�c���̃f�[�^�ǂݍ���
	virtual void BindObjectData(int nCntData);	// �I�u�W�F�N�g���̃f�[�^���蓖��

	CModel* m_apModel[mylib_const::MAX_MODEL];	// ���f��(�p�[�c)�̃|�C���^
	static Load m_aLoadData[mylib_const::MAX_MODEL];
	static int m_nNumLoad;	// �ǂݍ��񂾐�
private:

	// �����o�֐�
	void CalWorldMtx(void);		// ���[���h�}�g���b�N�X�̌v�Z����

	// �����o�ϐ�
	D3DXMATRIX	m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_posOrigin;	// �ŏ��̈ʒu
	float m_fRadius;			// ���a
	int m_nNumModel;			// ���f���̐�
	int m_nIdxFile;				// �t�@�C���̃C���f�b�N�X�ԍ�

};



#endif