//=============================================================================
// 
//  �K�w�I�u�W�F�N�g���� [objectHierarchy.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "objectHierarchy.h"
#include "calculation.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CObjectHierarchy::Load CObjectHierarchy::m_aLoadData[mylib_const::MAX_MODEL] = {};	// �ǂݍ��݃f�[�^
int CObjectHierarchy::m_nNumLoad = 0;	// �ǂݍ��񂾐�

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObjectHierarchy::CObjectHierarchy(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	D3DXMatrixIdentity(&m_mtxWorld);			// ���[���h�}�g���b�N�X
	m_posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ŏ��̈ʒu
	m_fRadius = 0.0f;			// ���a
	m_nNumModel = 0;			// ���f���̐�
	m_nIdxFile = 0;				// �t�@�C���̃C���f�b�N�X�ԍ�

	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		m_apModel[nCntModel] = NULL;	// ���f��(�p�[�c)�̃|�C���^
	}
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObjectHierarchy::~CObjectHierarchy()
{

}

//==========================================================================
// ��������
//==========================================================================
CObjectHierarchy *CObjectHierarchy::Create(const std::string pTextFile)
{
	// �����p�̃I�u�W�F�N�g
	CObjectHierarchy *pObjChara = NULL;

	if (pObjChara == NULL)
	{// NULL��������

		// �������̊m��
		pObjChara = DEBUG_NEW CObjectHierarchy;

		if (pObjChara != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			HRESULT hr = pObjChara->SetCharacter(pTextFile);
			if (FAILED(hr))
			{// ���s���Ă�����
				return NULL;
			}
		}

		return pObjChara;
	}

	return NULL;
}

