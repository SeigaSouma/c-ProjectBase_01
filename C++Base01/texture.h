//=============================================================================
// 
// �e�N�X�`���w�b�_�[ [texture.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TEXTURE_H_
#define _TEXTURE_H_	// ��d�C���N���[�h�h�~

#include "constans.h"
#include "constans.h"

//==========================================================================
// �N���X��`
//==========================================================================
//�e�N�X�`���N���X��`
class CTexture
{
private:
	struct STexture
	{
		LPDIRECT3DTEXTURE9 pTexture;	// �e�N�X�`���̃|�C���^
		D3DXIMAGE_INFO imageInfo;		// �e�N�X�`���f�ޏ��
		std::string filename;			// �t�@�C����
		int nFileNameLen;				// �t�@�C�����̕�����
	};

public:

	CTexture();
	~CTexture();

	void Init(void);	// ����������
	HRESULT Load(void);
	HRESULT LoadAll(void);
	void Unload(void);
	int Regist(std::string file);
	LPDIRECT3DTEXTURE9 GetAdress(int nIdx);
	D3DXVECTOR2 GetImageSize(int nIdx);		// �e�N�X�`���f�ނ̃T�C�Y�擾

	int GetNumAll(void);		// �ǂݍ��񂾑���
	STexture GetTextureInfo(std::string file);	// �e�N�X�`�����擾
	STexture GetTextureInfo(int nIdxTex);	// �e�N�X�`�����擾

	static CTexture* Create(void);
	static CTexture* GetInstance(void) { return m_pTexture; }
private:

	// �����o�֐�
	HRESULT LoadTex(std::string file);

	// �����o�ϐ�
	std::vector<STexture> m_TexInfo;	// �e�N�X�`���̏��
	static CTexture* m_pTexture;	// ���g�̃|�C���^
};



#endif