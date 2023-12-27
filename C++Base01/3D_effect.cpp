//=============================================================================
// 
// �G�t�F�N�g���� [3D_effect.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "3D_effect.h"
#include "renderer.h"
#include "texture.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define DEF_RADIUS			(20.0f)
#define EFFECT_3D_LIFE		(30)
#define EFFECT_3DSIZE1		(0.97f)
#define EFFECT_3DSIZE2		(0.98f)
#define EFFECT_3DSIZE3		(0.99f)

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
const char *CEffect3D::m_apTextureFile[] =					// �t�@�C���ǂݍ���
{
	"data\\TEXTURE\\effect\\effect000.jpg",	   // �ʏ�G�t�F�N�g
	"data\\TEXTURE\\effect\\effect_point01.tga",	   // �_�G�t�F�N�g
	"data\\TEXTURE\\effect\\smoke_05.tga",	   // ���G�t�F�N�g
	"data\\TEXTURE\\effect\\smoke_05.tga",	   // ����
	"data\\TEXTURE\\effect\\effect000.png",	   // ���G�t�F�N�g
	"data\\TEXTURE\\effect\\effect001.png",		// �\���G�t�F�N�g
	"data\\TEXTURE\\effect\\Star01.png",		// �\���G�t�F�N�g
	"data\\TEXTURE\\effect\\sand_01.png",		// ���G�t�F�N�g
	"data\\TEXTURE\\effect\\sand_02.png",		// ���G�t�F�N�g
	"data\\TEXTURE\\effect\\line_02.png",		// ���G�t�F�N�g
	"data\\TEXTURE\\grassblades_01.png",		// ���G�t�F�N�g
	"data\\TEXTURE\\effect\\sweat_01.png",		// ���G�t�F�N�g
	"data\\TEXTURE\\effect\\thunder_02.tga",	// ���G�t�F�N�g
	NULL,										// NULL�G�t�F�N�g
};
int CEffect3D::m_nNumAll = 0;	// ����
int CEffect3D::m_nTexIdx[TYPE_MAX] = {};	// �e�N�X�`���̃C���f�b�N�X�ԍ�

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEffect3D::CEffect3D(int nPriority) : CObjectBillboard(nPriority)
{
	// �l�̃N���A
	m_posOrigin = mylib_const::DEFAULT_VECTOR3;			// ���_
	m_updatePosition = mylib_const::DEFAULT_VECTOR3;	// �X�V��̈ʒu
	m_setupPosition = mylib_const::DEFAULT_VECTOR3;		// �Z�b�g�A�b�v�ʒu
	m_posDest = mylib_const::DEFAULT_VECTOR3;			// �ڕW�̈ʒu
	m_colOrigin = mylib_const::DEFAULT_COLOR;	// �F�̌�
	m_pMtxParent = NULL;						// �e�}�g���b�N�X�̃|�C���^
	m_fRadius = 0.0f;							// ���a
	m_fMaxRadius = 0.0f;						// �ő唼�a
	m_fAddSizeValue = 0.0f;						// �T�C�Y�ύX��
	m_fGravity = 0.0f;							// �d��
	m_nLife = 0;								// ����
	m_nMaxLife = 0;								// �ő����(�Œ�)
	m_moveType = MOVEEFFECT_NONE;				// �ړ��̎��
	m_nType = TYPE_NORMAL;						// ���
	m_pParent = NULL;							// �e�̃|�C���^
	m_bAddAlpha = true;							// ���Z�����̔���
	m_bGravity = false;							// �d�͂̃t���O
	m_bChaseDest = false;						// �ڕW�̈ʒu�֌������t���O

	// �������Z
	m_nNumAll++;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEffect3D::~CEffect3D()
{
	
}

//==========================================================================
// �e�N�X�`���ǂݍ���
//==========================================================================
void CEffect3D::LoadTexture(void)
{
	// �e�N�X�`���̓ǂݍ���
	for (int nCntTex = 0; nCntTex < sizeof(m_apTextureFile) / sizeof(*m_apTextureFile); nCntTex++)
	{// �e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
		m_nTexIdx[nCntTex] = CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[nCntTex]);
	}
}