//==========================================================================
// �L�����쐬
//==========================================================================
HRESULT CObjectHierarchy::SetCharacter(const std::string pTextFile)
{
	// ����������
	if (FAILED(CObjectHierarchy::Init()))
	{// ���s���Ă�����
		return E_FAIL;
	}

	// �ǂݍ��݊m�F
	for (int nCntData = 0; nCntData < m_nNumLoad; nCntData++)
	{
		if (m_aLoadData[nCntData].sTextFile == pTextFile)
		{// �t�@�C��������v���Ă���

			// �I�u�W�F�N�g���̃f�[�^���蓖��
			BindObjectData(nCntData);
			return S_OK;
		}
	}

	if (FAILED(CObjectHierarchy::ReadText(pTextFile)))
	{// ���s���Ă�����
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �I�u�W�F�N�g���̃f�[�^���蓖��
//==========================================================================
void CObjectHierarchy::BindObjectData(int nCntData)
{
	// ���f�����ݒ�
	m_nNumModel = m_aLoadData[nCntData].nNumModel;

	// �ŏ��̈ʒu�ݒ�
	m_posOrigin = m_aLoadData[nCntData].posOrigin;

	// ���a
	m_fRadius = m_aLoadData[nCntData].fRadius;

	// �t�@�C���̃C���f�b�N�X�ԍ�
	m_nIdxFile = nCntData;

	for (int nCntParts = 0; nCntParts < m_aLoadData[nCntData].nNumModel; nCntParts++)
	{// �p�[�c���J��Ԃ�

		// ���f���̐���
		m_apModel[nCntParts] = CModel::Create(
			m_aLoadData[nCntData].LoadData[m_aLoadData[nCntData].LoadData[nCntParts].nType].pModelFile.c_str(),
			m_aLoadData[nCntData].LoadData[nCntParts].pos,
			m_aLoadData[nCntData].LoadData[nCntParts].rot);

		if (m_apModel[nCntParts] == NULL)
		{// NULL��������

			// ���f���̏I������
			m_apModel[nCntParts]->Uninit();
			delete m_apModel[nCntParts];
			m_apModel[nCntParts] = NULL;
		}

		// �e���f���̐ݒ�
		if (m_aLoadData[nCntData].LoadData[nCntParts].nParent >= 0)
		{
			// �e�̃��f���I�u�W�F�N�g�ݒ�
			m_apModel[nCntParts]->SetParent(m_apModel[m_aLoadData[nCntData].LoadData[nCntParts].nParent]);
		}
		else
		{// �������e�̎�
			m_apModel[nCntParts]->SetParent(NULL);
		}

		// �����z�u�̔���
		if (m_aLoadData[nCntData].LoadData[nCntParts].nStart != 1)
		{
			ChangeObject(nCntParts, -1);
		}
	}
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObjectHierarchy::Init(void)
{
	
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CObjectHierarchy::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULL����Ȃ�������

			// ���f���̏I������
			m_apModel[nCntModel]->Uninit();
			delete m_apModel[nCntModel];
			m_apModel[nCntModel] = NULL;
		}
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CObjectHierarchy::Update(void)
{

}

//==========================================================================
// ���f���ύX
//==========================================================================
void CObjectHierarchy::ChangeObject(int nDeleteParts, int nNewParts)
{
	// �t�@�C�����擾
	Load LoadData = GetLoadData(m_nIdxFile);

	// �I�u�W�F�N�g�L�����N�^�[�̏��擾
	CObjectHierarchy *pObjChar = GetObjectHierarchy();

	int nNumAll = pObjChar->GetNumModel();

	if (m_apModel[nDeleteParts] != NULL)
	{// NULL����Ȃ�������

		// ���f���̏I������
		m_apModel[nDeleteParts]->Uninit();
		delete m_apModel[nDeleteParts];
		m_apModel[nDeleteParts] = NULL;
	}

	// �V�����p�[�c��ǂݍ���
	if (nNewParts >= 0 && m_apModel[nNewParts] == NULL)
	{
		if (m_apModel[nNewParts] == NULL)
		{
			// ���f���쐬
			m_apModel[nNewParts] = CModel::Create(
				LoadData.LoadData[LoadData.LoadData[nNewParts].nType].pModelFile.c_str(),
				LoadData.LoadData[nNewParts].pos,
				LoadData.LoadData[nNewParts].rot);
		}

		if (m_apModel[nNewParts] == NULL)
		{// NULL��������

			// ���f���̏I������
			m_apModel[nNewParts]->Uninit();
			delete m_apModel[nNewParts];
			m_apModel[nNewParts] = NULL;
		}
	}

	// �e���f���̐ݒ�
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// �p�[�c���J��Ԃ�

		if (m_apModel[nCntParts] == NULL)
		{// NULL��������
			continue;
		}

		// �e���f���̐ݒ�
		if (LoadData.LoadData[nCntParts].nParent >= 0)
		{
			// �e�̃��f���I�u�W�F�N�g�ݒ�
			m_apModel[nCntParts]->SetParent(m_apModel[LoadData.LoadData[nCntParts].nParent]);
		}
		else
		{// �������e�̎�
			m_apModel[nCntParts]->SetParent(NULL);
		}
	}
}

//==========================================================================
// �؂�ւ��̎��
//==========================================================================
void CObjectHierarchy::ChangeObject(int nSwitchType)
{
	// �t�@�C�����擾
	Load LoadData = GetLoadData(m_nIdxFile);

#if _DEBUG
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// �p�[�c���J��Ԃ�

		if (m_apModel[nCntParts] != NULL)
		{
			// ���f���̏I������
			m_apModel[nCntParts]->Uninit();
			delete m_apModel[nCntParts];
			m_apModel[nCntParts] = NULL;
		}

		// ���f���쐬
		if (m_apModel[nCntParts] == NULL)
		{
			m_apModel[nCntParts] = CModel::Create(
				LoadData.LoadData[LoadData.LoadData[nCntParts].nType].pModelFile.c_str(),
				LoadData.LoadData[nCntParts].pos,
				LoadData.LoadData[nCntParts].rot);
		}

		// �e���f���̐ݒ�
		if (LoadData.LoadData[nCntParts].nParent >= 0)
		{
			// �e�̃��f���I�u�W�F�N�g�ݒ�
			m_apModel[nCntParts]->SetParent(m_apModel[LoadData.LoadData[nCntParts].nParent]);
		}
		else
		{// �������e�̎�
			m_apModel[nCntParts]->SetParent(NULL);
		}

		if (LoadData.LoadData[nCntParts].nStart != 1)
		{
			ChangeObject(nCntParts, -1);
		}
	}
#endif

	// ���f���̐؂�ւ�
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// �p�[�c���J��Ԃ�

		if (LoadData.LoadData[nCntParts].nSwitchType != nSwitchType)
		{// �؂�ւ��̎�ނ��Ⴄ�Ƃ�
			continue;
		}

		// �폜����C���f�b�N�X�ԍ�
		int nDeleteIdx = LoadData.LoadData[nCntParts].nIDSwitchModel;

		if (nDeleteIdx >= 0 && m_apModel[nDeleteIdx] != NULL)
		{// NULL����Ȃ�������

			// ���f���̏I������
			m_apModel[nDeleteIdx]->Uninit();
			delete m_apModel[nDeleteIdx];
			m_apModel[nDeleteIdx] = NULL;
		}

		// ��������C���f�b�N�X�ԍ�
		int nNewIdx = nDeleteIdx;

		if (nNewIdx < 0)
		{
			nNewIdx = nCntParts;
		}

		// ���f���쐬
		if (m_apModel[nNewIdx] == NULL)
		{
			m_apModel[nNewIdx] = CModel::Create(
				LoadData.LoadData[LoadData.LoadData[nCntParts].nType].pModelFile.c_str(),
				LoadData.LoadData[nCntParts].pos,
				LoadData.LoadData[nCntParts].rot);
		}

		// �e���f���̐ݒ�
		if (LoadData.LoadData[nCntParts].nParent >= 0)
		{
			// �e�̃��f���I�u�W�F�N�g�ݒ�
			m_apModel[nNewIdx]->SetParent(m_apModel[LoadData.LoadData[nCntParts].nParent]);
		}
		else
		{// �������e�̎�
			m_apModel[nNewIdx]->SetParent(NULL);
		}
	}

}

//==========================================================================
// ���f���ݒ�
//==========================================================================
void CObjectHierarchy::SetObject(int nNewParts)
{
	// �t�@�C�����擾
	Load LoadData = GetLoadData(m_nIdxFile);

	// �V�����p�[�c��ǂݍ���
	if (nNewParts >= 0 && m_apModel[nNewParts] == NULL)
	{
		if (m_apModel[nNewParts] == NULL)
		{
			// ���f���쐬
			m_apModel[nNewParts] = CModel::Create(
				LoadData.LoadData[LoadData.LoadData[nNewParts].nType].pModelFile.c_str(),
				LoadData.LoadData[nNewParts].pos,
				LoadData.LoadData[nNewParts].rot);
		}

		if (m_apModel[nNewParts] == NULL)
		{// NULL��������

			// ���f���̏I������
			m_apModel[nNewParts]->Uninit();
			delete m_apModel[nNewParts];
			m_apModel[nNewParts] = NULL;
		}
	}

	// �e���f���̐ݒ�
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// �p�[�c���J��Ԃ�

		if (m_apModel[nCntParts] == NULL)
		{// NULL��������
			continue;
		}

		// �e���f���̐ݒ�
		if (LoadData.LoadData[nCntParts].nParent >= 0)
		{
			// �e�̃��f���I�u�W�F�N�g�ݒ�
			m_apModel[nCntParts]->SetParent(m_apModel[LoadData.LoadData[nCntParts].nParent]);
		}
		else
		{// �������e�̎�
			m_apModel[nCntParts]->SetParent(NULL);
		}
	}
}

