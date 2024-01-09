//=============================================================================
// 
//  �e�N�X�`������ [texture.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "texture.h"
#include "manager.h"
#include "renderer.h"

// �ǂݍ��ރe�N�X�`���̃w�b�_�[
#include "map.h"
#include "3D_effect.h"
#include "effect_slashhit.h"
#include "effect_thunderring.h"
#include "effect_enemyspawn.h"
#include "effect_startupeye.h"

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CTexture* CTexture::m_pTexture = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTexture::CTexture()
{
	m_TexInfo.clear();
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTexture::~CTexture()
{

}

//==========================================================================
// ��������
//==========================================================================
CTexture* CTexture::Create(void)
{
	if (m_pTexture == nullptr)
	{// �܂��������Ă��Ȃ�������

		// �C���X�^���X����
		m_pTexture = DEBUG_NEW CTexture;
		m_pTexture->Init();
	}
	else
	{
		// �C���X�^���X�擾
		m_pTexture->GetInstance();
	}

	return m_pTexture;
}

//==========================================================================
// ����������
//==========================================================================
void CTexture::Init(void)
{
	STexture init = {};
	m_TexInfo.emplace_back();

	// �ǂݍ���
	Load();

	// �S�ēǂݍ���
	LoadAll();
}

//==========================================================================
// �e�N�X�`���̓ǂݍ���
//==========================================================================
HRESULT CTexture::LoadAll(void)
{
	// �}�b�v�p�̓ǂݍ���
	if (FAILED(MyMap::ReadTexture()))
	{// ���s�����ꍇ
		return E_FAIL;
	}

	// 3D�G�t�F�N�g
	CEffect3D::LoadTexture();

	// �a���q�b�g
	CEffectSlashHit::LoadTexture();

	// ���̃����O
	CThunderRing::LoadTexture();

	// �G�X�|�[��
	CEffectEnemySpawn::LoadTexture();

	// �N�����̖�
	CEffectStartupEye::LoadTexture();

	return S_OK;
}

//==========================================================================
// ����e�N�X�`���̓ǂݍ���
//==========================================================================
HRESULT CTexture::Load(void)
{
	return S_OK;
}

//==========================================================================
// �S�Ẵe�N�X�`���̔j��
//==========================================================================
void CTexture::Unload(void)
{

	for (const auto& texture : m_TexInfo)
	{
		if (texture.pTexture == nullptr)
		{
			continue;
		}
		texture.pTexture->Release();
	}

	m_TexInfo.clear();
	m_TexInfo.shrink_to_fit();
}

//==========================================================================
// �e�N�X�`���̊��蓖�ď���
//==========================================================================
int CTexture::Regist(std::string file)
{
	if (file == "")
	{
		return 0;
	}

	int nNumAll = GetNumAll();	// �ő吔�擾
	int nIdx = -1;	// ����̔ԍ��ۑ�
	int nNowLen = 0;	// ����̃t�@�C��������
	nNowLen = file.length();

	for (int nCntData = 0; nCntData < nNumAll; nCntData++)
	{
		if (m_TexInfo[nCntData].nFileNameLen != nNowLen)
		{// �t�@�C�����̒�����������������
			continue;
		}

		if (m_TexInfo[nCntData].filename.length() != nNowLen)
		{// �t�@�C�����̒������������
			continue;
		}

		// ���Ƀe�N�X�`�����ǂݍ��܂�ĂȂ����̍ŏI�m�F
		if (m_TexInfo[nCntData].filename == file)
		{// �t�@�C��������v���Ă���

			// �ԍ����蓖��
			nIdx = nCntData;
			return nIdx;
		}
	}

	// �e�N�X�`���ǂݍ���
	HRESULT hr = LoadTex(file);
	if (FAILED(hr))
	{// ���s���Ă�����
		return 0;
	}

	// �ԍ����蓖��
	nIdx = nNumAll;
	return nIdx;
}

//==========================================================================
// �e�N�X�`���̓ǂݍ��ݏ���
//==========================================================================
HRESULT CTexture::LoadTex(std::string file)
{
	HRESULT hr;
	int nIdx = static_cast<int>(m_TexInfo.size());

	// ���蓖��
	m_TexInfo.emplace_back();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevive = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	hr = D3DXCreateTextureFromFile(pDevive,
		file.c_str(),
		&m_TexInfo[nIdx].pTexture);

	if (hr == D3DXERR_INVALIDDATA)
	{// ���s�����Ƃ�

		// ���蓖��
		m_TexInfo.erase(m_TexInfo.end() - 1);
		return E_FAIL;
	}

	// �e�N�X�`���f�ޏ��
	D3DXGetImageInfoFromFile(file.c_str(), &m_TexInfo[nIdx].imageInfo);

	// �t�@�C�����ƒ����ۑ�
	m_TexInfo[nIdx].filename = file;
	m_TexInfo[nIdx].nFileNameLen = m_TexInfo[nIdx].filename.length();

	return S_OK;
}

//==========================================================================
// �e�N�X�`���̃A�h���X�擾
//==========================================================================
LPDIRECT3DTEXTURE9 CTexture::GetAdress(int nIdx)
{
	return m_TexInfo[nIdx].pTexture;
}

//==========================================================================
// �e�N�X�`���̑����擾
//==========================================================================
int CTexture::GetNumAll(void)
{
	return static_cast<int>(m_TexInfo.size());
}

//==========================================================================
// �e�N�X�`�����擾
//==========================================================================
CTexture::STexture CTexture::GetTextureInfo(std::string file)
{
	// �ő吔�擾
	int nNumAll = GetNumAll() + 1;

	if (file == "")
	{
		return m_TexInfo[nNumAll];
	}

	int nNowLen = file.length();	// ����̃t�@�C��������

	for (int nCntData = 0; nCntData < nNumAll; nCntData++)
	{
		if (m_TexInfo[nCntData].nFileNameLen != nNowLen)
		{// �t�@�C�����̒������������
			continue;
		}

		// ���Ƀe�N�X�`�����ǂݍ��܂�ĂȂ����̍ŏI�m�F
		if (m_TexInfo[nCntData].filename == file)
		{// �t�@�C��������v���Ă���

			return m_TexInfo[nCntData];
		}
	}

	int nIdx = nNumAll + 1;
	return m_TexInfo[nIdx];
}

//==========================================================================
// �e�N�X�`�����擾
//==========================================================================
CTexture::STexture CTexture::GetTextureInfo(int nIdxTex)
{
	return m_TexInfo[nIdxTex];
}

//==========================================================================
// �e�N�X�`���f�ނ̃T�C�Y�擾
//==========================================================================
D3DXVECTOR2 CTexture::GetImageSize(int nIdx)
{
	if (static_cast<int>(m_TexInfo.size()) >= nIdx)
	{
		return mylib_const::DEFAULT_VECTOR2;
	}
	return D3DXVECTOR2((float)m_TexInfo[nIdx].imageInfo.Width, (float)m_TexInfo[nIdx].imageInfo.Height);
}