//==========================================================================
// ��������
//==========================================================================
CEffect3D *CEffect3D::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CEffect3D *pEffect = NULL;

	if (pEffect == NULL)
	{// NULL��������

		// �������̊m��
		pEffect = DEBUG_NEW CEffect3D;

		if (pEffect != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pEffect->Init();
		}

		return pEffect;
	}

	return NULL;
}

//==========================================================================
// ��������
//==========================================================================
CEffect3D *CEffect3D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXCOLOR col, const float fRadius, const int nLife, const int moveType, const TYPE type, const float fAddSizeValue)
{
	// �����p�̃I�u�W�F�N�g
	CEffect3D *pEffect = NULL;

	if (pEffect == NULL)
	{// NULL��������

		// �������̊m��
		pEffect = DEBUG_NEW CEffect3D;

		if (pEffect != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pEffect->m_fAddSizeValue = fAddSizeValue;	// �T�C�Y�ύX��
			HRESULT hr = pEffect->Init(pos, move, col, fRadius, nLife, moveType, type);
			if (FAILED(hr))
			{
				return NULL;
			}
		}

		return pEffect;
	}

	return NULL;
}

//==================================================================================
// �G�t�F�N�g�̏���������
//==================================================================================
HRESULT CEffect3D::Init(void)
{
	HRESULT hr;

	// �e��ϐ��̏�����
	m_posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���_
	m_updatePosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �X�V��̈ʒu
	m_setupPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �Z�b�g�A�b�v�ʒu
	m_colOrigin = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F�̌�
	m_fRadius = DEF_RADIUS;						// ���a
	m_fMaxRadius = m_fRadius;					// �ő唼�a
	m_nLife = EFFECT_3D_LIFE;					// ����
	m_nMaxLife = m_nLife;						// �ő����(�Œ�)
	m_moveType = MOVEEFFECT_NONE;				// �ړ��̎��
	m_nType = TYPE_NORMAL;						// ���
	m_bAddAlpha = true;							// ���Z�����̔���
	m_fGravity = mylib_const::GRAVITY;

	// ��ނ̐ݒ�
	SetType(TYPE_EFFECT3D);

	// �e�N�X�`���̊��蓖��
	//if (m_nTexIdx[m_nType] == 0)
	//{
	//	m_nTexIdx[m_nType] = CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[m_nType]);
	//}

	//// �e�N�X�`���̊��蓖��
	//BindTexture(m_nTexIdx[m_nType]);

	// ����������
	hr = CObjectBillboard::Init();

	if (FAILED(hr))
	{// ���s�����Ƃ�

		Uninit();
		return E_FAIL;
	}

	return S_OK;
}