//==========================================================================
// ���f���폜
//==========================================================================
void CObjectHierarchy::DeleteObject(int nSwitchType)
{
	// �t�@�C�����擾
	Load LoadData = GetLoadData(m_nIdxFile);

	// ���f���̐؂�ւ�
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// �p�[�c���J��Ԃ�

		if (LoadData.LoadData[nCntParts].nSwitchType != nSwitchType)
		{// �؂�ւ��̎�ނ��Ⴄ�Ƃ�
			continue;
		}

		// �폜����C���f�b�N�X�ԍ�
		int nDeleteIdx = LoadData.LoadData[nCntParts].nIDSwitchModel;

		if (nDeleteIdx >= 0 && m_apModel[nDeleteIdx] != NULL)
		{// NULL����Ȃ�������

			// ���f���̏I������
			m_apModel[nDeleteIdx]->Uninit();
			delete m_apModel[nDeleteIdx];
			m_apModel[nDeleteIdx] = NULL;
		}
	}
}

//==========================================================================
// ���[���h�}�g���b�N�X�̌v�Z����
//==========================================================================
void CObjectHierarchy::CalWorldMtx(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

	// ���擾
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObjectHierarchy::Draw(void)
{
	// ���[���h�}�g���b�N�X�̌v�Z����
	CalWorldMtx();

	// ���f���̕`��
	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULL����Ȃ�������

			// �p�[�c���Ƃ̕`��
			m_apModel[nCntModel]->Draw();
		}
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObjectHierarchy::Draw(D3DXCOLOR col)
{
	// ���[���h�}�g���b�N�X�̌v�Z����
	CalWorldMtx();

	// ���f���̕`��
	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULL����Ȃ�������

			// �p�[�c���Ƃ̕`��
			m_apModel[nCntModel]->Draw(col);
		}
	}
}


