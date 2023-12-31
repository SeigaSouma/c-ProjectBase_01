//=============================================================================
// 
//  オブジェクトキャラクター処理 [objectChara.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "objectChara.h"
#include "calculation.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "3D_effect.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CObjectChara::CObjectChara(int nPriority) : CObjectHierarchy(nPriority)
{
	// 値のクリア
	m_fVelocity = 0.0f;		// 移動速度
	m_fRotDest = 0.0f;		// 目標の向き
	m_nLife = 0;			// 体力
	m_nLifeOrigin = 0;		// 元の体力
	m_nMotionStartIdx = 0;	// モーション開始のインデックス番号
	m_nAddScore = 0;		// スコア加算量
}

//==========================================================================
// デストラクタ
//==========================================================================
CObjectChara::~CObjectChara()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CObjectChara *CObjectChara::Create(const std::string pTextFile)
{
	// 生成用のオブジェクト
	CObjectChara *pObjChara = nullptr;

	if (pObjChara == nullptr)
	{

		// メモリの確保
		pObjChara = DEBUG_NEW CObjectChara;

		if (pObjChara != nullptr)
		{// メモリの確保が出来ていたら

			// 初期化処理
			HRESULT hr = pObjChara->SetCharacter(pTextFile);
			if (FAILED(hr))
			{// 失敗していたら
				return nullptr;
			}
		}

		return pObjChara;
	}

	return nullptr;
}

//==========================================================================
// キャラ作成
//==========================================================================
HRESULT CObjectChara::SetCharacter(const std::string pTextFile)
{
	// キャラ作成
	CObjectHierarchy::SetCharacter(pTextFile);

	// モーションの生成処理
	m_pMotion = CMotion::Create(pTextFile);

	// モーションの設定
	if (m_pMotion != nullptr)
	{
		m_pMotion->SetModel(GetModel(), GetNumModel(), this);

		// ポーズのリセット
		m_pMotion->ResetPose(0);
	}

	return S_OK;
}

//==========================================================================
// オブジェクト毎のデータ割り当て
//==========================================================================
void CObjectChara::BindObjectData(int nCntData)
{

	// オブジェクト毎のデータ割り当て
	CObjectHierarchy::BindObjectData(nCntData);

	// 移動速度
	m_fVelocity = m_aLoadData[nCntData].fVelocity;

	// 体力
	m_nLife = m_aLoadData[nCntData].nLife;
	m_nLifeOrigin = m_nLife;

	// スコア加算量
	m_nAddScore = m_aLoadData[nCntData].nAddScore;

	// モーションスタートのインデックス
	m_nMotionStartIdx = m_aLoadData[nCntData].nMotionStartIdx;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObjectChara::Init(void)
{
	// 初期化処理
	CObjectHierarchy::Init();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CObjectChara::Uninit(void)
{
	if (m_pMotion != NULL)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = NULL;
	}

	// 終了処理
	CObjectHierarchy::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CObjectChara::Update(void)
{
	// モーション更新
	if (m_pMotion != nullptr)
	{
		m_pMotion->Update();
	}

	// モーション中の行動処理
	MotionInProgressAction();
}

//==========================================================================
// モーション中の行動処理
//==========================================================================
void CObjectChara::MotionInProgressAction(void)
{
	if (m_pMotion == nullptr)
	{
		return;
	}

	// 情報取得
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	CMotion::Info aInfo = m_pMotion->GetInfo(m_pMotion->GetType());
	int nType = m_pMotion->GetType();

	// 攻撃情報の総数取得
	int nNumAttackInfo = aInfo.nNumAttackInfo;

	for (int nCntAttack = 0; nCntAttack < nNumAttackInfo; nCntAttack++)
	{
		if (aInfo.AttackInfo[nCntAttack] == nullptr)
		{
			continue;
		}

		// 攻撃情報取得
		CMotion::AttackInfo AttackInfo = *aInfo.AttackInfo[nCntAttack];

		if (m_pMotion->IsImpactFrame(AttackInfo))
		{// 衝撃のカウントと同じとき

			// 攻撃時処理
			AttackAction(AttackInfo, nCntAttack);
		}

		// モーションカウンター取得
		float fAllCount = m_pMotion->GetAllCount();
		if (fAllCount > AttackInfo.nMinCnt && fAllCount <= AttackInfo.nMaxCnt)
		{// 攻撃判定中

			// 攻撃判定中処理
			AttackInDicision(AttackInfo, nCntAttack);
		}
	}
}

//==========================================================================
// 攻撃時処理
//==========================================================================
void CObjectChara::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	// 武器の位置
	MyLib::Vector3 weponpos = m_pMotion->GetAttackPosition(GetModel(), ATKInfo);
}

//==========================================================================
// 攻撃判定中処理
//==========================================================================
void CObjectChara::AttackInDicision(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	// 武器の位置
	MyLib::Vector3 weponpos = m_pMotion->GetAttackPosition(GetModel(), ATKInfo);
}

//==========================================================================
// モーションファイル変更
//==========================================================================
void CObjectChara::ChangeMotion(const char* pMotionFile)
{
	if (m_pMotion != NULL)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = NULL;
	}

	// モーションの生成処理
	m_pMotion = CMotion::Create(pMotionFile);

	// モーションの設定
	m_pMotion->SetModel(GetModel(), GetNumModel(), this);

	// ポーズのリセット
	m_pMotion->ResetPose(0);
}