//==================================================================================
// �G�t�F�N�g�̏���������
//==================================================================================
HRESULT CEffect3D::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXCOLOR col, const float fRadius, const int nLife, const int moveType, const TYPE type)
{
	HRESULT hr;

	// �e��ϐ��̏�����
	m_posOrigin = pos;							// ���_
	m_updatePosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �X�V��̈ʒu
	m_setupPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �Z�b�g�A�b�v�ʒu
	SetPosition(pos);							// �ʒu
	SetMove(move);								// �ړ���
	m_colOrigin = col;							// �F�̌�
	SetColor(col);								// �F
	m_fRadius = fRadius;						// ���a
	m_fMaxRadius = m_fRadius;					// �ő唼�a
	SetSize(D3DXVECTOR2(m_fRadius, m_fRadius));	// �T�C�Y�ݒ�
	m_nLife = nLife;							// ����
	m_nMaxLife = m_nLife;						// �ő����(�Œ�)
	m_moveType = moveType;						// �ړ��̎��
	m_nType = type;								// ���

	if (m_nType >= TYPE_MAX)
	{
		int n = 0;
	}

	// ���Z�����̔���
	switch (m_nType)
	{
	case TYPE_NORMAL:
		m_bAddAlpha = true;
		break;

	case TYPE_POINT:
		m_bAddAlpha = true;
		break;

	case TYPE_SMOKE:
		m_bAddAlpha = true;
		break;

	case TYPE_SMOKEBLACK:
		m_bAddAlpha = false;
		break;

	case TYPE_BLACK:
		m_bAddAlpha = false;
		break;

	case TYPE_JUJI:
		m_bAddAlpha = true;
		break;

	case TYPE_JUJI2:
		m_bAddAlpha = true;
		break;

	case TYPE_SAND:
		m_bAddAlpha = true;
		break;

	case TYPE_SANDBLACK:
		m_bAddAlpha = false;
		break;

	case TYPE_LINE:
		m_bAddAlpha = true;
		break;

	case TYPE_GRASS:
		m_bAddAlpha = false;
		break;

	case TYPE_SWEAT:
		m_bAddAlpha = false;
		break;

	case TYPE_THUNDER:
		m_bAddAlpha = true;
		break;

	default:
		m_bAddAlpha = true;
		break;
	}

	// ��ނ̐ݒ�
	SetType(TYPE_EFFECT3D);

	// �e�N�X�`���̊��蓖��
	//if (m_nTexIdx[m_nType] == 0)
	//{
	//	m_nTexIdx[m_nType] = CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[m_nType]);
	//}

	//// �e�N�X�`���̊��蓖��
	//BindTexture(m_nTexIdx[m_nType]);

	// ����������
	hr = CObjectBillboard::Init();

	if (FAILED(hr))
	{// ���s�����Ƃ�
		Uninit();
		return E_FAIL;
	}

	return S_OK;
}

//==================================================================================
// �G�t�F�N�g�̏I������
//==================================================================================
void CEffect3D::Uninit(void)
{
	if (m_pParent != NULL)
	{
		// �G�t�F�N�g�̊J��
		m_pParent->ReleaseEffect(m_nParentIdx);
		m_pParent = NULL;
	}

	// �������Z
	m_nNumAll--;

	// �I������
	CObjectBillboard::Uninit();
}

//==================================================================================
// �e�̔j��
//==================================================================================
void CEffect3D::UninitParent(void)
{
	// �e��NULL�ɂ���
	m_pParent = NULL;
}

//==================================================================================
// �G�t�F�N�g�̍X�V����
//==================================================================================
void CEffect3D::Update(void)
{
	// �ߋ��̈ʒu�ݒ�
	SetOldPosition(GetPosition());

	// �ړ�����
	UpdateMove();

	switch (m_moveType)
	{
	case MOVEEFFECT_ADD:
		AddSize();
		break;

	case MOVEEFFECT_SUB:
		SubSize();
		break;

	case MOVEEFFECT_SUPERSUB:
		SuperSubSize();
		break;

	case MOVEEFFECT_GENSUI:
		Gensui();
		break;
	}

	// �T�C�Y�ݒ�
	SetSize(D3DXVECTOR2(m_fRadius, m_fRadius));

	// �����̍X�V
	m_nLife--;

	// �F�擾
	D3DXCOLOR col = GetColor();

	// �s�����x�̍X�V
	col.a = m_colOrigin.a * ((float)m_nLife / (float)m_nMaxLife);

	// �F�ݒ�
	SetColor(col);

	if (m_nLife <= 0)
	{// �������s������

		// �G�t�F�N�g�̍폜
		Uninit();
		return;
	}

	// ���_���W�̐ݒ�
	SetVtx();

}

//==================================================================================
// �ړ�����
//==================================================================================
void CEffect3D::UpdateMove(void)
{
	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posOld = GetOldPosition();

	// �ړ��ʎ擾
	D3DXVECTOR3 move = GetMove();

	// �ʒu�X�V
	if (m_bGravity == true)
	{
		move.y -= m_fGravity;
	}

	// �␳�ʏ���
	if (m_bChaseDest == false)
	{
		m_updatePosition += move;
		pos = m_posOrigin + m_updatePosition;
	}
	else
	{
		// �������`���
		float fRatio = 1.0f - ((float)m_nLife / (float)m_nMaxLife);
		pos.x = EasingLinear(m_posOrigin.x, m_posDest.x, fRatio);
		pos.y = EasingLinear(m_posOrigin.y, m_posDest.y, fRatio);
		pos.z = EasingLinear(m_posOrigin.z, m_posDest.z, fRatio);
	}

	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);
}