//==========================================================================
// �`�揈��
//==========================================================================
void CObjectHierarchy::Draw(float fAlpha)
{
	// ���[���h�}�g���b�N�X�̌v�Z����
	CalWorldMtx();

	// ���f���̕`��
	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULL����Ȃ�������

			// �p�[�c���Ƃ̕`��
			m_apModel[nCntModel]->Draw(fAlpha);
		}
	}
}

//==========================================================================
// �e�L�X�g�ǂݍ��ݏ���
//==========================================================================
HRESULT CObjectHierarchy::ReadText(const std::string pTextFile)
{
	FILE *pFile = NULL;	// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	pFile = fopen(pTextFile.c_str(), "r");

	if (pFile == NULL)
	{//�t�@�C�����J�����ꍇ
		return E_FAIL;
	}

	// �t�@�C�����ۑ�
	m_aLoadData[m_nNumLoad].sTextFile = pTextFile;

	// �t�@�C���̃C���f�b�N�X�ԍ�
	m_nIdxFile = m_nNumLoad;

	char aComment[MAX_COMMENT];	// �R�����g
	int nCntFileName = 0;
	int nCntSetParts = 0;

	while (1)
	{// END_SCRIPT������܂ŌJ��Ԃ�

		// ������̓ǂݍ���
		fscanf(pFile, "%s", &aComment[0]);

		// ���f�����̐ݒ�
		if (strcmp(aComment, "NUM_MODEL") == 0)
		{// NUM_MODEL��������

			fscanf(pFile, "%s", &aComment[0]);	// =�̕�
			fscanf(pFile, "%d", &m_nNumModel);	// ���f����
			m_aLoadData[m_nNumLoad].nNumModel = m_nNumModel;
		}

		while (nCntFileName != m_nNumModel)
		{// ���f���̐����ǂݍ��ނ܂ŌJ��Ԃ�

			// ������̓ǂݍ���
			fscanf(pFile, "%s", &aComment[0]);

			// ���f�����̐ݒ�
			if (strcmp(aComment, "MODEL_FILENAME") == 0)
			{// NUM_MODEL��������

				fscanf(pFile, "%s", &aComment[0]);	// =�̕�
				fscanf(pFile, "%s", &aComment[0]);	// �t�@�C����

				// �t�@�C�����ۑ�
				m_aLoadData[m_nNumLoad].LoadData[nCntFileName].pModelFile = aComment;

				nCntFileName++;	// �t�@�C�������Z
			}
		}

		// �e�p�[�c�̐ݒ�
		if (strcmp(aComment, "CHARACTERSET") == 0)
		{// �L�����N�^�[���̓ǂݍ��݂��J�n

			while (strcmp(aComment, "END_CHARACTERSET") != 0)
			{// END_CHARACTERSET������܂ŌJ��Ԃ�

				fscanf(pFile, "%s", &aComment[0]);	//�m�F����

				// �I�u�W�F�N�g���̃f�[�^�ǂݍ���
				LoadObjectData(pFile, aComment);

				// �p�[�c���̃f�[�^�ǂݍ���
				LoadPartsData(pFile, aComment, &nCntSetParts);
			}
		}

		if (strcmp(aComment, "END_SCRIPT") == 0)
		{// �I�������Ń��[�v�𔲂���
			break;
		}
	}

	// �t�@�C�������
	fclose(pFile);

	// �ǂݍ��񂾐����Z
	m_nNumLoad++;

	return S_OK;
}