//==========================================================================
// 描画処理
//==========================================================================
void CObjectChara::Draw(void)
{
	// 描画処理
	CObjectHierarchy::Draw();
}

//==========================================================================
// 描画処理
//==========================================================================
void CObjectChara::Draw(D3DXCOLOR col)
{
	// 描画処理
	CObjectHierarchy::Draw(col);
}

//==========================================================================
// 描画処理
//==========================================================================
void CObjectChara::Draw(float fAlpha)
{
	// 描画処理
	CObjectHierarchy::Draw(fAlpha);
}

//==========================================================================
// オブジェクト毎のデータ読み込み
//==========================================================================
void CObjectChara::LoadObjectData(FILE* pFile, const char* pComment)
{
	// オブジェクト毎のデータ読み込み
	CObjectHierarchy::LoadObjectData(pFile, pComment);

	char hoge[MAX_COMMENT];	// コメント

	if (strcmp(pComment, "VELOCITY") == 0)
	{// VELOCITYで移動速度

		fscanf(pFile, "%s", &hoge[0]);	// =の分
		fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].fVelocity);	// 移動速度
		m_fVelocity = m_aLoadData[m_nNumLoad].fVelocity;
	}

	if (strcmp(pComment, "LIFE") == 0)
	{// LIFEで体力

		fscanf(pFile, "%s", &hoge[0]);	// =の分
		fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].nLife);	// 体力
		m_nLife = m_aLoadData[m_nNumLoad].nLife;
		m_nLifeOrigin = m_nLife;	// 元の体力
	}

	if (strcmp(pComment, "MOTION_STARTPARTS") == 0)
	{// MOTION_STARTPARTSでモーション開始のインデックス番号

		fscanf(pFile, "%s", &hoge[0]);	// =の分
		fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].nMotionStartIdx);	// モーション開始のインデックス番号
		m_nMotionStartIdx = m_aLoadData[m_nNumLoad].nMotionStartIdx;
	}

	if (strcmp(pComment, "SCORE") == 0)
	{// LIFEで体力

		fscanf(pFile, "%s", &hoge[0]);	// =の分
		fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].nAddScore);	// 体力
		m_nAddScore = m_aLoadData[m_nNumLoad].nAddScore;
	}

}

//==========================================================================
// パーツ毎のデータ読み込み
//==========================================================================
void CObjectChara::LoadPartsData(FILE* pFile, const char* pComment, int *pCntParts)
{
	// パーツ毎のデータ読み込み
	CObjectHierarchy::LoadPartsData(pFile, pComment, pCntParts);
}

//==========================================================================
// 移動速度設定
//==========================================================================
void CObjectChara::SetVelocity(const float fVelocity)
{
	m_fVelocity = fVelocity;
}

//==========================================================================
// 移動速度設定
//==========================================================================
float CObjectChara::GetVelocity(void) const
{
	return m_fVelocity;
}

//==========================================================================
// 目標の向き設定
//==========================================================================
void CObjectChara::SetRotDest(const float fRotDest)
{
	m_fRotDest = fRotDest;
}

//==========================================================================
// 目標の向き設定
//==========================================================================
float CObjectChara::GetRotDest(void) const
{
	return m_fRotDest;
}

//==========================================================================
// 体力設定
//==========================================================================
void CObjectChara::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//==========================================================================
// 体力取得
//==========================================================================
int CObjectChara::GetLife(void) const
{
	return m_nLife;
}

//==========================================================================
// 元の体力取得
//==========================================================================
int CObjectChara::GetLifeOrigin(void) const
{
	return m_nLifeOrigin;
}

//==========================================================================
// モーション開始のインデックス番号取得
//==========================================================================
int CObjectChara::GetMotionStartIdx(void) const
{
	return m_nMotionStartIdx;
}

//==========================================================================
// スコア加算量取得
//==========================================================================
int CObjectChara::GetAddScoreValue(void) const
{
	return m_nAddScore;
}

//==========================================================================
// モーションオブジェクト取得
//==========================================================================
CMotion* CObjectChara::GetMotion(void)
{
	return m_pMotion;
}

//==========================================================================
// オブジェクトキャラクターオブジェクトの取得
//==========================================================================
CObjectChara* CObjectChara::GetObjectChara(void)
{
	return this;
}
