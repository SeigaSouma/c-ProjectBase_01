//=============================================================================
// 
//  X�t�@�C���w�b�_�[ [Xload.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _XLOAD_H_
#define _XLOAD_H_	// ��d�C���N���[�h�h�~

#include "object.h"

class CObjectX;

//==========================================================================
// �N���X��`
//==========================================================================
// X�t�@�C���N���X��`
class CXLoad
{
public:

	// �}�N����`
#define MAX_MAT				(512)		// �}�e���A���T�C�Y

	struct SXFile
	{
		MyLib::Vector3 vtxMin;					// ���f���̍ŏ��l
		MyLib::Vector3	vtxMax;					// ���f���̍ő�l
		BYTE *pVtxBuff;							// ���_�o�b�t�@�̃|�C���^
		LPD3DXMESH pMesh;						// ���b�V��(���_���)�ւ̃|�C���^
		LPD3DXBUFFER pBuffMat;					// �}�e���A���ւ̃|�C���^
		DWORD dwNumMat;							// �}�e���A���̐�
		D3DXMATERIAL pMatData[MAX_MAT];			// �}�e���A���̃f�[�^
		int nVtxNum;							// ���_��
		int nFaceNum;							// �ʂ̐�
		MyLib::Vector3 *pVtxPos;				// ���_���W
		float fMaxVtxDistance;					// ���_�Ԃ̍ő勗��
		int *nIdxTexture;						// �e�N�X�`���̃C���f�b�N�X�ԍ�
		std::string filename;					// �t�@�C����
		int nFileNameLen;						// �t�@�C�����̕�����
	};

	CXLoad();
	~CXLoad();

	HRESULT Init(void);
	void Uninit(void);
	int XLoad(std::string file);

	SXFile *GetMyObject(int nIdx);
	int GetNumAll(void);		// ����

	static CXLoad* Create(void);
	static CXLoad* GetInstance(void) { return m_pXX; }
protected:

private:
	HRESULT Load(std::string file);
	static void Unload(void);

	std::vector<SXFile> m_XFileInfo;	// X�t�@�C���̏��
	static CXLoad* m_pXX;	// ���g�̃|�C���^
};



#endif