//==========================================================================
// �I�u�W�F�N�g���̃f�[�^�ǂݍ���
//==========================================================================
void CObjectHierarchy::LoadObjectData(FILE *pFile, const char* pComment)
{
	char hoge[MAX_COMMENT];	// �R�����g
	if (strcmp(pComment, "RADIUS") == 0)
	{// RADIUS�Ŕ��a

		fscanf(pFile, "%s", &hoge[0]);	// =�̕�
		fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].fRadius);	// ���a
		m_fRadius = m_aLoadData[m_nNumLoad].fRadius;
	}
}

//==========================================================================
// �p�[�c���̃f�[�^�ǂݍ���
//==========================================================================
void CObjectHierarchy::LoadPartsData(FILE* pFile, const char* pComment, int *pCntParts)
{
	char CheckText[MAX_COMMENT];	// �`�F�b�N�p
	char hoge[MAX_COMMENT];	// �R�����g

	int nCntSetParts = *pCntParts;

	if (strcmp(pComment, "PARTSSET") == 0)
	{// PARTSSET�Ńp�[�c���̓ǂݍ��݊J�n

		while (strcmp(CheckText, "END_PARTSSET") != 0)
		{// END_PARTSSET������܂ŌJ��Ԃ�

			fscanf(pFile, "%s", &CheckText[0]);	// �m�F����

			if (strcmp(CheckText, "INDEX") == 0)
			{// INDEX�������烂�f���ԍ��ǂݍ���

				fscanf(pFile, "%s", &hoge[0]);	// =�̕�
				fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nType);		// ���f����ނ̗�
			}

			if (strcmp(CheckText, "PARENT") == 0)
			{// PARENT��������e�̔ԍ��ǂݍ���

				fscanf(pFile, "%s", &hoge[0]);	// =�̕�
				fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent);		// �e�̔ԍ�
			}

			if (strcmp(CheckText, "POS") == 0)
			{// POS��������ʒu�ǂݍ���

				fscanf(pFile, "%s", &hoge[0]);		// =�̕�
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos.x);	// X���W
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos.y);	// Y���W
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos.z);	// Z���W

				if (nCntSetParts == 0)
				{
					m_posOrigin = m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos;	// �ŏ��̈ʒu
					m_aLoadData[m_nNumLoad].posOrigin = m_posOrigin;	// �ŏ��̈ʒu
				}
			}

			if (strcmp(CheckText, "ROT") == 0)
			{// ROT������������ǂݍ���

				fscanf(pFile, "%s", &hoge[0]);		//=�̕�
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot.x);	// X�̊p�x
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot.y);	// Y�̊p�x
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot.z);	// Z�̊p�x
			}

			if (strcmp(CheckText, "START") == 0)
			{// START�������珉�����

				fscanf(pFile, "%s", &hoge[0]);		//=�̕�
				fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nStart);	// �ŏ����炠�邩
			}

			if (strcmp(CheckText, "SWITCHTYPE") == 0)
			{// SWITCHTYPE��������؂�ւ��̎��

				fscanf(pFile, "%s", &hoge[0]);		//=�̕�
				fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nSwitchType);	// �؂�ւ��̎��
			}

			if (strcmp(CheckText, "ID_SWITCHMODEL") == 0)
			{// ID_SWITCHMODEL��������؂�ւ��郂�f��ID

				fscanf(pFile, "%s", &hoge[0]);		//=�̕�
				fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nIDSwitchModel);	// �؂�ւ��郂�f��ID
			}

		}// END_PARTSSET�̂�����

		// ���f���̐���
		if (m_apModel[nCntSetParts] == NULL)
		{
			m_apModel[nCntSetParts] = CModel::Create(
				m_aLoadData[m_nNumLoad].LoadData[m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nType].pModelFile.c_str(),
				m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos,
				m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot);
		}

		if (m_apModel[nCntSetParts] == NULL)
		{// NULL��������

			// ���f���̏I������
			m_apModel[nCntSetParts]->Uninit();
			delete m_apModel[nCntSetParts];
			m_apModel[nCntSetParts] = NULL;
		}

		// �e���f���̐ݒ�
		if (m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent >= 0)
		{
			m_apModel[nCntSetParts]->SetParent(m_apModel[m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent]);
		}
		else
		{
			m_apModel[nCntSetParts]->SetParent(NULL);
		}

		if (m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nStart != 1)
		{
			ChangeObject(nCntSetParts, -1);
		}

		(*pCntParts)++;	// �p�[�c�̃J�E���g�����Z
	}
}