//==================================================================================
// �Z�b�g�A�b�v
//==================================================================================
void CEffect3D::SetUp(D3DXVECTOR3 setup, D3DXMATRIX *pMtxParent, CObject *pObj, int nParentIdx)
{
	// �e�̃|�C���^�n��
	if (m_pParent == NULL)
	{
		m_pParent = pObj;
	}

	// �e�}�g���b�N�X�̃|�C���^
	m_pMtxParent = pMtxParent;

	// �e�̃C���f�b�N�X�ԍ�
	m_nParentIdx = nParentIdx;

	// �Z�b�g�A�b�v�ʒu
	m_setupPosition = setup;
}

//==================================================================================
// �ʒu�X�V
//==================================================================================
void CEffect3D::UpdatePosition(D3DXVECTOR3 rot)
{

	D3DXMATRIX mtxRot, mtxTrans, mtxWorld;	// �v�Z�p�}�g���b�N�X�錾

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, m_setupPosition.x, m_setupPosition.y, m_setupPosition.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// �����ɐe�̃��[���h�}�g���b�N�X���|����
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, m_pMtxParent);

	m_posOrigin = WorldMtxChangeToPosition(mtxWorld);

}

//==================================================================================
// �G�t�F�N�g�̏k������
//==================================================================================
void CEffect3D::SubSize(void)
{
	int nEffect_3DType = rand() % 3;

	if (nEffect_3DType == 0)
	{
		m_fRadius *= EFFECT_3DSIZE1;
	}
	else if (nEffect_3DType == 1)
	{
		m_fRadius *= EFFECT_3DSIZE2;
	}
	else if (nEffect_3DType == 2)
	{
		m_fRadius *= EFFECT_3DSIZE3;
	}
}

//==================================================================================
// �G�t�F�N�g�̏k������
//==================================================================================
void CEffect3D::SuperSubSize(void)
{
	m_fRadius = m_fMaxRadius * (float)m_nLife / (float)m_nMaxLife;
}

//==================================================================================
// �G�t�F�N�g�̊g�又��
//==================================================================================
void CEffect3D::AddSize(void)
{
	// �g��
	m_fRadius += m_fAddSizeValue;
}

//==================================================================================
// �G�t�F�N�g�̌�������
//==================================================================================
void CEffect3D::Gensui(void)
{
	// �ړ��ʎ擾
	D3DXVECTOR3 move = GetMove();

	move.x += (0.0f - move.x) * 0.15f;
	move.y += (0.0f - move.y) * 0.15f;
	move.z += (0.0f - move.z) * 0.15f;

	// �ړ��ʐݒ�
	SetMove(move);

	m_fRadius = m_fMaxRadius * (float)m_nLife / (float)m_nMaxLife;

}

//==================================================================================
// �G�t�F�N�g�̕`�揈��
//==================================================================================
void CEffect3D::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���u�����f�B���O�����Z�����ɐݒ�
	if (m_bAddAlpha == true)
	{
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	// �`�揈��
	CObjectBillboard::Draw();

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CEffect3D::SetVtx(void)
{
	// ���_�ݒ�
	CObjectBillboard::SetVtx();
}

//==========================================================================
// �ڕW�̈ʒu�ݒ�
//==========================================================================
void CEffect3D::SetPositionDest(D3DXVECTOR3 pos)
{
	m_posDest = pos;
	m_bChaseDest = true;	// �L��
}

//==========================================================================
// �d�͂̒l�ݒ�
//==========================================================================
void CEffect3D::SetGravityValue(float fValue)
{ 
	m_fGravity = fValue; 
	m_bGravity = true;	// �L��
}

//==========================================================================
// �����擾
//==========================================================================
int CEffect3D::GetNumAll(void)
{
	return m_nNumAll;
}