//==========================================================================
// �}�g���b�N�X�ݒ�
//==========================================================================
void CObjectHierarchy::SetmtxWorld(const D3DXMATRIX mtx)
{
	m_mtxWorld = mtx;
}

//==========================================================================
// �}�g���b�N�X�擾
//==========================================================================
D3DXMATRIX CObjectHierarchy::GetmtxWorld(void) const
{
	return m_mtxWorld;
}

//==========================================================================
// ���S�̈ʒu�擾
//==========================================================================
D3DXVECTOR3 CObjectHierarchy::GetCenterPosition(void) const
{
	if (m_apModel[0] == NULL)
	{
		return mylib_const::DEFAULT_VECTOR3;
	}
	return GetPosition() + m_apModel[0]->GetPosition();
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CObjectHierarchy::SetOriginPosition(const D3DXVECTOR3 pos)
{
	m_posOrigin = pos;
}

//==========================================================================
// �ʒu�擾
//==========================================================================
D3DXVECTOR3 CObjectHierarchy::GetOriginPosition(void) const
{
	return m_posOrigin;
}

//==========================================================================
// ���a�ݒ�
//==========================================================================
void CObjectHierarchy::SetRadius(const float fRadius)
{
	m_fRadius = fRadius;
}

//==========================================================================
// ���a�擾
//==========================================================================
float CObjectHierarchy::GetRadius(void) const
{
	return m_fRadius;
}

//==========================================================================
// �I�u�W�F�N�g�L�����N�^�[�I�u�W�F�N�g�̎擾
//==========================================================================
CObjectHierarchy *CObjectHierarchy::GetObjectHierarchy(void)
{
	return this;
}

//==========================================================================
// ���f�����擾
//==========================================================================
int CObjectHierarchy::GetNumModel(void) 
{
	return m_nNumModel;
}

//==========================================================================
// ���f�����ݒ�
//==========================================================================
void CObjectHierarchy::SetNumModel(int nNumModel)
{
	m_nNumModel = nNumModel;
}

//==========================================================================
// ���f�����̎擾
//==========================================================================
CModel **CObjectHierarchy::GetModel(void)
{
	return &m_apModel[0];
}

//==========================================================================
// �ǂݍ��ݏ��擾
//==========================================================================
CObjectHierarchy::Load CObjectHierarchy::GetLoadData(int nIdx)
{
	return m_aLoadData[nIdx];
}

//==========================================================================
// �t�@�C���̃C���f�b�N�X�ԍ��擾
//==========================================================================
int CObjectHierarchy::GetIdxFile(void)
{
	return m_nIdxFile;
}
